//
// Created by hejsil on 3/4/16.
//

#include "SkeletonVisitor.h"

void SkeletonVisitor::visit(Program *node) {
    /* Code goes here */


    /* Code stops here */

    /* Visit all children */
    for (auto func : node->funcs) {
        func->accept(this);
    }
    /* Visit stops here */

    /* Code goes here */


    /* Code stops here */
}

void SkeletonVisitor::visit(Function *node) {
    /* Code goes here */


    /* Code stops here */

    /* Visit children */
    for (auto type : node->types) {
        type->accept(this);
    }
    for (auto cse : node->cases) {
        cse->accept(this);
    }
    /* Visit stops here */

    /* Code goes here */


    /* Code stops here */
}

void SkeletonVisitor::visit(Case *node) {
    /* Code goes here */


    /* Code stops here */

    /* Visit children */
    for (auto pattern : node->patterns) {
        pattern->accept(this);
    }
    node->expr->accept(this);
    /* Visit stops here */

    /* Code goes here */


    /* Code stops here */
}

void SkeletonVisitor::visit(Or *node) {
    /* Code goes here */


    /* Code stops here */

    /* Visit children */
    node->left->accept(this);
    node->right->accept(this);
    /* Visit stops here */

    /* Code goes here */


    /* Code stops here */
}

void SkeletonVisitor::visit(And *node) {
    /* Code goes here */


    /* Code stops here */

    /* Visit children */
    node->left->accept(this);
    node->right->accept(this);
    /* Visit stops here */

    /* Code goes here */


    /* Code stops here */
}

void SkeletonVisitor::visit(Equal *node) {
    /* Code goes here */


    /* Code stops here */

    /* Visit children */
    node->left->accept(this);
    node->right->accept(this);
    /* Visit stops here */

    /* Code goes here */


    /* Code stops here */
}

void SkeletonVisitor::visit(NotEqual *node) {
    /* Code goes here */


    /* Code stops here */

    /* Visit children */
    node->left->accept(this);
    node->right->accept(this);
    /* Visit stops here */

    /* Code goes here */


    /* Code stops here */
}

void SkeletonVisitor::visit(Lesser *node) {
    /* Code goes here */


    /* Code stops here */

    /* Visit children */
    node->left->accept(this);
    node->right->accept(this);
    /* Visit stops here */

    /* Code goes here */


    /* Code stops here */
}

void SkeletonVisitor::visit(Greater *node) {
    /* Code goes here */


    /* Code stops here */

    /* Visit children */
    node->left->accept(this);
    node->right->accept(this);
    /* Visit stops here */

    /* Code goes here */


    /* Code stops here */
}

void SkeletonVisitor::visit(LesserEq *node) {
    /* Code goes here */


    /* Code stops here */

    /* Visit children */
    node->left->accept(this);
    node->right->accept(this);
    /* Visit stops here */

    /* Code goes here */


    /* Code stops here */
}

void SkeletonVisitor::visit(GreaterEq *node) {
    /* Code goes here */


    /* Code stops here */

    /* Visit children */
    node->left->accept(this);
    node->right->accept(this);
    /* Visit stops here */

    /* Code goes here */


    /* Code stops here */
}

void SkeletonVisitor::visit(Add *node) {
    /* Code goes here */


    /* Code stops here */

    /* Visit children */
    node->left->accept(this);
    node->right->accept(this);
    /* Visit stops here */

    /* Code goes here */


    /* Code stops here */
}

void SkeletonVisitor::visit(Sub *node) {
    /* Code goes here */


    /* Code stops here */

    /* Visit children */
    node->left->accept(this);
    node->right->accept(this);
    /* Visit stops here */

    /* Code goes here */


    /* Code stops here */
}

void SkeletonVisitor::visit(Mul *node) {
    /* Code goes here */


    /* Code stops here */

    /* Visit children */
    node->left->accept(this);
    node->right->accept(this);
    /* Visit stops here */

    /* Code goes here */


    /* Code stops here */
}

void SkeletonVisitor::visit(Div *node) {
    /* Code goes here */


    /* Code stops here */

    /* Visit children */
    node->left->accept(this);
    node->right->accept(this);
    /* Visit stops here */

    /* Code goes here */


    /* Code stops here */
}

