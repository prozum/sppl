//
// Created by hejsil on 3/4/16.
//

#include "Scope.h"
#include <iostream>
#include "ScopeGenerator.h"

void ScopeGenerator::visit(Program &node) {
    res = new Scope();
    current_scope = res;
    garbage.push_back(current_scope);

    /* Visit all children */
    for (auto func : node.funcs) {
        func->accept(*this);
    }
    /* Visit stops here */
}

void ScopeGenerator::visit(Function &node) {
    current_func = &node;

    if (!exists_in_scope(node.id)) {
        current_scope->decls.insert({node.id, new Type(Types::SIGNATURE, &node.types)});

        /* Visit children */
        for (auto type : node.types) {
            type->accept(*this);
        }
        for (auto cse : node.cases) {
            cse->accept(*this);
        }
        /* Visit stops here */
    } else {
        /* ERROR! Id exists in scope */
        throw "Id exists in scope";
    }
}

void ScopeGenerator::visit(Case &node) {
    auto case_scope = new Scope(current_scope);
    current_scope->children.push_back(case_scope);
    current_scope = case_scope;
    garbage.push_back(current_scope);

    if (node.patterns.size() == current_func->types.size() - 1) {

        /* Visit children */
        context = ScopeContext::PATTERN;

        for (int i = 0; i < node.patterns.size(); i++){
            type_stack.push(current_func->types[i]);
            node.patterns[i]->accept(*this);
            type_stack.pop();
        }

        context = ScopeContext::EXPR;
        node.expr->accept(*this);
    } else {
        /* ERROR! Cases doesn't have the currect number of patterns */
        throw "Case doesn't have the currect number of patterns";
    }

    current_scope = current_scope->parent;
}

void ScopeGenerator::visit(Or &node) {
    /* Visit children */
    node.left->accept(*this);
    node.right->accept(*this);
    /* Visit stops here */
}

void ScopeGenerator::visit(And &node) {
    /* Visit children */
    node.left->accept(*this);
    node.right->accept(*this);
    /* Visit stops here */
}

void ScopeGenerator::visit(Equal &node) {
    /* Visit children */
    node.left->accept(*this);
    node.right->accept(*this);
    /* Visit stops here */
}

void ScopeGenerator::visit(NotEqual &node) {
    /* Visit children */
    node.left->accept(*this);
    node.right->accept(*this);
    /* Visit stops here */
}

void ScopeGenerator::visit(Lesser &node) {
    /* Visit children */
    node.left->accept(*this);
    node.right->accept(*this);
    /* Visit stops here */
}

void ScopeGenerator::visit(Greater &node) {
    /* Visit children */
    node.left->accept(*this);
    node.right->accept(*this);
    /* Visit stops here */
}

void ScopeGenerator::visit(LesserEq &node) {
    /* Visit children */
    node.left->accept(*this);
    node.right->accept(*this);
    /* Visit stops here */
}

void ScopeGenerator::visit(GreaterEq &node) {
    /* Visit children */
    node.left->accept(*this);
    node.right->accept(*this);
    /* Visit stops here */
}

void ScopeGenerator::visit(Add &node) {
    /* Visit children */
    node.left->accept(*this);
    node.right->accept(*this);
    /* Visit stops here */
}

void ScopeGenerator::visit(Sub &node) {
    /* Visit children */
    node.left->accept(*this);
    node.right->accept(*this);
    /* Visit stops here */
}

void ScopeGenerator::visit(Mul &node) {
    /* Visit children */
    node.left->accept(*this);
    node.right->accept(*this);
    /* Visit stops here */
}

void ScopeGenerator::visit(Div &node) {
    /* Visit children */
    node.left->accept(*this);
    node.right->accept(*this);
    /* Visit stops here */
}

void ScopeGenerator::visit(Mod &node) {
    /* Visit children */
    node.left->accept(*this);
    node.right->accept(*this);
    /* Visit stops here */
}

void ScopeGenerator::visit(ListAdd &node) {
    /* Visit children */
    node.left->accept(*this);
    node.right->accept(*this);
    /* Visit stops here */
}

void ScopeGenerator::visit(Par &node) {
    /* Visit children */
    node.child->accept(*this);
    /* Visit stops here */
}

void ScopeGenerator::visit(Not &node) {
    /* Visit children */
    node.child->accept(*this);
    /* Visit stops here */
}

void ScopeGenerator::visit(Int &node) {

}

void ScopeGenerator::visit(Float &node) {

}

void ScopeGenerator::visit(Bool &node) {

}

void ScopeGenerator::visit(Char &node) {

}

void ScopeGenerator::visit(String &node) {

}

void ScopeGenerator::visit(ListPattern &node) {
    if (type_stack.top()->type == Types::LIST) {
        type_stack.push(type_stack.top()->types[0]);

        /* Visit children */
        for (auto pattern : node.patterns) {
            pattern->accept(*this);
        }
        /* Visit stops here */

        type_stack.pop();
    } else {
        /* ERROR! Expected list type, but was something else */
        throw "Expected list type, but was something else";
    }

}

void ScopeGenerator::visit(TuplePattern &node) {
    if (node.patterns.size() == type_stack.top()->types.size() && type_stack.top()->type == Types::TUPLE) {

        /* Visit children */
        for (int i = 0; i < node.patterns.size(); i++){
            type_stack.push(type_stack.top()->types[i]);
            node.patterns[i]->accept(*this);
            type_stack.pop();
        }

    } else {
        /* ERROR! TuplePattern doesn't have the currect number of patterns */
        throw "TuplePattern doesn't have the currect number of patterns";
    }
}

void ScopeGenerator::visit(ListSplit &node) {
    if (type_stack.top()->type == Types::LIST) {
        type_stack.push(type_stack.top()->types[0]);

        /* Visit children */
        node.left->accept(*this);
        type_stack.pop();
        node.right->accept(*this);
        /* Visit stops here */
    } else {
        /* ERROR! Expected list type, but was something else */
        throw "Expected list type, but was something else";
    }
}

void ScopeGenerator::visit(List &node) {
    /* Visit children */
    for (auto expr : node.exprs) {
        expr->accept(*this);
    }
    /* Visit stops here */
}

void ScopeGenerator::visit(Tuple &node) {
    /* Visit children */
    for (auto expr : node.exprs) {
        expr->accept(*this);
    }
    /* Visit stops here */
}

void ScopeGenerator::visit(Id &node) {
    node.scope = current_scope;

    if (context == ScopeContext::PATTERN) {
        if (!exists_in_scope(node.id)) {
            current_scope->decls.insert({node.id, type_stack.top()});
        } else {
            /* ERROR! Id exists in scope */
            throw "Id exists in scope";
        }
    }
}

void ScopeGenerator::visit(Call &node) {
    /* Visit children */
    node.callee->accept(*this);
    for (auto expr : node.exprs) {
        expr->accept(*this);
    }
    /* Visit stops here */
}

void ScopeGenerator::visit(Type &node) {

}

bool ScopeGenerator::exists_in_scope(std::string id) {
    Scope *scope = current_scope;

    for (;;) {
        auto got = scope->decls.find(id);

        if (got != scope->decls.end())
            return true;

        if (scope->parent)
            scope = scope->parent;
        else
            break;
    }

    return false;
}

