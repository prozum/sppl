#include "CCodeGenOld.h"

using namespace common;
using namespace std;
using namespace codegen;

void CCodeGenOld::visit(ListPattern &Node) {
    // Result is needed, so we don't generate something else, while the
    // ListPattern is being generated
    stringstream Res;
    string ValGotten;
    string TypeName = getList(Node.RetTy);

    // Get the actual list from the function arguments, but putting together
    // everything that is in GetValueBuilder
    for (auto Str : PatternBuilder) {
        ValGotten += Str;
    }

    Res << "(" << ValGotten << "->" << GSize << " - " << ListOffsets.back() << " == " << Node.Patterns.size() << ")";

    for (size_t i = 0; i < Node.Patterns.size(); ++i) {
        // Insert the way in which the current item of the list should be dereferenced so that later
        // patterns can match based on it
        PatternBuilder.insert(PatternBuilder.begin(), GGenerated + GAt + TypeName + "(");
        PatternBuilder.push_back(", " + to_string(i + ListOffsets.back()) + ")->" + GValue);

        // Push a new offset on a stack. Offsets are used when ListSplits occure so that we don't need to make a
        // copy of the lists tail when list splits occure
        ListOffsets.push_back(0);

        Node.Patterns[i]->accept(*this);

        ListOffsets.pop_back();
        PatternBuilder.pop_back();
        PatternBuilder.erase(PatternBuilder.begin());

        if (!LastPattern.empty()) {
            Res << " && " << LastPattern;
        }
    }

    LastPattern = "(" + Res.str() + ")";
}

void CCodeGenOld::visit(TuplePattern &Node) {
    stringstream Res;
    bool Empty = true;

    for (size_t i = 0; i < Node.Patterns.size(); ++i) {
        // Insert the way in which the current item of the tuple should be dereferenced so that later
        // patterns can match based on it
        PatternBuilder.push_back("." + GItem + to_string(i));

        Node.Patterns[i]->accept(*this);

        PatternBuilder.pop_back();

        if (!LastPattern.empty()) {
            if (!Empty)
                Res << " && ";
            else
                Empty = false;

            Res << LastPattern;
        }
    }

    // If empty, then let LastPattern be ""
    if (Empty) {
        LastPattern.clear();
    } else {
        LastPattern = "(" + Res.str() + ")";
    }
}

void CCodeGenOld::visit(ListSplit &Node) {
    stringstream Res;
    string TypeName = getList(Node.RetTy);
    bool Empty = true;

    // Insert the way in which the first item of the list should be dereferenced so that later
    // patterns can match based on it
    PatternBuilder.insert(PatternBuilder.begin(), GGenerated + GAt + TypeName + "(");
    PatternBuilder.push_back(", " + to_string(ListOffsets.back()) + ")->" + GValue);
    ListOffsets.push_back(0);

    Node.Left->accept(*this);

    ListOffsets.pop_back();
    PatternBuilder.pop_back();
    PatternBuilder.erase(PatternBuilder.begin());

    if (!LastPattern.empty()) {
        Empty = false;
        Res << LastPattern;
    }

    // All the patterns that occure on the right side of a ListSplit is offset by one
    ListOffsets[ListOffsets.size() - 1]++;

    Node.Right->accept(*this);

    ListOffsets[ListOffsets.size() - 1]--;

    if (!LastPattern.empty()) {
        if (!Empty)
            Res << " && ";
        else
            Empty = false;

        Res << LastPattern;
    }

    // If empty, then let LastPattern be ""
    if (Empty) {
        LastPattern.clear();
    } else {
        LastPattern = "(" + Res.str() + ")";
    }
}

void CCodeGenOld::visit(IntPattern &Node) {
    string Val;

    for (auto &Str : PatternBuilder) {
        Val += Str;
    }

    LastPattern = Val + " == " + Node.str();
}

void CCodeGenOld::visit(FloatPattern &Node) {
    string Val;

    for (auto &Str : PatternBuilder) {
        Val += Str;
    }

    LastPattern = Val + " == " + Node.str();
}

void CCodeGenOld::visit(CharPattern &Node) {
    string Val;

    for (auto &Str : PatternBuilder) {
        Val += Str;
    }

    LastPattern = Val + " == " + Node.str();
}

void CCodeGenOld::visit(BoolPattern &Node) {
    string Val;

    for (auto &Str : PatternBuilder) {
        Val += Str;
    }

    LastPattern = Val + " == " + to_string(Node.Val);
}

void CCodeGenOld::visit(IdPattern &Node) {
    stringstream Assign;
    string Name;

    for (auto &Str : PatternBuilder) {
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
    // set to ""
    LastPattern.clear();
}

void CCodeGenOld::visit(WildPattern &Node) {
    // Since a wildcard, in a pattern is always true, then LastPattern is just
    // set to ""
    LastPattern.clear();
}

void CCodeGenOld::visit(AlgebraicPattern &Node) {
    // TODO
    throw std::runtime_error("Not implemented");
}

void CCodeGenOld::visit(ParPattern &Node) {
    Node.Pat->accept(*this);

    if (!LastPattern.empty()) {
        LastPattern = "(" + LastPattern + ")";
    }
}

void CCodeGenOld::visit(StringPattern &Node) {
    string Val;

    for (auto &Str : PatternBuilder) {
        Val += Str;
    }

    // GCompare GString is generated by generate_std. it compares the custome
    // string type, to a char*.
    // It also takes an offset, for when list splits occur on strings
    LastPattern = GGenerated + GCompare + GString + "(" + Val + ", " +
                  Node.str() + ", " + to_string(ListOffsets.back()) + ")";
}