//
// Created by hejsil on 3/4/16.
//

#include "TypeChecker.h"
#include "Node.h"
#include <vector>
#include <bits/unordered_map.h>
#include "Scope.h"

using namespace std;




void TypeChecker::visit(Program &node) {
    /* Visit all children */
    for (auto func : node.funcs) {
        func->accept(this);
    }
    /* Visit stops here */
}

void TypeChecker::visit(Function &node) {
    current_func = node;

    /* Visit children */
    for (auto type : node.types) {
        type->accept(this);
    }
    for (auto cse : node.cases) {
        cse->accept(this);
    }
    /* Visit stops here */
}

void TypeChecker::visit(Case &node) {
    type_stack.push(vector<Type>());

    /* Visit children */
    for (auto pattern : node.patterns) {
        pattern->accept(this);
    }
    node.expr->accept(this);
    /* Visit stops here */

    for (int i = 0; i < current_func->types.size(); ++i) {

    }

    type_stack.pop();
}

void TypeChecker::visit(Or &node) {
    /* Code goes here */


    /* Code stops here */

    /* Visit children */
    node.left->accept(this);
    node.right->accept(this);
    /* Visit stops here */

    /* Code goes here */


    /* Code stops here */
}

void TypeChecker::visit(And &node) {
    /* Code goes here */


    /* Code stops here */

    /* Visit children */
    node.left->accept(this);
    node.right->accept(this);
    /* Visit stops here */

    /* Code goes here */


    /* Code stops here */
}

void TypeChecker::visit(Equal &node) {
    /* Code goes here */


    /* Code stops here */

    /* Visit children */
    node.left->accept(this);
    node.right->accept(this);
    /* Visit stops here */

    /* Code goes here */


    /* Code stops here */
}

void TypeChecker::visit(NotEqual &node) {
    /* Code goes here */


    /* Code stops here */

    /* Visit children */
    node.left->accept(this);
    node.right->accept(this);
    /* Visit stops here */

    /* Code goes here */
    if(check_types(node.left->node_type, node.right->node_type)){
        node.node_type = new Type(BOOL);
    } else {
        throw "Bad programmer exception";
    }

    /* Code stops here */
}

void TypeChecker::visit(Lesser &node) {
    /* Code goes here */


    /* Code stops here */

    /* Visit children */
    node.left->accept(this);
    node.right->accept(this);
    /* Visit stops here */

    /* Code goes here */
    if(node.left->node_type->type == INT && node.right->node_type->type == INT ||
       node.left->node_type->type == FLOAT && node.right->node_type->type == FLOAT){
        node.node_type = new Type(BOOL);
    } else {
        throw "Bad programmer exception";
    }

    /* Code stops here */
}

void TypeChecker::visit(Greater &node) {
    /* Code goes here */


    /* Code stops here */

    /* Visit children */
    node.left->accept(this);
    node.right->accept(this);
    /* Visit stops here */

    /* Code goes here */
    if(node.left->node_type->type == INT && node.right->node_type->type == INT ||
       node.left->node_type->type == FLOAT && node.right->node_type->type == FLOAT){
        node.node_type = new Type(BOOL);
    } else {
        throw "Bad programmer exception";
    }
    /* Code stops here */
}

void TypeChecker::visit(LesserEq &node) {
    /* Code goes here */


    /* Code stops here */

    /* Visit children */
    node.left->accept(this);
    node.right->accept(this);
    /* Visit stops here */

    /* Code goes here */
    if(node.left->node_type->type == INT && node.right->node_type->type == INT ||
       node.left->node_type->type == FLOAT && node.right->node_type->type == FLOAT){
        node.node_type = new Type(BOOL);
    } else {
        throw "Bad programmer exception";
    }

    /* Code stops here */
}

void TypeChecker::visit(GreaterEq &node) {
    /* Code goes here */


    /* Code stops here */

    /* Visit children */
    node.left->accept(this);
    node.right->accept(this);
    /* Visit stops here */

    /* Code goes here */
    if(node.left->node_type->type == INT && node.right->node_type->type == INT ||
       node.left->node_type->type == FLOAT && node.right->node_type->type == FLOAT){
        node.node_type = new Type(BOOL);
    } else {
        throw "Bad programmer exception";
    }

    /* Code stops here */
}

void TypeChecker::visit(Add &node) {
    /* Code goes here */


    /* Code stops here */

    /* Visit children */
    node.left->accept(this);
    node.right->accept(this);
    /* Visit stops here */

    /* Code goes here */
    if(node.left->node_type->type == INT && node.right->node_type->type == INT ||
       node.left->node_type->type == FLOAT && node.right->node_type->type == FLOAT){
        node.node_type = node.left->node_type;
    } else {
        throw "Bad programmer exception";
    }

    /* Code stops here */
}

void TypeChecker::visit(Sub &node) {
    /* Code goes here */


    /* Code stops here */

    /* Visit children */
    node.left->accept(this);
    node.right->accept(this);
    /* Visit stops here */

    /* Code goes here */
    if(node.left->node_type->type == INT && node.right->node_type->type == INT ||
       node.left->node_type->type == FLOAT && node.right->node_type->type == FLOAT){
        node.node_type = node.left->node_type;
    } else {
        throw "Bad programmer exception";
    }

    /* Code stops here */
}

