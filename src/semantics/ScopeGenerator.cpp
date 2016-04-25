#include "Scope.h"
#include <iostream>
#include "ScopeGenerator.h"

namespace semantics {
    ScopeGenerator::ScopeGenerator(Scope* Scp)
            : CurScope(Scp) { }

    void ScopeGenerator::visit(Program &Node) {
        // Visit children

        for (auto &Func : Node.Decls) {
            try {
                Func->accept(*this);
            }
            catch (Error err) {
                Errors.push_back(err);
            }
        }
    }

    void ScopeGenerator::visit(Function &Node) {
        CurFunc = &Node;
        Node.Scp = CurScope;

        if (!CurScope->declExists(Node.Id) || Node.Anon) {
            CurScope->Decls.insert({Node.Id, Node.Signature});

            // Visit children
            for (auto &Case : Node.Cases) {
                Case->accept(*this);
            }
        } else {
            throw Error(Node.Id + " has already been declared", Node.Loc);
        }
    }

    void ScopeGenerator::visit(AlgebraicDT &Node) {
        CurScope->Types.insert({ Node.Name, Node });
    }

    void ScopeGenerator::visit(Product &Node) {

    }


    void ScopeGenerator::visit(Case &Node) {
        auto CaseScope = new Scope(CurScope);
        CurScope->Children.push_back(unique_ptr<Scope>(CaseScope));
        CurScope = CaseScope;

        // Visit children
        for (size_t i = 0; i < Node.Patterns.size(); ++i) {
            Node.Patterns[i]->RetTy = CurFunc->Signature[i];
            Node.Patterns[i]->accept(*this);
        }

        Node.Expr->accept(*this);

        CurScope = CurScope->Parent;
    }

    void ScopeGenerator::visit(LambdaArg &Node) {
        Node.Scp = CurScope;

        // TODO Type infer this
        /*
        if (!CurScope->declExists(Node.Id) {
            CurScope->Decls.insert({Node.Id, Node.RetTy});
        } else {
            throw Error("Id was allready declared in the current scope", Node.Loc);
        }
        */
    }

    void ScopeGenerator::visit(Or &Node) {
        // Visit children
        Node.Left->accept(*this);
        Node.Right->accept(*this);
    }

    void ScopeGenerator::visit(And &Node) {
        // Visit children
        Node.Left->accept(*this);
        Node.Right->accept(*this);
    }

    void ScopeGenerator::visit(Equal &Node) {
        // Visit children
        Node.Left->accept(*this);
        Node.Right->accept(*this);
    }

    void ScopeGenerator::visit(NotEqual &Node) {
        // Visit children
        Node.Left->accept(*this);
        Node.Right->accept(*this);
    }

    void ScopeGenerator::visit(Lesser &Node) {
        // Visit children
        Node.Left->accept(*this);
        Node.Right->accept(*this);
    }

    void ScopeGenerator::visit(Greater &Node) {
        // Visit children
        Node.Left->accept(*this);
        Node.Right->accept(*this);
    }

    void ScopeGenerator::visit(LesserEq &Node) {
        // Visit children
        Node.Left->accept(*this);
        Node.Right->accept(*this);
    }

    void ScopeGenerator::visit(GreaterEq &Node) {
        // Visit children
        Node.Left->accept(*this);
        Node.Right->accept(*this);
    }

    void ScopeGenerator::visit(Add &Node) {
        // Visit children
        Node.Left->accept(*this);
        Node.Right->accept(*this);
    }

    void ScopeGenerator::visit(Sub &Node) {
        // Visit children
        Node.Left->accept(*this);
        Node.Right->accept(*this);
    }

    void ScopeGenerator::visit(Mul &Node) {
        // Visit children
        Node.Left->accept(*this);
        Node.Right->accept(*this);
    }

    void ScopeGenerator::visit(Div &Node) {
        // Visit children
        Node.Left->accept(*this);
        Node.Right->accept(*this);
    }

    void ScopeGenerator::visit(Mod &Node) {
        // Visit children
        Node.Left->accept(*this);
        Node.Right->accept(*this);
    }

    void ScopeGenerator::visit(ListAdd &Node) {
        // Visit children
        Node.Left->accept(*this);
        Node.Right->accept(*this);
    }

    void ScopeGenerator::visit(ProducerConsumer &Node) {
        // Visit children
        Node.Left->accept(*this);
        Node.Right->accept(*this);
    }

    void ScopeGenerator::visit(Concat &Node) {
        // Visit children
        Node.Left->accept(*this);
        Node.Right->accept(*this);
    }

