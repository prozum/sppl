#include "Scope.h"
#include <iostream>
#include "ScopeGenerator.h"

namespace semantics {
    ScopeGenerator::ScopeGenerator(Scope* global)
            : current_scope(global) { }

    void ScopeGenerator::visit(Program &node) {
        // Visit children
        for (auto &func: node.funcs) {
            func->accept(*this);
        }
        // Visit stops here
    }

    void ScopeGenerator::visit(Function &node) {
        current_func = &node;
        node.scope = current_scope;

        if (!current_scope->exists(node.id) || node.is_anon) {
            current_scope->decls.insert({node.id, node.signature});

            // Visit children
            for (auto &cse: node.cases) {
                cse->accept(*this);
            }
            // Visit stops here
        } else {
            addError(Error(node.id + " has already been declared", node.loc));
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
            node.patterns[i]->type = current_func->signature[i];
            node.patterns[i]->accept(*this);
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
        if (node.type.id == TypeId::LIST) {
            for (int i = 0; i < node.patterns.size(); ++i) {
                node.patterns[i]->type = node.type.subtypes[i];
                node.patterns[i]->accept(*this);
            }
        }
    }

    void ScopeGenerator::visit(TuplePattern &node) {
        if (node.type.id == TypeId::TUPLE) {
            if (node.patterns.size() == node.type.subtypes.size()) {
                for (size_t i = 0; i < node.patterns.size(); i++) {
                    node.patterns[i]->type = node.type.subtypes[i];
                    node.patterns[i]->accept(*this);
                }
            }
        }
    }

    void ScopeGenerator::visit(ListSplit &node) {
        if (node.type.id == TypeId::LIST) {
            node.left->type = node.type.subtypes.front();
            node.right->type = node.type;
        } else if (node.type.id == TypeId::STRING){
            node.left->type = Type(TypeId::CHAR);
            node.right->type = node.type;
        }

        node.left->accept(*this);
        node.right->accept(*this);
    }

    void ScopeGenerator::visit(List &node) {
        // Visit children
        for (auto &expr: node.exprs) {
            expr->accept(*this);
        }
        // Visit stops here
    }

    void ScopeGenerator::visit(Tuple &node) {
        // Visit children
        for (auto &expr: node.exprs) {
            expr->accept(*this);
        }
        // Visit stops here
    }

    void ScopeGenerator::visit(Id &node) {
        node.scope = current_scope;

        if (context == ScopeContext::PATTERN) {
            if (!current_scope->exists(node.id)) {
                current_scope->decls.insert({node.id, node.type});
            }
        }
    }

    void ScopeGenerator::visit(Call &node) {
        // Visit children
        node.callee->accept(*this);
        for (auto &expr: node.exprs) {
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
