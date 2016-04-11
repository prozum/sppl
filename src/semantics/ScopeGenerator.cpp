#include "Scope.h"
#include <iostream>
#include "ScopeGenerator.h"

namespace semantics {
    ScopeGenerator::ScopeGenerator(Scope* global)
            : current_scope(global) { }

    void ScopeGenerator::visit(Program &node) {
        // Visit children
        for (auto func : node.funcs) {
            func->accept(*this);
        }
        // Visit stops here
    }

    void ScopeGenerator::visit(Function &node) {
        current_func = &node;
        node.scope = current_scope;

        if (!current_scope->exists(node.id)) {
            auto type = make_shared<Type>(Types::SIGNATURE);

            for (auto i : node.types)
                type->types.push_back(i);

            current_scope->decls.insert({node.id, type});

            // Visit children
            for (auto type : node.types) {
                type->accept(*this);
            }
            for (auto cse : node.cases) {
                cse->accept(*this);
            }
            // Visit stops here
        } else {
            AddError(Error(node.id + " has already been declared", node.loc));
            return;
        }
    }


    void ScopeGenerator::visit(Case &node) {
        auto case_scope = new Scope(current_scope);
        current_scope->children.push_back(unique_ptr<Scope>(case_scope));
        current_scope = case_scope;

        // Visit children
        context = ScopeContext::PATTERN;

        for (size_t i = 0; i < node.patterns.size(); i++) {
            type_stack.push(current_func->types[i]);
            node.patterns[i]->accept(*this);
            type_stack.pop();
        }

        context = ScopeContext::EXPR;
        node.expr->accept(*this);

        current_scope = current_scope->parent;
    }

    void ScopeGenerator::visit(Or &node) {
        // Visit children
        node.left->accept(*this);
        node.right->accept(*this);
        // Visit stops here
    }

    void ScopeGenerator::visit(And &node) {
        // Visit children
        node.left->accept(*this);
        node.right->accept(*this);
        // Visit stops here
    }

    void ScopeGenerator::visit(Equal &node) {
        // Visit children
        node.left->accept(*this);
        node.right->accept(*this);
        // Visit stops here
    }

    void ScopeGenerator::visit(NotEqual &node) {
        // Visit children
        node.left->accept(*this);
        node.right->accept(*this);
        // Visit stops here
    }

    void ScopeGenerator::visit(Lesser &node) {
        // Visit children
        node.left->accept(*this);
        node.right->accept(*this);
        // Visit stops here
    }

    void ScopeGenerator::visit(Greater &node) {
        // Visit children
        node.left->accept(*this);
        node.right->accept(*this);
        // Visit stops here
    }

    void ScopeGenerator::visit(LesserEq &node) {
        // Visit children
        node.left->accept(*this);
        node.right->accept(*this);
        // Visit stops here
    }

    void ScopeGenerator::visit(GreaterEq &node) {
        // Visit children
        node.left->accept(*this);
        node.right->accept(*this);
        // Visit stops here
    }

    void ScopeGenerator::visit(Add &node) {
        // Visit children
        node.left->accept(*this);
        node.right->accept(*this);
        // Visit stops here
    }

    void ScopeGenerator::visit(Sub &node) {
        // Visit children
        node.left->accept(*this);
        node.right->accept(*this);
        // Visit stops here
    }

    void ScopeGenerator::visit(Mul &node) {
        // Visit children
        node.left->accept(*this);
        node.right->accept(*this);
        // Visit stops here
    }

    void ScopeGenerator::visit(Div &node) {
        // Visit children
        node.left->accept(*this);
        node.right->accept(*this);
        // Visit stops here
    }

    void ScopeGenerator::visit(Mod &node) {
        // Visit children
        node.left->accept(*this);
        node.right->accept(*this);
        // Visit stops here
    }

    void ScopeGenerator::visit(ListAdd &node) {
        // Visit children
        node.left->accept(*this);
        node.right->accept(*this);
        // Visit stops here
    }

    void ScopeGenerator::visit(Par &node) {
        // Visit children
        node.child->accept(*this);
        // Visit stops here
    }

    void ScopeGenerator::visit(Not &node) {
        // Visit children
        node.child->accept(*this);
        // Visit stops here
    }

    void ScopeGenerator::visit(ListPattern &node) {
        if (type_stack.top()->type == Types::LIST) {
            type_stack.push(type_stack.top()->types[0]);

            // Visit children
            for (auto pattern : node.patterns) {
                pattern->accept(*this);
            }
            // Visit stops here

            type_stack.pop();
        }
    }

    void ScopeGenerator::visit(TuplePattern &node) {
        if (type_stack.top()->type == Types::TUPLE) {
            if (node.patterns.size() == type_stack.top()->types.size()) {
                // Visit children
                for (size_t i = 0; i < node.patterns.size(); i++) {
                    type_stack.push(type_stack.top()->types[i]);
                    node.patterns[i]->accept(*this);
                    type_stack.pop();
                }
            }
        }
    }

    void ScopeGenerator::visit(ListSplit &node) {
        if (type_stack.top()->type == Types::LIST) {
            type_stack.push(type_stack.top()->types[0]);

            // Visit children
            node.left->accept(*this);
            type_stack.pop();
            node.right->accept(*this);
            // Visit stops here
        } else if (type_stack.top()->type == Types::STRING){
            auto _char = make_shared<Type>(Types::CHAR);
            type_stack.push(_char);

            // Visit children
            node.left->accept(*this);
            type_stack.pop();
            node.right->accept(*this);
            // Visit stops here
        }
    }

    void ScopeGenerator::visit(List &node) {
        // Visit children
        for (auto expr : node.exprs) {
            expr->accept(*this);
        }
        // Visit stops here
    }

    void ScopeGenerator::visit(Tuple &node) {
        // Visit children
        for (auto expr : node.exprs) {
            expr->accept(*this);
        }
        // Visit stops here
    }

    void ScopeGenerator::visit(Id &node) {
        node.scope = current_scope;

        if (context == ScopeContext::PATTERN) {
            if (!current_scope->exists(node.id)) {
                current_scope->decls.insert({node.id, type_stack.top()});
            }
        }
    }

    void ScopeGenerator::visit(Call &node) {
        // Visit children
        node.callee->accept(*this);
        for (auto expr : node.exprs) {
            expr->accept(*this);
        }
        // Visit stops here
    }

    void ScopeGenerator::visit(Type &node) { }

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
}
