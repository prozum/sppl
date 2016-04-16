#include "Scope.h"
#include <iostream>
#include "ScopeGenerator.h"

namespace semantics {
    ScopeGenerator::ScopeGenerator(Scope* global)
            : current_scope(global) { }

    void ScopeGenerator::visit(Program &node) {
        // Visit children
        for (auto &func: node.Funcs) {
            func->accept(*this);
        }
        // Visit stops here
    }

    void ScopeGenerator::visit(Function &node) {
        current_func = &node;
        node.Scp = current_scope;

        if (!current_scope->exists(node.Id) || node.Anon) {
            current_scope->Decls.insert({node.Id, node.Signature});

            // Visit children
            for (auto &cse: node.Cases) {
                cse->accept(*this);
            }
            // Visit stops here
        } else {
            throw Error(node.Id + " has already been declared", node.Loc);
        }
    }


    void ScopeGenerator::visit(Case &node) {
        auto case_scope = new Scope(current_scope);
        current_scope->Children.push_back(unique_ptr<Scope>(case_scope));
        current_scope = case_scope;

        // Visit children
        context = ScopeContext::PATTERN;

        for (size_t i = 0; i < node.Patterns.size(); i++) {
            node.Patterns[i]->Ty = current_func->Signature[i];
            node.Patterns[i]->accept(*this);
        }

        context = ScopeContext::EXPR;
        node.Expr->accept(*this);

        current_scope = current_scope->Parent;
    }

    void ScopeGenerator::visit(Or &node) {
        // Visit children
        node.Left->accept(*this);
        node.Right->accept(*this);
        // Visit stops here
    }

    void ScopeGenerator::visit(And &node) {
        // Visit children
        node.Left->accept(*this);
        node.Right->accept(*this);
        // Visit stops here
    }

    void ScopeGenerator::visit(Equal &node) {
        // Visit children
        node.Left->accept(*this);
        node.Right->accept(*this);
        // Visit stops here
    }

    void ScopeGenerator::visit(NotEqual &node) {
        // Visit children
        node.Left->accept(*this);
        node.Right->accept(*this);
        // Visit stops here
    }

    void ScopeGenerator::visit(Lesser &node) {
        // Visit children
        node.Left->accept(*this);
        node.Right->accept(*this);
        // Visit stops here
    }

    void ScopeGenerator::visit(Greater &node) {
        // Visit children
        node.Left->accept(*this);
        node.Right->accept(*this);
        // Visit stops here
    }

    void ScopeGenerator::visit(LesserEq &node) {
        // Visit children
        node.Left->accept(*this);
        node.Right->accept(*this);
        // Visit stops here
    }

    void ScopeGenerator::visit(GreaterEq &node) {
        // Visit children
        node.Left->accept(*this);
        node.Right->accept(*this);
        // Visit stops here
    }

    void ScopeGenerator::visit(Add &node) {
        // Visit children
        node.Left->accept(*this);
        node.Right->accept(*this);
        // Visit stops here
    }

    void ScopeGenerator::visit(Sub &node) {
        // Visit children
        node.Left->accept(*this);
        node.Right->accept(*this);
        // Visit stops here
    }

    void ScopeGenerator::visit(Mul &node) {
        // Visit children
        node.Left->accept(*this);
        node.Right->accept(*this);
        // Visit stops here
    }

    void ScopeGenerator::visit(Div &node) {
        // Visit children
        node.Left->accept(*this);
        node.Right->accept(*this);
        // Visit stops here
    }

    void ScopeGenerator::visit(Mod &node) {
        // Visit children
        node.Left->accept(*this);
        node.Right->accept(*this);
        // Visit stops here
    }

    void ScopeGenerator::visit(ListAdd &node) {
        // Visit children
        node.Left->accept(*this);
        node.Right->accept(*this);
        // Visit stops here
    }

    void ScopeGenerator::visit(Par &node) {
        // Visit children
        node.Child->accept(*this);
        // Visit stops here
    }

    void ScopeGenerator::visit(Not &node) {
        // Visit children
        node.Child->accept(*this);
        // Visit stops here
    }

    void ScopeGenerator::visit(ListPattern &node) {
        if (node.Ty.Id == TypeId::LIST) {
            for (size_t i = 0; i < node.Patterns.size(); ++i) {
                node.Patterns[i]->Ty = node.Ty.Subtypes[i];
                node.Patterns[i]->accept(*this);
            }
        }
    }

    void ScopeGenerator::visit(TuplePattern &node) {
        if (node.Ty.Id == TypeId::TUPLE) {
            if (node.Patterns.size() == node.Ty.Subtypes.size()) {
                for (size_t i = 0; i < node.Patterns.size(); i++) {
                    node.Patterns[i]->Ty = node.Ty.Subtypes[i];
                    node.Patterns[i]->accept(*this);
                }
            }
        }
    }

    void ScopeGenerator::visit(ListSplit &node) {
        if (node.Ty.Id == TypeId::LIST) {
            node.Left->Ty = node.Ty.Subtypes.front();
            node.Right->Ty = node.Ty;
        } else if (node.Ty.Id == TypeId::STRING){
            node.Left->Ty = Type(TypeId::CHAR);
            node.Right->Ty = node.Ty;
        }

        node.Left->accept(*this);
        node.Right->accept(*this);
    }

    void ScopeGenerator::visit(List &node) {
        // Visit children
        for (auto &expr: node.Elements) {
            expr->accept(*this);
        }
        // Visit stops here
    }

    void ScopeGenerator::visit(Tuple &node) {
        // Visit children
        for (auto &expr: node.Elements) {
            expr->accept(*this);
        }
        // Visit stops here
    }

    void ScopeGenerator::visit(Id &node) {
        node.Scp = current_scope;

        if (context == ScopeContext::PATTERN) {
            if (!current_scope->exists(node.Val)) {
                current_scope->Decls.insert({node.Val, node.Ty});
            }
        }
    }

    void ScopeGenerator::visit(Call &node) {
        // Visit children
        node.Callee->accept(*this);
        for (auto &expr: node.Args) {
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
