//
// Created by hejsil on 3/4/16.
//

#include "TypeChecker.h"
#include "Node.h"
#include <vector>

using namespace std;

void TypeChecker::visit(Program *node) {
    /* Visit all children */
    for (auto func : node->funcs) {
        func->accept(this);
    }
    /* Visit stops here */
}

void TypeChecker::visit(Function *node) {
    current_func = node;

    /* Visit children */
    for (auto type : node->types) {
        type->accept(this);
    }
    for (auto cse : node->cases) {
        cse->accept(this);
    }
    /* Visit stops here */
}

void TypeChecker::visit(Case *node) {
    type_stack.push(vector<Type>());

    /* Visit children */
    for (auto pattern : node->patterns) {
        pattern->accept(this);
    }
    node->expr->accept(this);
    /* Visit stops here */

    for (int i = 0; i < current_func->types.size(); ++i) {

    }

    type_stack.pop();
}

void TypeChecker::visit(Or *node) {
    /* Code goes here */


    /* Code stops here */

    /* Visit children */
    node->left->accept(this);
    node->right->accept(this);
    /* Visit stops here */

    /* Code goes here */


    /* Code stops here */
}

void TypeChecker::visit(And *node) {
    /* Code goes here */


    /* Code stops here */

    /* Visit children */
    node->left->accept(this);
    node->right->accept(this);
    /* Visit stops here */

    /* Code goes here */


    /* Code stops here */
}

void TypeChecker::visit(Equal *node) {
    /* Code goes here */


    /* Code stops here */

    /* Visit children */
    node->left->accept(this);
    node->right->accept(this);
    /* Visit stops here */

    /* Code goes here */


    /* Code stops here */
}

void TypeChecker::visit(NotEqual *node) {
    /* Code goes here */


    /* Code stops here */

    /* Visit children */
    node->left->accept(this);
    node->right->accept(this);
    /* Visit stops here */

    /* Code goes here */


    /* Code stops here */
}

void TypeChecker::visit(Lesser *node) {
    /* Code goes here */


    /* Code stops here */

    /* Visit children */
    node->left->accept(this);
    node->right->accept(this);
    /* Visit stops here */

    /* Code goes here */


    /* Code stops here */
}

void TypeChecker::visit(Greater *node) {
    /* Code goes here */


    /* Code stops here */

    /* Visit children */
    node->left->accept(this);
    node->right->accept(this);
    /* Visit stops here */

    /* Code goes here */


    /* Code stops here */
}

void TypeChecker::visit(LesserEq *node) {
    /* Code goes here */


    /* Code stops here */

    /* Visit children */
    node->left->accept(this);
    node->right->accept(this);
    /* Visit stops here */

    /* Code goes here */


    /* Code stops here */
}

void TypeChecker::visit(GreaterEq *node) {
    /* Code goes here */


    /* Code stops here */

    /* Visit children */
    node->left->accept(this);
    node->right->accept(this);
    /* Visit stops here */

    /* Code goes here */


    /* Code stops here */
}

void TypeChecker::visit(Add *node) {
    /* Code goes here */


    /* Code stops here */

    /* Visit children */
    node->left->accept(this);
    node->right->accept(this);
    /* Visit stops here */

    /* Code goes here */


    /* Code stops here */
}

void TypeChecker::visit(Sub *node) {
    /* Code goes here */


    /* Code stops here */

    /* Visit children */
    node->left->accept(this);
    node->right->accept(this);
    /* Visit stops here */

    /* Code goes here */


    /* Code stops here */
}

void TypeChecker::visit(Mul *node) {
    /* Code goes here */


    /* Code stops here */

    /* Visit children */
    node->left->accept(this);
    node->right->accept(this);
    /* Visit stops here */

    /* Code goes here */


    /* Code stops here */
}

void TypeChecker::visit(Div *node) {
    /* Code goes here */


    /* Code stops here */

    /* Visit children */
    node->left->accept(this);
    node->right->accept(this);
    /* Visit stops here */

    /* Code goes here */


    /* Code stops here */
}

void TypeChecker::visit(Mod *node) {
    /* Code goes here */


    /* Code stops here */

    /* Visit children */
    node->left->accept(this);
    node->right->accept(this);
    /* Visit stops here */

    /* Code goes here */


    /* Code stops here */
}

void TypeChecker::visit(ListAdd *node) {
    /* Code goes here */


    /* Code stops here */

    /* Visit children */
    node->left->accept(this);
    node->right->accept(this);
    /* Visit stops here */

    /* Code goes here */


    /* Code stops here */
}

void TypeChecker::visit(Par *node) {
    /* Code goes here */


    /* Code stops here */

    /* Visit children */
    node->child->accept(this);
    /* Visit stops here */

    /* Code goes here */


    /* Code stops here */
}

void TypeChecker::visit(Not *node) {
    /* Code goes here */


    /* Code stops here */

    /* Visit children */
    node->child->accept(this);
    /* Visit stops here */

    /* Code goes here */


    /* Code stops here */
}

void TypeChecker::visit(Int *node) {
    /* Code goes here */


    /* Code stops here */
}

void TypeChecker::visit(Float *node) {
    /* Code goes here */


    /* Code stops here */
}

void TypeChecker::visit(Bool *node) {
    /* Code goes here */


    /* Code stops here */
}

void TypeChecker::visit(Char *node) {
    /* Code goes here */


    /* Code stops here */
}

void TypeChecker::visit(String *node) {
    /* Code goes here */


    /* Code stops here */
}

void TypeChecker::visit(ListPattern *node) {
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

void TypeChecker::visit(TuplePattern *node) {
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

void TypeChecker::visit(ListSplit *node) {
    /* Code goes here */


    /* Code stops here */

    /* Visit children */
    node->left->accept(this);
    node->right->accept(this);
    /* Visit stops here */

    /* Code goes here */


    /* Code stops here */
}

void TypeChecker::visit(List *node) {
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

void TypeChecker::visit(Tuple *node) {
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

void TypeChecker::visit(Id *node) {
    /* Code goes here */


    /* Code stops here */
}

void TypeChecker::visit(Call *node) {
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

void TypeChecker::visit(Type *node) {
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

bool TypeChecker::check_types(Type *type1, Type *type2) {
    TypeEnum cast = type_cast(type1->type);
    if (cast == type_cast(type2->type)){
        switch (cast) {
            case LIST:
                return check_types(type1->types[0], type2->types[0]);
            case SIGNATURE:
            case TUPLE:
                for (int i = 0; i < type1->types.size(); ++i) {
                    if (!check_types(type1->types[i], type2->types[i])){
                        return false;
                    }
                }
            default:
                return true;
        }
    } else {
        return false;
    }
}

TypeEnum TypeChecker::type_cast(TypeEnum type) {
    return type;
}
