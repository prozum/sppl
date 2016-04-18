#include "Scope.h"
#include <iostream>
#include "ScopeGenerator.h"

namespace semantics {
    ScopeGenerator::ScopeGenerator(Scope* Scp)
            : CurScope(Scp) { }

    void ScopeGenerator::visit(Program &Node) {
        // Visit children
        for (auto &Func : Node.Funcs) {
            Func->accept(*this);
        }
        // Visit stops here
    }

    void ScopeGenerator::visit(Function &Node) {
        CurFunc = &Node;
        Node.Scp = CurScope;

        if (!CurScope->exists(Node.Id) || Node.Anon) {
            CurScope->Decls.insert({Node.Id, Node.Signature});

            // Visit children
            for (auto &Case : Node.Cases) {
                Case->accept(*this);
            }
            // Visit stops here
        } else {
            throw Error(Node.Id + " has already been declared", Node.Loc);
        }
    }


    void ScopeGenerator::visit(Case &Node) {
        auto CaseScope = new Scope(CurScope);
        CurScope->Children.push_back(unique_ptr<Scope>(CaseScope));
        CurScope = CaseScope;

        // Visit children
        Ctx = ScopeContext::PATTERN;

        for (size_t i = 0; i < Node.Patterns.size(); ++i) {
            Node.Patterns[i]->Ty = CurFunc->Signature[i];
            Node.Patterns[i]->accept(*this);
        }

        Ctx = ScopeContext::EXPR;
        Node.Expr->accept(*this);

        CurScope = CurScope->Parent;
    }

    void ScopeGenerator::visit(Or &Node) {
        // Visit children
        Node.Left->accept(*this);
        Node.Right->accept(*this);
        // Visit stops here
    }

    void ScopeGenerator::visit(And &Node) {
        // Visit children
        Node.Left->accept(*this);
        Node.Right->accept(*this);
        // Visit stops here
    }

    void ScopeGenerator::visit(Equal &Node) {
        // Visit children
        Node.Left->accept(*this);
        Node.Right->accept(*this);
        // Visit stops here
    }

    void ScopeGenerator::visit(NotEqual &Node) {
        // Visit children
        Node.Left->accept(*this);
        Node.Right->accept(*this);
        // Visit stops here
    }

    void ScopeGenerator::visit(Lesser &Node) {
        // Visit children
        Node.Left->accept(*this);
        Node.Right->accept(*this);
        // Visit stops here
    }

    void ScopeGenerator::visit(Greater &Node) {
        // Visit children
        Node.Left->accept(*this);
        Node.Right->accept(*this);
        // Visit stops here
    }

    void ScopeGenerator::visit(LesserEq &Node) {
        // Visit children
        Node.Left->accept(*this);
        Node.Right->accept(*this);
        // Visit stops here
    }

    void ScopeGenerator::visit(GreaterEq &Node) {
        // Visit children
        Node.Left->accept(*this);
        Node.Right->accept(*this);
        // Visit stops here
    }

    void ScopeGenerator::visit(Add &Node) {
        // Visit children
        Node.Left->accept(*this);
        Node.Right->accept(*this);
        // Visit stops here
    }

    void ScopeGenerator::visit(Sub &Node) {
        // Visit children
        Node.Left->accept(*this);
        Node.Right->accept(*this);
        // Visit stops here
    }

    void ScopeGenerator::visit(Mul &Node) {
        // Visit children
        Node.Left->accept(*this);
        Node.Right->accept(*this);
        // Visit stops here
    }

    void ScopeGenerator::visit(Div &Node) {
        // Visit children
        Node.Left->accept(*this);
        Node.Right->accept(*this);
        // Visit stops here
    }

    void ScopeGenerator::visit(Mod &Node) {
        // Visit children
        Node.Left->accept(*this);
        Node.Right->accept(*this);
        // Visit stops here
    }

    void ScopeGenerator::visit(ListAdd &Node) {
        // Visit children
        Node.Left->accept(*this);
        Node.Right->accept(*this);
        // Visit stops here
    }

    void ScopeGenerator::visit(Par &Node) {
        // Visit children
        Node.Child->accept(*this);
        // Visit stops here
    }

    void ScopeGenerator::visit(Not &Node) {
        // Visit children
        Node.Child->accept(*this);
        // Visit stops here
    }

    void ScopeGenerator::visit(ListPattern &Node) {
        if (Node.Ty.Id == TypeId::LIST) {
            for (size_t i = 0; i < Node.Patterns.size(); ++i) {
                Node.Patterns[i]->Ty = Node.Ty.Subtypes[i];
                Node.Patterns[i]->accept(*this);
            }
        }
    }

    void ScopeGenerator::visit(TuplePattern &Node) {
        if (Node.Ty.Id == TypeId::TUPLE) {
            if (Node.Patterns.size() == Node.Ty.Subtypes.size()) {
                for (size_t i = 0; i < Node.Patterns.size(); ++i) {
                    Node.Patterns[i]->Ty = Node.Ty.Subtypes[i];
                    Node.Patterns[i]->accept(*this);
                }
            }
        }
    }

    void ScopeGenerator::visit(ListSplit &Node) {
        if (Node.Ty.Id == TypeId::LIST) {
            Node.Left->Ty = Node.Ty.Subtypes.front();
            Node.Right->Ty = Node.Ty;
        } else if (Node.Ty.Id == TypeId::STRING){
            Node.Left->Ty = Type(TypeId::CHAR);
            Node.Right->Ty = Node.Ty;
        }

        Node.Left->accept(*this);
        Node.Right->accept(*this);
    }

    void ScopeGenerator::visit(ListExpression &Node) {
        // Visit children
        for (auto &Element : Node.Elements) {
            Element->accept(*this);
        }
        // Visit stops here
    }

    void ScopeGenerator::visit(TupleExpression &Node) {
        // Visit children
        for (auto &Element : Node.Elements) {
            Element->accept(*this);
        }
        // Visit stops here
    }

    void ScopeGenerator::visit(IdPattern &Node) {
        Node.Scp = CurScope;

        if (Ctx == ScopeContext::PATTERN) {
            if (!CurScope->exists(Node.Val)) {
                CurScope->Decls.insert({Node.Val, Node.Ty});
            }
        }
    }

    void ScopeGenerator::visit(Call &Node) {
        // Visit children
        Node.Callee->accept(*this);
        for (auto &Arg : Node.Args) {
            Arg->accept(*this);
        }
        // Visit stops here
    }

    void ScopeGenerator::visit(Type &Node) { }

    void ScopeGenerator::visit(IntPattern &Node) {
    }

    void ScopeGenerator::visit(FloatPattern &Node) {
    }

    void ScopeGenerator::visit(Bool &Node) {
    }

    void ScopeGenerator::visit(CharPattern &Node) {
    }

    void ScopeGenerator::visit(String &Node) {
    }
}
