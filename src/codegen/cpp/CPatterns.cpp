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

    // Generate the gotten value from get_value_builder
    for (auto Str : GetValueBuilder) {
        ValGotten += Str;
    }

    Res << "(" << ValGotten << "->" << GSize << " - " << ListOffsets.back()
    << " == " << Node.Patterns.size() << ")";

    for (size_t i = 0; i < Node.Patterns.size(); ++i) {
        // Add "gat_"name"(" and ", i + offset)" to get_value_builder
        GetValueBuilder.insert(GetValueBuilder.begin(),
                               GGenerated + GValueAt + TypeName + "(");
        GetValueBuilder.push_back(", " + to_string(i + ListOffsets.back()) +
                                  ")");

        // Push new offset
        // Lists accesed later down in pattern should not be offseted by the
        // same as current list
        ListOffsets.push_back(0);

        // Generate pattern
        Node.Patterns[i]->accept(*this);

        // Cleanup
        ListOffsets.pop_back();
        GetValueBuilder.pop_back();
        GetValueBuilder.erase(GetValueBuilder.begin());

        // Don't add pattern, if pattern is "1"
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
    string TypeName = getTuple(Node.RetTy);
    bool Empty = true;

    // Iterate through all items in tuple
    for (size_t i = 0; i < Node.Patterns.size(); ++i) {
        // Add ".gi"i"" to get_value_builder
        GetValueBuilder.push_back("." + GGenerated + GItem + to_string(i));

        // Generate pattern
        Node.Patterns[i]->accept(*this);

        // Cleanup
        GetValueBuilder.pop_back();

        // Don't add pattern, if pattern is "1"
        if (LastPattern != "1") {
            if (!Empty)
                Res << " && ";

            Empty = false;
            Res << LastPattern;
        }
    }

    // If empty, then let last_pattern be "1"
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

    // Add "gat_"name"(" and ", offset)" to get_value_builder.
    // This is done, so that patterns on the left of node, will use
    // the first + offset item in the list
    GetValueBuilder.insert(GetValueBuilder.begin(),
                           GGenerated + GValueAt + TypeName + "(");
    GetValueBuilder.push_back(", " + to_string(ListOffsets.back()) + ")");

    // Push new offset.
    // Lists accesed later down in pattern should not be offseted by the same as
    // current list.
    ListOffsets.push_back(0);

    // Generate pattern
    Node.Left->accept(*this);

    // Cleanup
    ListOffsets.pop_back();
    GetValueBuilder.pop_back();
    GetValueBuilder.erase(GetValueBuilder.begin());

    // Don't add pattern, if pattern is "1"
    if (LastPattern != "1") {
        Empty = false;
        Res << LastPattern;
    }

    // Right side of a list split, will be the list, but with the first + offset
    // elements missing.
    // This is why we track an offset, so that we don't clone a list, unless we
    // have to.
    ListOffsets[ListOffsets.size() - 1]++;

    // Generate pattern
    Node.Right->accept(*this);

    // Reverse offset back to what it was
    ListOffsets[ListOffsets.size() - 1]--;

    // Don't add pattern, if pattern is "1"
    if (LastPattern != "1") {
        if (!Empty)
            Res << " && ";

        Empty = false;
        Res << LastPattern;
    }

    // If empty, then let last_pattern be "1"
    if (Empty) {
        LastPattern = "1";
    } else {
        LastPattern = "(" + Res.str() + ")";
    }
}

void CCodeGenerator::visit(IntPattern &Node) {
    string Val = "";

    for (auto &Str : GetValueBuilder) {
        Val += Str;
    }

    LastPattern = Val + " == " + Node.str();
}

void CCodeGenerator::visit(FloatPattern &Node) {
    string Val = "";

    for (auto &Str : GetValueBuilder) {
        Val += Str;
    }

    LastPattern = Val + " == " + Node.str();
}

void CCodeGenerator::visit(CharPattern &Node) {
    string Val = "";

    for (auto &Str : GetValueBuilder) {
        Val += Str;
    }

    LastPattern = Val + " == " + Node.str();
}

void CCodeGenerator::visit(BoolPattern &Node) {
    string Val = "";

    for (auto &Str : GetValueBuilder) {
        Val += Str;
    }

    LastPattern = Val + " == " + to_string(Node.Val);
}

void CCodeGenerator::visit(IdPattern &Node) {
    stringstream Assign;
    string Name = "";

    // Created name from get_value_builder
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

    // Save the assigment untill after the pattern has been generated
    Assignments.push_back(Assign.str());

    // Since an id, in a pattern is always true, then last_pattern is just
    // set to "1"
    LastPattern = "1";
}

void CCodeGenerator::visit(WildPattern &Node) {
    // Since a wildcard, in a pattern is always true, then last_pattern is just
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
    string Val = "";

    for (auto &Str : GetValueBuilder) {
        Val += Str;
    }

    // gcompare_string is generated by generate_std. it compares the custome
    // string type, to a char*.
    // It also takes an offset, for when list splits occur on strings
    LastPattern = GGenerated + GCompare + GString + "(" + Val + ", " +
                  Node.str() + ", " + to_string(ListOffsets.back()) + ")";
}