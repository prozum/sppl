//
// Created by hejsil on 3/2/16.
//

#include "HPInkJetVisitor.h"
#include <iostream>
using namespace std;

void HPInkJetVisitor::visit(Program *node) {
    res = "";

    cout << "Program " << endl;

    for (Node * func : node->funcs) {
        func->accept(this);
    }
}

void HPInkJetVisitor::visit(Function *node) {
    cout << "Function " << node->id << endl;

    res.append("def ");
    res.append(node->id);
    res.append(" ");

    node->sig->accept(this);

    res.append("\n");

    for (Node * _case : node->cases) {
        _case->accept(this);
        res.append("\n");
    }

    res.append("\n");
}

void HPInkJetVisitor::visit(Case *node) {

    cout << "Case" << endl;

    res.append("\t| ");

    for (int i = 0; i < node->patterns.size() - 1; ++i) {
        node->patterns[i]->accept(this);
        res.append(" ");
    }

    if (node->patterns.size() != 0)
        node->patterns.back()->accept(this);

    res.append(" = ");

    node->expr->accept(this);
}

void HPInkJetVisitor::visit(Or *node) {

    cout << "Or" << endl;

    node->left->accept(this);
    res.append(" || ");
    node->right->accept(this);
}

void HPInkJetVisitor::visit(And *node) {

    cout << "And" << endl;

    node->left->accept(this);
    res.append(" && ");
    node->right->accept(this);
}

void HPInkJetVisitor::visit(Equal *node) {

    cout << "Equal" << endl;

    node->left->accept(this);
    res.append(" == ");
    node->right->accept(this);
}

void HPInkJetVisitor::visit(NotEqual *node) {

    cout << "Not Equal" << endl;

    node->left->accept(this);
    res.append(" != ");
    node->right->accept(this);
}

void HPInkJetVisitor::visit(Lesser *node) {

    cout << "Lesser" << endl;

    node->left->accept(this);
    res.append(" < ");
    node->right->accept(this);
}

void HPInkJetVisitor::visit(Greater *node) {

    cout << "Greater" << endl;

    node->left->accept(this);
    res.append(" > ");
    node->right->accept(this);
}

void HPInkJetVisitor::visit(LesserEq *node) {

    cout << "LesserEq" << endl;

    node->left->accept(this);
    res.append(" <= ");
    node->right->accept(this);
}

void HPInkJetVisitor::visit(GreaterEq *node) {

    cout << "GreaterEq" << endl;

    node->left->accept(this);
    res.append(" >= ");
    node->right->accept(this);
}

void HPInkJetVisitor::visit(Add *node) {

    cout << "Add" << endl;

    node->left->accept(this);
    res.append(" + ");
    node->right->accept(this);
}

void HPInkJetVisitor::visit(Sub *node) {

    cout << "Sub" << endl;

    node->left->accept(this);
    res.append(" - ");
    node->right->accept(this);
}

void HPInkJetVisitor::visit(Mul *node) {

    cout << "Mul" << endl;

    node->left->accept(this);
    res.append(" * ");
    node->right->accept(this);
}

void HPInkJetVisitor::visit(Div *node) {

    cout << "Div" << endl;

    node->left->accept(this);
    res.append(" / ");
    node->right->accept(this);
}

void HPInkJetVisitor::visit(Mod *node) {

    cout << "Mod" << endl;

    node->left->accept(this);
    res.append(" % ");
    node->right->accept(this);
}

void HPInkJetVisitor::visit(ListAdd *node) {

    cout << "ListAdd" << endl;

    node->left->accept(this);
    res.append(" : ");
    node->right->accept(this);
}

void HPInkJetVisitor::visit(Par *node) {

    cout << "Par" << endl;

    res.append("(");
    node->child->accept(this);
    res.append(")");
}

void HPInkJetVisitor::visit(Not *node) {

    cout << "Not" << endl;

    res.append("!");
    node->child->accept(this);
}

void HPInkJetVisitor::visit(Int *node) {

    cout << "Int" << endl;

    res.append(to_string(node->value));
}

void HPInkJetVisitor::visit(Float *node) {

    cout << "Float" << endl;

    res.append(to_string(node->value));
}

