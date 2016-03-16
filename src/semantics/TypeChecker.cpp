//
// Created by hejsil on 3/4/16.
//

#include "TypeChecker.h"
#include <unordered_map>
#include "Scope.h"

using namespace std;

namespace semantics
{

void TypeChecker::visit(Program &node) {
    /* Visit all children */
    for (auto func : node.funcs) {
        func->accept(*this);
    }
    /* Visit stops here */
}

void TypeChecker::visit(Function &node) {
    current_func = &node;

    /* Visit children */
    for (auto type : node.types) {
        type->accept(*this);
    }
    for (auto cse : node.cases) {
        cse->accept(*this);
    }
    /* Visit stops here */
}

void TypeChecker::visit(Case &node) {
    /* Visit children */
    for (auto pattern : node.patterns) {
        pattern->accept(*this);
    }
    node.expr->accept(*this);
    /* Visit stops here */

    /* code starts here*/
    if (node.patterns.size() + 1 == current_func->types.size()) {
        for (size_t i = 0; i < node.patterns.size(); ++i) {
            if (node.patterns[i]->node_type->type == Types::EMPTYLIST) {
                node.patterns[i]->node_type = current_func->types[i];
            }

            if (!equal(node.patterns[i]->node_type, current_func->types[i])) {
                throw "fuck";
            }
        }
    } else {
        throw "Number of cases is wrong";
    }
}

void TypeChecker::visit(Or &node) {
    /* Code goes here */


    /* Code stops here */

    /* Visit children */
    node.left->accept(*this);
    node.right->accept(*this);
    /* Visit stops here */

    /* Code goes here */
    if (node.left->node_type->type == Types::BOOL && node.right->node_type->type == Types::BOOL) {
        node.node_type = node.left->node_type;
    } else {
        throw "Not good";
    }

    /* Code stops here */
}

void TypeChecker::visit(And &node) {
    /* Code goes here */


    /* Code stops here */

    /* Visit children */
    node.left->accept(*this);
    node.right->accept(*this);
    /* Visit stops here */

    /* Code goes here */
    if (node.left->node_type->type == Types::BOOL && node.right->node_type->type == Types::BOOL) {
        node.node_type = node.left->node_type;
    } else {
        throw "Not good";
    }

    /* Code stops here */
}

void TypeChecker::visit(Equal &node) {
    /* Code goes here */


    /* Code stops here */

    /* Visit children */
    node.left->accept(*this);
    node.right->accept(*this);
    /* Visit stops here */

    /* Code goes here */
    if (equal(node.left->node_type, node.right->node_type)) {
        node.node_type = new Type(Types::BOOL);
        garbage.push_back(node.node_type);
    } else {
        throw "equal operator need to compare same types";
    }
    /* Code stops here */
}

void TypeChecker::visit(NotEqual &node) {
    /* Code goes here */


    /* Code stops here */

    /* Visit children */
    node.left->accept(*this);
    node.right->accept(*this);
    /* Visit stops here */

    /* Code goes here */
    if (equal(node.left->node_type, node.right->node_type)) {
        node.node_type = new Type(Types::BOOL, vector<Type *>(), 0);
        garbage.push_back(node.node_type);
    } else {
        throw "not equal operator need to compare same types";
    }

    /* Code stops here */
}

void TypeChecker::visit(Lesser &node) {
    /* Code goes here */


    /* Code stops here */

    /* Visit children */
    node.left->accept(*this);
    node.right->accept(*this);
    /* Visit stops here */

    /* Code goes here */
    if ((node.left->node_type->type == Types::INT && node.right->node_type->type == Types::INT) ||
        (node.left->node_type->type == Types::FLOAT && node.right->node_type->type == Types::FLOAT)) {
        node.node_type = new Type(Types::BOOL);
        garbage.push_back(node.node_type);
    } else {
        throw "lesser operator only compares int, float";
    }

    /* Code stops here */
}

void TypeChecker::visit(Greater &node) {
    /* Code goes here */


    /* Code stops here */

    /* Visit children */
    node.left->accept(*this);
    node.right->accept(*this);
    /* Visit stops here */

    /* Code goes here */
    if ((node.left->node_type->type == Types::INT && node.right->node_type->type == Types::INT) ||
        (node.left->node_type->type == Types::FLOAT && node.right->node_type->type == Types::FLOAT)) {
        node.node_type = new Type(Types::BOOL);
        garbage.push_back(node.node_type);
    } else {
        throw "greater operator only compares int, float";
    }
    /* Code stops here */
}

void TypeChecker::visit(LesserEq &node) {
    /* Code goes here */


    /* Code stops here */

    /* Visit children */
    node.left->accept(*this);
    node.right->accept(*this);
    /* Visit stops here */

    /* Code goes here */
    if ((node.left->node_type->type == Types::INT && node.right->node_type->type == Types::INT) ||
        (node.left->node_type->type == Types::FLOAT && node.right->node_type->type == Types::FLOAT)) {
        node.node_type = new Type(Types::BOOL);
        garbage.push_back(node.node_type);
    } else {
        throw "lesser equal only compares int, float";
    }

    /* Code stops here */
}

void TypeChecker::visit(GreaterEq &node) {
    /* Code goes here */


    /* Code stops here */

    /* Visit children */
    node.left->accept(*this);
    node.right->accept(*this);
    /* Visit stops here */

    /* Code goes here */
    if ((node.left->node_type->type == Types::INT && node.right->node_type->type == Types::INT) ||
        (node.left->node_type->type == Types::FLOAT && node.right->node_type->type == Types::FLOAT)) {
        node.node_type = new Type(Types::BOOL);
        garbage.push_back(node.node_type);
    } else {
        throw "greater equal only compares int, float";
    }

    /* Code stops here */
}

void TypeChecker::visit(Add &node) {
    /* Code goes here */


    /* Code stops here */

    /* Visit children */
    node.left->accept(*this);
    node.right->accept(*this);
    /* Visit stops here */

    /* Code goes here */
    if ((node.left->node_type->type == Types::INT && node.right->node_type->type == Types::INT) ||
        (node.left->node_type->type == Types::FLOAT && node.right->node_type->type == Types::FLOAT)) {
        node.node_type = node.left->node_type;
    } else {
        throw "can only add int to int and float to float";
    }

    /* Code stops here */
}

void TypeChecker::visit(Sub &node) {
    /* Code goes here */


    /* Code stops here */

    /* Visit children */
    node.left->accept(*this);
    node.right->accept(*this);
    /* Visit stops here */

    /* Code goes here */
    if ((node.left->node_type->type == Types::INT && node.right->node_type->type == Types::INT) ||
        (node.left->node_type->type == Types::FLOAT && node.right->node_type->type == Types::FLOAT)) {
        node.node_type = node.left->node_type;
    } else {
        throw "can only sub int from int and float from float";
    }

    /* Code stops here */
}

void TypeChecker::visit(Mul &node) {
    /* Code goes here */


    /* Code stops here */

    /* Visit children */
    node.left->accept(*this);
    node.right->accept(*this);
    /* Visit stops here */

    /* Code goes here */
    if ((node.left->node_type->type == Types::INT && node.right->node_type->type == Types::INT) ||
        (node.left->node_type->type == Types::FLOAT && node.right->node_type->type == Types::FLOAT)) {
        node.node_type = node.left->node_type;
    } else {
        throw "can only multiply int with int and float with float";
    }

    /* Code stops here */
}

void TypeChecker::visit(Div &node) {
    /* Code goes here */


    /* Code stops here */

    /* Visit children */
    node.left->accept(*this);
    node.right->accept(*this);
    /* Visit stops here */

    /* Code goes here */
    if ((node.left->node_type->type == Types::INT && node.right->node_type->type == Types::INT) ||
        (node.left->node_type->type == Types::FLOAT && node.right->node_type->type == Types::FLOAT)) {
        node.node_type = node.left->node_type;
    } else {
        throw "can only divide int with int and float with float";
    }

    /* Code stops here */
}

void TypeChecker::visit(Mod &node) {
    /* Code goes here */


    /* Code stops here */

    /* Visit children */
    node.left->accept(*this);
    node.right->accept(*this);
    /* Visit stops here */

    /* Code goes here */
    if ((node.left->node_type->type == Types::INT && node.right->node_type->type == Types::INT) ||
        (node.left->node_type->type == Types::FLOAT && node.right->node_type->type == Types::FLOAT)) {
        node.node_type = node.left->node_type;
    } else {
        throw "can only modulo int with int and float with float";
    }

    /* Code stops here */
}

void TypeChecker::visit(ListAdd &node) {
    /* Code goes here */


    /* Code stops here */

    /* Visit children */
    node.left->accept(*this);
    node.right->accept(*this);
    /* Visit stops here */

    /* Code goes here */
    if (node.right->node_type->type == Types::LIST) {
        if (equal(node.left->node_type, node.right->node_type->types.front())) {
            node.node_type = node.right->node_type;
        } else {
            throw "not same types in list";
        }
    } else if (node.right->node_type->type == Types::EMPTYLIST) {
        node.node_type = new Type(Types::LIST);
        node.node_type->types.push_back(node.left->node_type);
        node.right->node_type = node.node_type;
        garbage.push_back(node.node_type);
    } else {
        throw "making list from non list";
    }

    /* Code stops here */
}

void TypeChecker::visit(Par &node) {
    /* Code goes here */


    /* Code stops here */

    /* Visit children */
    node.child->accept(*this);
    /* Visit stops here */

    /* Code goes here */
    node.node_type = node.child->node_type;
    /* Code stops here */
}

void TypeChecker::visit(Not &node) {
    /* Code goes here */


    /* Code stops here */

    /* Visit children */
    node.child->accept(*this);
    /* Visit stops here */

    /* Code goes here */
    if (node.child->node_type->type == Types::BOOL) {
        node.node_type = node.child->node_type;
    } else {
        throw "can only use not on bools";
    }
    /* Code stops here */
}

void TypeChecker::visit(Int &node) {
    /* Code goes here */
    node.node_type = new Type(Types::INT);
    garbage.push_back(node.node_type);

    /* Code stops here */
}

void TypeChecker::visit(Float &node) {
    /* Code goes here */
    node.node_type = new Type(Types::FLOAT);
    garbage.push_back(node.node_type);

    /* Code stops here */
}

void TypeChecker::visit(Bool &node) {
    /* Code goes here */
    node.node_type = new Type(Types::BOOL);
    garbage.push_back(node.node_type);

    /* Code stops here */
}

void TypeChecker::visit(Char &node) {
    /* Code goes here */
    // TODO check char length
    node.node_type = new Type(Types::CHAR);
    garbage.push_back(node.node_type);

    /* Code stops here */
}

void TypeChecker::visit(String &node) {
    /* Code goes here */

    node.node_type = new Type(Types::STRING);
    garbage.push_back(node.node_type);

    /* Code stops here */
}

void TypeChecker::visit(ListPattern &node) {
    /* Code goes here */


    /* Code stops here */

    /* Visit children */
    for (auto pattern : node.patterns) {
        pattern->accept(*this);
    }
    /* Visit stops here */

    /* Code goes here */
    if (node.patterns.size() == 0) {
        node.node_type = new Type(Types::EMPTYLIST);
        garbage.push_back(node.node_type);
    } else {
        for (size_t i = 0; i < node.patterns.size() - 1; ++i) {
            if (!equal(node.patterns[i]->node_type, node.patterns[i + 1]->node_type)) {
                throw "list pattern types not equal";
            }
        }

        node.node_type = new Type(Types::LIST);
        node.node_type->types.push_back(node.patterns[0]->node_type);
        garbage.push_back(node.node_type);
    }
    /* Code stops here */
}

void TypeChecker::visit(TuplePattern &node) {
    /* Code goes here */


    /* Code stops here */

    /* Visit children */
    for (auto pattern : node.patterns) {
        pattern->accept(*this);
    }
    /* Visit stops here */

    /* Code goes here */
    node.node_type = new Type(Types::TUPLE);
    garbage.push_back(node.node_type);

    for (auto pattern : node.patterns) {
        node.node_type->types.push_back(pattern->node_type);
    }
    /* Code stops here */
}

void TypeChecker::visit(ListSplit &node) {
    /* Code goes here */


    /* Code stops here */

    /* Visit children */
    node.left->accept(*this);
    node.right->accept(*this);
    /* Visit stops here */

    /* Code goes here */
    if (node.right->node_type->type == Types::LIST) {
        if (equal(node.left->node_type, node.right->node_type->types.front())) {
            node.node_type = node.right->node_type;
        } else {
            throw "Not same types in list split";
        }
    } else if (node.right->node_type->type == Types::STRING) {
        node.node_type = node.right->node_type;
    } else if (node.right->node_type->type == Types::EMPTYLIST) {
        node.node_type = new Type(Types::LIST);
        node.node_type->types.push_back(node.left->node_type);
        garbage.push_back(node.node_type);
    } else {
        throw "not a list in listsplit";
    }
    /* Code stops here */
}

void TypeChecker::visit(List &node) {
    /* Code goes here */


    /* Code stops here */

    /* Visit children */
    for (auto expr : node.exprs) {
        expr->accept(*this);
    }
    /* Visit stops here */

    /* Code goes here */
    if (node.exprs.size() == 0) {
        node.node_type = new Type(Types::EMPTYLIST);
        garbage.push_back(node.node_type);
    } else {
        for (size_t i = 0; i < node.exprs.size() - 1; ++i) {
            if (!equal(node.exprs[i]->node_type, node.exprs[i + 1]->node_type)) {
                throw "wrong type when creating list";
            }
        }
        node.node_type = new Type(Types::LIST);
        node.node_type->types.push_back(node.exprs[0]->node_type);
        garbage.push_back(node.node_type);
    }

    /* Code stops here */
}

void TypeChecker::visit(Tuple &node) {
    /* Code goes here */


    /* Code stops here */

    /* Visit children */
    for (auto expr : node.exprs) {
        expr->accept(*this);
    }
    /* Visit stops here */

    /* Code goes here */
    node.node_type = new Type(Types::TUPLE);
    garbage.push_back(node.node_type);

    for (size_t i = 0; i < node.exprs.size(); ++i) {
        node.node_type->types.push_back(node.exprs[i]->node_type);
    }
    /* Code stops here */
}


void TypeChecker::visit(Id &node) {
    /* Code goes here */
    node.node_type = node.scope->get_type(node.id);
    /* Code stops here */
}

void TypeChecker::visit(Call &node) {
    /* Code goes here */


    /* Code stops here */

    /* Visit children */
    node.callee->accept(*this);
    for (auto expr : node.exprs) {
        expr->accept(*this);
    }
    /* Visit stops here */

    /* Code goes here */
    if (node.callee->node_type->type == Types::SIGNATURE) {
        if (node.exprs.size() + 1 == node.callee->node_type->types.size()) {
            for (size_t i = 0; i < node.exprs.size(); ++i) {
                if (!equal(node.exprs[i]->node_type, node.callee->node_type->types[i])) {
                    throw "wrong types in call";
                }
            }
        } else {
            throw "wrong size in call";
        }
        node.node_type = node.callee->node_type->types.back();
    } else {
        throw "wrong signature in call";
    }

    /* Code stops here */
}

void TypeChecker::visit(Type &node) {
    /* Code goes here */


    /* Code stops here */

    /* Visit children */
    for (auto type : node.types) {
        type->accept(*this);
    }
    /* Visit stops here */

    /* Code goes here */
    node.node_type = &node;

    /* Code stops here */
}

bool TypeChecker::equal(Type *type1, Type *type2) {
    return *type1 == *type2;
}

}