void TypeChecker::visit(Mul &node) {
    /* Code goes here */


    /* Code stops here */

    /* Visit children */
    node.left->accept(this);
    node.right->accept(this);
    /* Visit stops here */

    /* Code goes here */
    if(node.left->node_type->type == INT && node.right->node_type->type == INT ||
       node.left->node_type->type == FLOAT && node.right->node_type->type == FLOAT){
        node.node_type = node.left->node_type;
    } else {
        throw "Bad programmer exception";
    }

    /* Code stops here */
}

void TypeChecker::visit(Div &node) {
    /* Code goes here */


    /* Code stops here */

    /* Visit children */
    node.left->accept(this);
    node.right->accept(this);
    /* Visit stops here */

    /* Code goes here */
    if(node.left->node_type->type == INT && node.right->node_type->type == INT ||
       node.left->node_type->type == FLOAT && node.right->node_type->type == FLOAT){
        node.node_type = node.left->node_type;
    } else {
        throw "Bad programmer exception";
    }

    /* Code stops here */
}

void TypeChecker::visit(Mod &node) {
    /* Code goes here */


    /* Code stops here */

    /* Visit children */
    node.left->accept(this);
    node.right->accept(this);
    /* Visit stops here */

    /* Code goes here */
    if(node.left->node_type->type == INT && node.right->node_type->type == INT ||
       node.left->node_type->type == FLOAT && node.right->node_type->type == FLOAT){
        node.node_type = node.left->node_type;
    } else {
        throw "Bad programmer exception";
    }

    /* Code stops here */
}

void TypeChecker::visit(ListAdd &node) {
    /* Code goes here */


    /* Code stops here */

    /* Visit children */
    node.left->accept(this);
    node.right->accept(this);
    /* Visit stops here */

    /* Code goes here */
    if(node.right->node_type->type == LIST){
        if(check_types(node.left->node_type, node.right->node_type->types[0])){
            node.node_type = node.right->node_type;
        } else {
            throw "Not same type m8";
        }
    } else {
        throw "Not a list m8";
    }

    /* Code stops here */
}

void TypeChecker::visit(Par &node) {
    /* Code goes here */


    /* Code stops here */

    /* Visit children */
    node.child->accept(this);
    /* Visit stops here */

    /* Code goes here */
    node.node_type = node.child->node_type;
    /* Code stops here */
}

void TypeChecker::visit(Not &node) {
    /* Code goes here */


    /* Code stops here */

    /* Visit children */
    node.child->accept(this);
    /* Visit stops here */

    /* Code goes here */
    if(node.child->node_type->type == BOOL){
        node.node_type = node.child->node_type;
    } else {
        throw " YA DUN FUKD IT UP";
    }
    /* Code stops here */
}

void TypeChecker::visit(Int &node) {
    /* Code goes here */
    node -> node_type = new Type(INT);

    /* Code stops here */
}

void TypeChecker::visit(Float &node) {
    /* Code goes here */
    node -> node_type = new Type(FLOAT);

    /* Code stops here */
}

void TypeChecker::visit(Bool &node) {
    /* Code goes here */
    node -> node_type = new Type(BOOL);

    /* Code stops here */
}

void TypeChecker::visit(Char &node) {
    /* Code goes here */
    node -> node_type = new Type(CHAR);

    /* Code stops here */
}

void TypeChecker::visit(String &node) {
    /* Code goes here */
    node -> node_type = new Type(STRING);

    /* Code stops here */
}

void TypeChecker::visit(ListPattern &node) {
    /* Code goes here */


    /* Code stops here */

    /* Visit children */
    for (auto pattern : node.patterns) {
        pattern->accept(this);
    }
    /* Visit stops here */

    /* Code goes here */


    /* Code stops here */
}

void TypeChecker::visit(TuplePattern &node) {
    /* Code goes here */


    /* Code stops here */

    /* Visit children */
    for (auto pattern : node.patterns) {
        pattern->accept(this);
    }
    /* Visit stops here */

    /* Code goes here */


    /* Code stops here */
}

void TypeChecker::visit(ListSplit &node) {
    /* Code goes here */


    /* Code stops here */

    /* Visit children */
    node.left->accept(this);
    node.right->accept(this);
    /* Visit stops here */

    /* Code goes here */


    /* Code stops here */
}

void TypeChecker::visit(List &node) {
    /* Code goes here */


    /* Code stops here */

    /* Visit children */
    for (auto expr : node.exprs) {
        expr->accept(this);
    }
    /* Visit stops here */

    /* Code goes here */


    /* Code stops here */
}

void TypeChecker::visit(Tuple &node) {
    /* Code goes here */


    /* Code stops here */

    /* Visit children */
    for (auto expr : node.exprs) {
        expr->accept(this);
    }
    /* Visit stops here */

    /* Code goes here */


    /* Code stops here */
}


void TypeChecker::visit(Id &node) {
    /* Code goes here */
    node.node_type = get_type(node, node.scope);
    /* Code stops here */
}

Type* TypeChecker::get_type(Id &node, Scope *scope){
    auto got = scope->decls.find(node.id);
    if(got == scope->decls.end()){
        return get_type(node, scope->parent);
    } else {
        return got->second;
    }
}

void TypeChecker::visit(Call &node) {
    /* Code goes here */


    /* Code stops here */

    /* Visit children */
    node.callee->accept(this);
    for (auto expr : node.exprs) {
        expr->accept(this);
    }
    /* Visit stops here */

    /* Code goes here */


    /* Code stops here */
}

void TypeChecker::visit(Type &node) {
    /* Code goes here */


    /* Code stops here */

    /* Visit children */
    for (auto type : node.types) {
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
