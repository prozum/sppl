#include "CppCodeGenerator.h"

using namespace common;
using namespace std;
using namespace codegen;

void CCodeGenerator::visit(ListPattern &Node) {
    // Result is needed, so we don't generate something else, while the
    // ListPattern is being generated
    stringstream Res;
    string ValGotten;
    string TypeName = getList(Node.RetTy);

    // Get the actual list from the function arguments, but putting together
    // everything that is in GetValueBuilder
    for (auto Str : GetValueBuilder) {
        ValGotten += Str;
    }

    Res << "(" << ValGotten << "->" << GSize << " - " << ListOffsets.back() << " == " << Node.Patterns.size() << ")";

    for (size_t i = 0; i < Node.Patterns.size(); ++i) {
        // Insert the way in which the current item of the list should be dereferenced so that later
        // patterns can match based on it
        GetValueBuilder.insert(GetValueBuilder.begin(), GGenerated + GValueAt + TypeName + "(");
        GetValueBuilder.push_back(", " + to_string(i + ListOffsets.back()) + ")");

        // Push a new offset on a stack. Offsets are used when ListSplits occure so that we don't need to make a
        // copy of the lists tail when list splits occure
        ListOffsets.push_back(0);

        Node.Patterns[i]->accept(*this);

        ListOffsets.pop_back();
        GetValueBuilder.pop_back();
        GetValueBuilder.erase(GetValueBuilder.begin());

        if (LastPattern != "1") {
            Res << " && " << LastPattern;
        }
    }

    LastPattern = "(" + Res.str() + ")";
}

void CCodeGenerator::visit(TuplePattern &Node) {
    // Result is needed, so we don't start generating something in a signature
    // in the header file
    stringstream Res;
    bool Empty = true;

    for (size_t i = 0; i < Node.Patterns.size(); ++i) {
        // Insert the way in which the current item of the tuple should be dereferenced so that later
        // patterns can match based on it
        GetValueBuilder.push_back("." + GGenerated + GItem + to_string(i));

        Node.Patterns[i]->accept(*this);

        GetValueBuilder.pop_back();

        if (LastPattern != "1") {
            if (!Empty)
                Res << " && ";

            Empty = false;
            Res << LastPattern;
        }
    }

    // If empty, then let LastPattern be "1"
    if (Empty) {
        LastPattern = "1";
    } else {
        LastPattern = "(" + Res.str() + ")";
    }
}

void CCodeGenerator::visit(ListSplit &Node) {
    stringstream Res;
    string TypeName = getList(Node.RetTy);
    bool Empty = true;

    // Insert the way in which the first item of the list should be dereferenced so that later
    // patterns can match based on it
    GetValueBuilder.insert(GetValueBuilder.begin(), GGenerated + GValueAt + TypeName + "(");
    GetValueBuilder.push_back(", " + to_string(ListOffsets.back()) + ")");
    ListOffsets.push_back(0);

    Node.Left->accept(*this);

    ListOffsets.pop_back();
    GetValueBuilder.pop_back();
    GetValueBuilder.erase(GetValueBuilder.begin());

    if (LastPattern != "1") {
        Empty = false;
        Res << LastPattern;
    }

    // All the patterns that occure on the right side of a ListSplit is offset by one
    ListOffsets[ListOffsets.size() - 1]++;

    Node.Right->accept(*this);

    ListOffsets[ListOffsets.size() - 1]--;

    if (LastPattern != "1") {
        if (!Empty)
            Res << " && ";

        Empty = false;
        Res << LastPattern;
    }

    // If empty, then let LastPattern be "1"
    if (Empty) {
        LastPattern = "1";
    } else {
        LastPattern = "(" + Res.str() + ")";
    }
}

void CCodeGenerator::visit(IntPattern &Node) {
    string Val;

    for (auto &Str : GetValueBuilder) {
        Val += Str;
    }

    LastPattern = Val + " == " + Node.str();
}

void CCodeGenerator::visit(FloatPattern &Node) {
    string Val;

    for (auto &Str : GetValueBuilder) {
        Val += Str;
    }

    LastPattern = Val + " == " + Node.str();
}

void CCodeGenerator::visit(CharPattern &Node) {
    string Val;

    for (auto &Str : GetValueBuilder) {
        Val += Str;
    }

    LastPattern = Val + " == " + Node.str();
}

void CCodeGenerator::visit(BoolPattern &Node) {
    string Val;

    for (auto &Str : GetValueBuilder) {
        Val += Str;
    }

    LastPattern = Val + " == " + to_string(Node.Val);
}

void CCodeGenerator::visit(IdPattern &Node) {
    stringstream Assign;
    string Name;

    for (auto &Str : GetValueBuilder) {
        Name += Str;
    }

    // Generate an assignment for the id, which should occur after the
    // if-statement
    Assign << getType(Node.RetTy) << " " << GUser << Node.Val << " = ";

    if ((Node.RetTy.Id == TypeId::LIST ||
         Node.RetTy.Id == TypeId::STRING) &&
         ListOffsets.back() > 0) {
        Assign << GGenerated << GAt << getList(Node.RetTy) << "(" << Name
               << ", " << ListOffsets.back() << ");";
    } else {
        Assign << Name << ";";
    }

    // Save the assigment until after the pattern has been generated
    Assignments.push_back(Assign.str());

    // Since an id, in a pattern is always true, then LastPattern is just
    // set to "1"
    LastPattern = "1";
}

void CCodeGenerator::visit(WildPattern &Node) {
    // Since a wildcard, in a pattern is always true, then LastPattern is just
    // set to "1"
    LastPattern = "1";
}

void CCodeGenerator::visit(AlgebraicPattern &Node) {
    // TODO
    throw std::runtime_error("Not implemented");
}

void CCodeGenerator::visit(ParPattern &Node) {
    Node.Pat->accept(*this);

    if (LastPattern != "1") {
        LastPattern = "(" + LastPattern + ")";
    }
}

void CCodeGenerator::visit(StringPattern &Node) {
    string Val;

    for (auto &Str : GetValueBuilder) {
        Val += Str;
    }

    // GCompare GString is generated by generate_std. it compares the custome
    // string type, to a char*.
    // It also takes an offset, for when list splits occur on strings
    LastPattern = GGenerated + GCompare + GString + "(" + Val + ", " +
                  Node.str() + ", " + to_string(ListOffsets.back()) + ")";
}