//
// Created by hejsil on 3/4/16.
//

#define PRINT(a) std::cout << a << std::endl;

#include "Scope.h"
#include <iostream>
#include "ScopeGenerator.h"

void ScopeGenerator::visit(Program *node) {
    PRINT("Program")
    res = new Scope();
    current_scope = res;

    /* Visit all children */
    for (auto func : node->funcs) {
        func->accept(this);
    }
    /* Visit stops here */
}

void ScopeGenerator::visit(Function *node) {
    PRINT("Function")
    current_func = node;
    current_scope->decls.insert({node->id, node->types.back()});

    /* Visit children */
    for (auto type : node->types) {
        type->accept(this);
    }
    for (auto cse : node->cases) {
        cse->accept(this);
    }
    /* Visit stops here */
}

void ScopeGenerator::visit(Case *node) {
    PRINT("Case")
    auto case_scope = new Scope(current_scope);
    current_scope->children.push_back(case_scope);
    current_scope = case_scope;

    if (node->patterns.size() == current_func->types.size() - 1) {

        /* Visit children */
        for (int i = 0; i < node->patterns.size(); i++){
            type_stack.push(current_func->types[i]);
            node->patterns[i]->accept(this);
            type_stack.pop();
        }

        /*
        node->expr->accept(this);
        */
    } else {
        /* ERROR! Cases doesn't have the currect number of patterns */
        throw "Case doesn't have the currect number of patterns";
    }

    current_scope = current_scope->parent;
}

void ScopeGenerator::visit(Or *node) {
    PRINT("Or")
    /* Visit children */
    node->left->accept(this);
    node->right->accept(this);
    /* Visit stops here */
}

void ScopeGenerator::visit(And *node) {
    PRINT("And")
    /* Visit children */
    node->left->accept(this);
    node->right->accept(this);
    /* Visit stops here */
}

void ScopeGenerator::visit(Equal *node) {
    PRINT("Equal")
    /* Visit children */
    node->left->accept(this);
    node->right->accept(this);
    /* Visit stops here */
}

void ScopeGenerator::visit(NotEqual *node) {
    PRINT("NotEqual")
    /* Visit children */
    node->left->accept(this);
    node->right->accept(this);
    /* Visit stops here */
}

void ScopeGenerator::visit(Lesser *node) {
    PRINT("Lesser")
    /* Visit children */
    node->left->accept(this);
    node->right->accept(this);
    /* Visit stops here */
}

void ScopeGenerator::visit(Greater *node) {
    PRINT("Greater")
    /* Visit children */
    node->left->accept(this);
    node->right->accept(this);
    /* Visit stops here */
}

void ScopeGenerator::visit(LesserEq *node) {
    PRINT("LesserEq")
    /* Visit children */
    node->left->accept(this);
    node->right->accept(this);
    /* Visit stops here */
}

void ScopeGenerator::visit(GreaterEq *node) {
    PRINT("GreaterEq")
    /* Visit children */
    node->left->accept(this);
    node->right->accept(this);
    /* Visit stops here */
}

void ScopeGenerator::visit(Add *node) {
    PRINT("Add")
    /* Visit children */
    node->left->accept(this);
    node->right->accept(this);
    /* Visit stops here */
}

void ScopeGenerator::visit(Sub *node) {
    PRINT("Sub")
    /* Visit children */
    node->left->accept(this);
    node->right->accept(this);
    /* Visit stops here */
}

void ScopeGenerator::visit(Mul *node) {
    PRINT("Mul")
    /* Visit children */
    node->left->accept(this);
    node->right->accept(this);
    /* Visit stops here */
}

void ScopeGenerator::visit(Div *node) {
    PRINT("Div")
    /* Visit children */
    node->left->accept(this);
    node->right->accept(this);
    /* Visit stops here */
}

void ScopeGenerator::visit(Mod *node) {
    PRINT("Mod")
    /* Visit children */
    node->left->accept(this);
    node->right->accept(this);
    /* Visit stops here */
}

void ScopeGenerator::visit(ListAdd *node) {
    PRINT("ListAdd")
    /* Visit children */
    node->left->accept(this);
    node->right->accept(this);
    /* Visit stops here */
}

void ScopeGenerator::visit(Par *node) {
    PRINT("Par")
    /* Visit children */
    node->child->accept(this);
    /* Visit stops here */
}

void ScopeGenerator::visit(Not *node) {
    PRINT("Not")
    /* Visit children */
    node->child->accept(this);
    /* Visit stops here */
}

void ScopeGenerator::visit(Int *node) {
    PRINT("Int")

}

void ScopeGenerator::visit(Float *node) {
    PRINT("Float")

}

void ScopeGenerator::visit(Bool *node) {
    PRINT("Bool")

}

void ScopeGenerator::visit(Char *node) {
    PRINT("Char")

}

void ScopeGenerator::visit(String *node) {
    PRINT("String")
}

void ScopeGenerator::visit(ListPattern *node) {
    PRINT("ListPattern")

    if (type_stack.top()->type == TUPLE) {
        type_stack.push(type_stack.top()->types[0]);

        /* Visit children */
        for (auto pattern : node->patterns) {
            pattern->accept(this);
        }
        /* Visit stops here */

        type_stack.pop();
    } else {
        /* ERROR! Cases doesn't have the currect number of patterns */
        throw "TuplePattern doesn't have the currect number of patterns";
    }

}

void ScopeGenerator::visit(TuplePattern *node) {
    PRINT("TuplePattern")
    if (node->patterns.size() == type_stack.top()->types.size() && type_stack.top()->type == TUPLE) {

        /* Visit children */
        for (int i = 0; i < node->patterns.size(); i++){
            type_stack.push(type_stack.top()->types[i]);
            node->patterns[i]->accept(this);
            type_stack.pop();
        }

    } else {
        /* ERROR! Cases doesn't have the currect number of patterns */
        throw "TuplePattern doesn't have the currect number of patterns";
    }
}

void ScopeGenerator::visit(ListSplit *node) {
    PRINT("ListSplit")
    type_stack.push(type_stack.top()->types[0]);

    /* Visit children */
    node->left->accept(this);
    type_stack.pop();
    node->right->accept(this);
    /* Visit stops here */
}

void ScopeGenerator::visit(List *node) {
    PRINT("List")
    /* Visit children */
    for (auto expr : node->exprs) {
        expr->accept(this);
    }
    /* Visit stops here */
}

void ScopeGenerator::visit(Tuple *node) {
    PRINT("Tuple")
    /* Visit children */
    for (auto expr : node->exprs) {
        expr->accept(this);
    }
    /* Visit stops here */
}

void ScopeGenerator::visit(Id *node) {
    PRINT("Id")
    std::cout << "1" << std::endl;
    node->scope = current_scope;
    std::cout << "2" << std::endl;
    current_scope->decls.insert({node->id, type_stack.top()});
    std::cout << "3" << std::endl;
}

void ScopeGenerator::visit(Call *node) {
    PRINT("Call")
    /* Visit children */
    node->callee->accept(this);
    for (auto expr : node->exprs) {
        expr->accept(this);
    }
    /* Visit stops here */
}

void ScopeGenerator::visit(Type *node) {
    PRINT("LiteralType")

}