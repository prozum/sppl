#include "GeneralOptimizer.h"

#include <iostream>
#include <string>
#include <typeinfo>

using namespace std;
using namespace common;

namespace optimizer {

    void GeneralOptimizer::visit(Program &Node) {
        for (auto &Func : Node.Decls){
            Func->accept(*this);
        }
    }

    void GeneralOptimizer::visit(Function &Node) {
        for (auto &Case : Node.Cases) {
            auto Expr = Case->Expr.get();

            if (typeid(*Expr) == typeid(CallExpr) &&
                typeid(static_cast<CallExpr*>(Expr)->Callee) == typeid(IdPattern) &&
                (static_cast<IdExpr*>(static_cast<CallExpr*>(Expr)->Callee.get()))->Val == Node.Id) {
                Case->TailRec = true;
            }

            Case->accept(*this);
        }
    }

    void GeneralOptimizer::visit(Case &Node) {
        for (auto &Pattern : Node.Patterns) {
            Pattern->accept(*this);
        }

        Node.Expr->accept(*this);
    }

    void GeneralOptimizer::visit(Or &Node) {
        Node.Left->accept(*this);
        Node.Right->accept(*this);
    }

    void GeneralOptimizer::visit(And &Node) {
        Node.Left->accept(*this);
        Node.Right->accept(*this);
    }

    void GeneralOptimizer::visit(Equal &Node) {
        Node.Left->accept(*this);
        Node.Right->accept(*this);
    }

    void GeneralOptimizer::visit(NotEqual &Node) {
        Node.Left->accept(*this);
        Node.Right->accept(*this);
    }

    void GeneralOptimizer::visit(Lesser &Node) {
        Node.Left->accept(*this);
        Node.Right->accept(*this);
    }

    void GeneralOptimizer::visit(LesserEq &Node) {
        Node.Left->accept(*this);
        Node.Right->accept(*this);
    }

    void GeneralOptimizer::visit(Greater &Node) {
        Node.Left->accept(*this);
        Node.Right->accept(*this);
    }

    void GeneralOptimizer::visit(GreaterEq &Node) {
        Node.Left->accept(*this);
        Node.Right->accept(*this);
    }

    void GeneralOptimizer::visit(Add &Node) {
        Node.Left->accept(*this);
        Node.Right->accept(*this);
    }

    void GeneralOptimizer::visit(Sub &Node) {
        Node.Left->accept(*this);
        Node.Right->accept(*this);
    }

    void GeneralOptimizer::visit(Mul &Node) {
        Node.Left->accept(*this);
        Node.Right->accept(*this);
    }

    void GeneralOptimizer::visit(Div &Node) {
        Node.Left->accept(*this);
        Node.Right->accept(*this);
    }

    void GeneralOptimizer::visit(Mod &Node) {
        Node.Left->accept(*this);
        Node.Right->accept(*this);
    }

    void GeneralOptimizer::visit(ListAdd &Node) {
        Node.Left->accept(*this);
        Node.Right->accept(*this);
    }

    void GeneralOptimizer::visit(ParExpr &Node) {
        Node.Child->accept(*this);
    }

    void GeneralOptimizer::visit(Not &Node) {
        Node.Child->accept(*this);
    }

    void GeneralOptimizer::visit(ListPattern &Node) {
        for (auto &Pattern : Node.Patterns) {
            Pattern->accept(*this);
        }
    }

    void GeneralOptimizer::visit(TuplePattern &Node) {
        for (auto &Pattern : Node.Patterns) {
            Pattern->accept(*this);
        }
    }

    void GeneralOptimizer::visit(ListSplit &Node) {
        Node.Left->accept(*this);
        Node.Right->accept(*this);
    }

    void GeneralOptimizer::visit(IntPattern &Node) {
    }

    void GeneralOptimizer::visit(FloatPattern &Node) {
    }

    void GeneralOptimizer::visit(CharPattern &Node) {
    }

    void GeneralOptimizer::visit(ListExpr &Node) {
        for (auto &Element : Node.Elements) {
            Element->accept(*this);
        }
    }

    void GeneralOptimizer::visit(IdPattern &Node) {
    }

    void GeneralOptimizer::visit(CallExpr &Node) {
        Node.Callee->accept(*this);

        for (auto &Arg : Node.Args) {
            Arg->accept(*this);
        }
    }

    void GeneralOptimizer::visit(Type &Node) {
    }

    void GeneralOptimizer::visit(TupleExpr &Node) {
        for (auto &Element : Node.Elements) {
            Element->accept(*this);
        }
    }
}
