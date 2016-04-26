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
    // If pattern, then generate an expression for matching on this pattern
    if (IdCtx == IdContext::PATTERN) {
        string Val = "";

        for (auto &Str : GetValueBuilder) {
            Val += Str;
        }

        LastPattern = Val + " == " + Node.str();

        // Else, just output value
    } else {
        ExprStack.top() << "(" << Node.Val << ")";
    }
}

void CCodeGenerator::visit(FloatPattern &Node) {
    // If pattern, then generate an expression for matching on this pattern
    if (IdCtx == IdContext::PATTERN) {
        string Val = "";

        for (auto &Str : GetValueBuilder) {
            Val += Str;
        }

        LastPattern = Val + " == " + Node.str();

        // Else, just output value
    } else {
        ExprStack.top() << "(" << Node.str() << ")";
    }
}

void CCodeGenerator::visit(CharPattern &Node) {
    // If pattern, then generate an expression for matching on this pattern
    if (IdCtx == IdContext::PATTERN) {
        string Val = "";

        for (auto &Str : GetValueBuilder) {
            Val += Str;
        }

        LastPattern = Val + " == " + Node.str();

        // else, just output value
    } else {
        ExprStack.top() << Node.str();
    }
}

void CCodeGenerator::visit(IdPattern &Node) {
    stringstream Assign;
    string Name = "";

    if (IdCtx == IdContext::PATTERN) {
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

        // Since and id, in a pattern is allways true, then last_pattern is just
        // set to "1"
        LastPattern = "1";
    } else {
        bool IsDeclared = false;

        for (auto &Decl : Prog->Decls) {
            if (typeid(*Decl.get()) == typeid(Function) &&
                ((Function *)Decl.get())->Id == Node.Val) {
                IsDeclared = true;
                break;
            }
        }

        if (IsDeclared) {
            ExprStack.top() << "&" << GGlobal;
        }

        ExprStack.top() << GUser << Node.Val;
    }
}

void CCodeGenerator::visit(WildPattern &Node) {

}

void CCodeGenerator::visit(AlgebraicPattern &Node) {

}

void CCodeGenerator::visit(ParPattern &Node) {

}

void CCodeGenerator::visit(BoolPattern &Node) {

}

void CCodeGenerator::visit(StringPattern &Node) {

}