void SkeletonVisitor::visit(Mod *node) {
    /* Code goes here */


    /* Code stops here */

    /* Visit children */
    node->left->accept(this);
    node->right->accept(this);
    /* Visit stops here */

    /* Code goes here */


    /* Code stops here */
}

void SkeletonVisitor::visit(ListAdd *node) {
    /* Code goes here */


    /* Code stops here */

    /* Visit children */
    node->left->accept(this);
    node->right->accept(this);
    /* Visit stops here */

    /* Code goes here */


    /* Code stops here */
}

void SkeletonVisitor::visit(Par *node) {
    /* Code goes here */


    /* Code stops here */

    /* Visit children */
    node->child->accept(this);
    /* Visit stops here */

    /* Code goes here */


    /* Code stops here */
}

void SkeletonVisitor::visit(Not *node) {
    /* Code goes here */


    /* Code stops here */

    /* Visit children */
    node->child->accept(this);
    /* Visit stops here */

    /* Code goes here */


    /* Code stops here */
}

void SkeletonVisitor::visit(Int *node) {
    /* Code goes here */


    /* Code stops here */
}

void SkeletonVisitor::visit(Float *node) {
    /* Code goes here */


    /* Code stops here */
}

void SkeletonVisitor::visit(Bool *node) {
    /* Code goes here */


    /* Code stops here */
}

void SkeletonVisitor::visit(Char *node) {
    /* Code goes here */


    /* Code stops here */
}

void SkeletonVisitor::visit(String *node) {
    /* Code goes here */


    /* Code stops here */
}

void SkeletonVisitor::visit(ListPattern *node) {
    /* Code goes here */


    /* Code stops here */

    /* Visit children */
    for (auto pattern : node->patterns) {
        pattern->accept(this);
    }
    /* Visit stops here */

    /* Code goes here */


    /* Code stops here */
}

void SkeletonVisitor::visit(TuplePattern *node) {
    /* Code goes here */


    /* Code stops here */

    /* Visit children */
    for (auto pattern : node->patterns) {
        pattern->accept(this);
    }
    /* Visit stops here */

    /* Code goes here */


    /* Code stops here */
}

void SkeletonVisitor::visit(ListSplit *node) {
    /* Code goes here */


    /* Code stops here */

    /* Visit children */
    node->left->accept(this);
    node->right->accept(this);
    /* Visit stops here */

    /* Code goes here */


    /* Code stops here */
}

void SkeletonVisitor::visit(List *node) {
    /* Code goes here */


    /* Code stops here */

    /* Visit children */
    for (auto expr : node->exprs) {
        expr->accept(this);
    }
    /* Visit stops here */

    /* Code goes here */


    /* Code stops here */
}

void SkeletonVisitor::visit(Tuple *node) {
    /* Code goes here */


    /* Code stops here */

    /* Visit children */
    for (auto expr : node->exprs) {
        expr->accept(this);
    }
    /* Visit stops here */

    /* Code goes here */


    /* Code stops here */
}

void SkeletonVisitor::visit(Id *node) {
    /* Code goes here */


    /* Code stops here */
}

void SkeletonVisitor::visit(Call *node) {
    /* Code goes here */


    /* Code stops here */

    /* Visit children */
    node->callee->accept(this);
    for (auto expr : node->exprs) {
        expr->accept(this);
    }
    /* Visit stops here */

    /* Code goes here */


    /* Code stops here */
}

void SkeletonVisitor::visit(LiteralType *node) {
    /* Code goes here */


    /* Code stops here */
}

void SkeletonVisitor::visit(ListType *node) {
    /* Code goes here */


    /* Code stops here */

    /* Visit children */
    node->type->accept(this);
    /* Visit stops here */

    /* Code goes here */


    /* Code stops here */
}

void SkeletonVisitor::visit(TupleType *node) {
    /* Code goes here */


    /* Code stops here */

    /* Visit children */
    for (auto type : node->types) {
        type->accept(this);
    }
    /* Visit stops here */

    /* Code goes here */


    /* Code stops here */
}

void SkeletonVisitor::visit(Signature *node) {
    /* Code goes here */


    /* Code stops here */

    /* Visit children */
    for (auto type : node->types) {
        type->accept(this);
    }
    /* Visit stops here */

    /* Code goes here */


    /* Code stops here */
}