    void ScopeGenerator::visit(To &Node) {
        // Visit children
        Node.Child->accept(*this);
    }

    void ScopeGenerator::visit(ParExpr &Node) {
        // Visit children
        Node.Child->accept(*this);
    }

    void ScopeGenerator::visit(Not &Node) {
        // Visit children
        Node.Child->accept(*this);
    }

    void ScopeGenerator::visit(Negative &Node) {
        // Visit children
        Node.Child->accept(*this);
    }

    void ScopeGenerator::visit(LambdaFunction &Node) {
        auto LambdaScope = new Scope(CurScope);
        CurScope->Children.push_back(unique_ptr<Scope>(LambdaScope));
        CurScope = LambdaScope;

        for (auto &Arg: Node.Args) {
            Arg->accept(*this);
        }
        Node.Expr->accept(*this);

        // TODO Get the type of the lambda

        CurScope = CurScope->Parent;
    }

    void ScopeGenerator::visit(BoolPattern &Node) {

    }

    void ScopeGenerator::visit(StringPattern &Node) {

    }

    void ScopeGenerator::visit(WildPattern &Node) {

    }

    void ScopeGenerator::visit(AlgebraicPattern &Node) {

    }

    void ScopeGenerator::visit(ParPattern &Node) {
        Node.Pat->accept(*this);
    }

    void ScopeGenerator::visit(IdPattern &Node) {
        Node.Scp = CurScope;

        if (!CurScope->declExists(Node.Val)) {
            CurScope->Decls.insert({Node.Val, Node.RetTy});
        } else {
            throw Error("Id was allready declared in the current scope", Node.Loc);
        }
    }

    void ScopeGenerator::visit(ListPattern &Node) {
        if (Node.RetTy.Id == TypeId::LIST) {
            for (auto &Pattern: Node.Patterns) {
                Pattern->RetTy = Node.RetTy.Subtypes.back();
                Pattern->accept(*this);
            }
        } else {
            throw Error::Expected("Wrong pattern type was used",
                                  Node.RetTy.str(),
                                  "ListSplit",
                                  Node.Loc);
        }
    }

    void ScopeGenerator::visit(TuplePattern &Node) {
        if (Node.RetTy.Id == TypeId::TUPLE) {
            if (Node.Patterns.size() == Node.RetTy.Subtypes.size()) {
                for (size_t i = 0; i < Node.Patterns.size(); ++i) {
                    Node.Patterns[i]->RetTy = Node.RetTy.Subtypes[i];
                    Node.Patterns[i]->accept(*this);
                }
            }
        } else {
            throw Error::Expected("Wrong pattern type was used",
                                  Node.RetTy.str(),
                                  "TuplePattern",
                                  Node.Loc);
        }
    }

    void ScopeGenerator::visit(ListSplit &Node) {
        if (Node.RetTy.Id == TypeId::LIST) {
            Node.Left->RetTy = Node.RetTy.Subtypes.front();
            Node.Right->RetTy = Node.RetTy;
        } else {
            throw Error::Expected("Wrong pattern type was used",
                                  Node.RetTy.str(),
                                  "ListSplit",
                                  Node.Loc);
        }

        Node.Left->accept(*this);
        Node.Right->accept(*this);
    }

    void ScopeGenerator::visit(ListExpr &Node) {
        // Visit children
        for (auto &Element : Node.Elements) {
            Element->accept(*this);
        }
    }

    void ScopeGenerator::visit(TupleExpr &Node) {
        // Visit children
        for (auto &Element : Node.Elements) {
            Element->accept(*this);
        }
    }

    void ScopeGenerator::visit(IdExpr &Node) {
        Node.Scp = CurScope;
    }

    void ScopeGenerator::visit(CallExpr &Node) {
        // Visit children
        Node.Callee->accept(*this);
        for (auto &Arg : Node.Args) {
            Arg->accept(*this);
        }
    }

    void ScopeGenerator::visit(BoolExpr &Node) { }
    void ScopeGenerator::visit(IntPattern &Node) { }
    void ScopeGenerator::visit(FloatPattern &Node) { }
    void ScopeGenerator::visit(CharPattern &Node) { }
    void ScopeGenerator::visit(IntExpr &Node) { }
    void ScopeGenerator::visit(FloatExpr &Node) { }
    void ScopeGenerator::visit(CharExpr &Node) { }
    void ScopeGenerator::visit(StringExpr &Node) { }

    void ScopeGenerator::visit(AlgebraicExpr &Node) {
        for (auto &Expr: Node.Exprs) {
            Expr->accept(*this);
        }
    }
}