void HPInkJetVisitor::visit(Bool *node) {

    cout << "Bool" << endl;

    res.append(to_string(node->value));
}

void HPInkJetVisitor::visit(Char *node) {

    cout << "Char" << endl;

    res.append("'");
    res.append(to_string(node->value));
    res.append("'");
}

void HPInkJetVisitor::visit(String *node) {

    cout << "String" << endl;

    res.append("\"");
    res.append(node->value);
    res.append("\"");
}

void HPInkJetVisitor::visit(List *node) {

    cout << "List" << endl;

    res.append("[");

    for (int i = 0; i < node->exprs.size() - 1; ++i) {
        node->exprs[i]->accept(this);
        res.append(", ");
    }

    if (node->exprs.size() != 0)
        node->exprs.back()->accept(this);

    res.append("]");
}

void HPInkJetVisitor::visit(Tuple *node) {

    cout << "Tuple" << endl;

    res.append("(");

    for (int i = 0; i < node->exprs.size() - 1; ++i) {
        node->exprs[i]->accept(this);
        res.append(", ");
    }

    if (node->exprs.size() != 0)
        node->exprs.back()->accept(this);

    res.append(")");
}

void HPInkJetVisitor::visit(Id *node) {

    cout << "Id" << endl;

    res.append(node->id);
}

void HPInkJetVisitor::visit(Call *node) {

    cout << "Call" << endl;

    node->callee->accept(this);
    res.append("(");

    for (int i = 0; i < node->exprs.size() - 1; ++i) {
        node->exprs[i]->accept(this);
        res.append(", ");
    }

    if (node->exprs.size() != 0)
        node->exprs.back()->accept(this);

    res.append(")");
}

void HPInkJetVisitor::visit(IntType *node) {

    cout << "IntType" << endl;

    res.append("Int");
}

void HPInkJetVisitor::visit(FloatType *node) {

    cout << "FloatType" << endl;

    res.append("Float");
}

void HPInkJetVisitor::visit(BoolType *node) {

    cout << "BoolType" << endl;

    res.append("Bool");
}

void HPInkJetVisitor::visit(CharType *node) {

    cout << "CharType" << endl;

    res.append("Char");
}

void HPInkJetVisitor::visit(StringType *node) {

    cout << "StringType" << endl;

    res.append("String");
}

void HPInkJetVisitor::visit(ListType *node) {

    cout << "ListType" << endl;

    res.append("[");
    node->type->accept(this);
    res.append("]");
}

void HPInkJetVisitor::visit(TupleType *node) {

    cout << "TupleType" << endl;

    res.append("(");

    for (int i = 0; i < node->types.size() - 1; ++i) {
        node->types[i]->accept(this);
        res.append(", ");
    }

    if (node->types.size() != 0)
        node->types.back()->accept(this);

    res.append(")");
}

void HPInkJetVisitor::visit(Signature *node) {

    cout << "Signature" << endl;

    res.append("(");

    for (int i = 0; i < node->types.size() - 1; ++i) {
        node->types[i]->accept(this);
        res.append(" -> ");
    }

    if (node->types.size() != 0)
        node->types.back()->accept(this);

    res.append(")");
}

void HPInkJetVisitor::visit(ListPattern *node) {

    cout << "ListPattern" << endl;

    res.append("[");

    for (int i = 0; i < node->patterns.size() - 1; ++i) {
        node->patterns[i]->accept(this);
        res.append(", ");
    }

    if (node->patterns.size() != 0)
        node->patterns.back()->accept(this);

    res.append("]");
}

void HPInkJetVisitor::visit(TuplePattern *node) {

    cout << "TuplePattern" << endl;

    res.append("(");

    for (int i = 0; i < node->patterns.size() - 1; ++i) {
        node->patterns[i]->accept(this);
        res.append(", ");
    }

    if (node->patterns.size() != 0)
        node->patterns.back()->accept(this);

    res.append(")");
}

void HPInkJetVisitor::visit(ListSplit *node) {

    cout << "ListSplit" << endl;

    res.append("(");
    node->left->accept(this);
    res.append(" : ");
    node->right->accept(this);
    res.append(")");
}
