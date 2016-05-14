#include "GeneralOptimizer.h"
using namespace std;
using namespace common;
using namespace optimizer;

void GeneralOptimizer::visit(Program &Node) {
    for (auto &Func : Node.Decls) {
        Func->accept(*this);
    }
}

void GeneralOptimizer::visit(Function &Node) {
    CurrFunc = &Node;

    for (auto &Case : Node.Cases) {
        auto Expr = Case->Expr.get();

        if (typeid(*Expr) == typeid(CallExpr) &&
            typeid(*static_cast<CallExpr *>(Expr)->Callee.get()) == typeid(IdExpr) &&
            static_cast<IdExpr *>(static_cast<CallExpr *>(Expr)->Callee.get())->Val == Node.Id)
        {
            Case->TailRec = true;
        }

        Case->accept(*this);
    }
}

void GeneralOptimizer::visit(common::Case &Node) {
    if (Node.When) {
        Calls.push_back(0);
        LastRecCall.push_back(nullptr);
        LastOtherCall.push_back(nullptr);

        Node.When->accept(*this);

        determingParallelism();
    }


    Calls.push_back(0);
    LastRecCall.push_back(nullptr);
    LastOtherCall.push_back(nullptr);

    Node.Expr->accept(*this);

    determingParallelism();
}

void GeneralOptimizer::visit(common::IdExpr &Node) {

}

void GeneralOptimizer::visit(common::IntExpr &Node) {

}

void GeneralOptimizer::visit(common::FloatExpr &Node) {

}

void GeneralOptimizer::visit(common::CharExpr &Node) {
    
}

void GeneralOptimizer::visit(common::BoolExpr &Node) {

}

void GeneralOptimizer::visit(common::StringExpr &Node) {

}

void GeneralOptimizer::visit(common::ListExpr &Node) {
    for (auto &Expr: Node.Elements) {
        Expr->accept(*this);
    }
}

void GeneralOptimizer::visit(common::TupleExpr &Node) {
    for (auto &Expr: Node.Elements) {
        Expr->accept(*this);
    }
}

void GeneralOptimizer::visit(common::CallExpr &Node) {
    Node.Callee->accept(*this);

    if (LastOtherCall.size() == CallDepth) {
        Calls.push_back(0);
        LastRecCall.push_back(nullptr);
        LastOtherCall.push_back(nullptr);
    }

    CallDepth++;

    for (auto &Expr: Node.Args) {
        Expr->accept(*this);
    }
    Node.Callee->accept(*this);
    CallDepth--;

    if (typeid(*Node.Callee.get()) == typeid(IdExpr) &&
        static_cast<IdExpr*>(Node.Callee.get())->Val == CurrFunc->Id) {
        LastRecCall[CallDepth] = &Node;
    } else {
        LastOtherCall[CallDepth] = &Node;
    }

    Calls[CallDepth]++;
}

void GeneralOptimizer::visit(common::AlgebraicExpr &Node) {
    for (auto &Expr: Node.Exprs) {
        Expr->accept(*this);
    }
}

void GeneralOptimizer::visit(common::Or &Node) {
    Node.Left->accept(*this);
    Node.Right->accept(*this);
}

void GeneralOptimizer::visit(common::And &Node) {
    Node.Left->accept(*this);
    Node.Right->accept(*this);
}

void GeneralOptimizer::visit(common::Equal &Node) {
    Node.Left->accept(*this);
    Node.Right->accept(*this);
}

void GeneralOptimizer::visit(common::NotEqual &Node) {
    Node.Left->accept(*this);
    Node.Right->accept(*this);
}

void GeneralOptimizer::visit(common::Lesser &Node) {
    Node.Left->accept(*this);
    Node.Right->accept(*this);
}

void GeneralOptimizer::visit(common::Greater &Node) {
    Node.Left->accept(*this);
    Node.Right->accept(*this);
}

void GeneralOptimizer::visit(common::LesserEq &Node) {
    Node.Left->accept(*this);
    Node.Right->accept(*this);
}

void GeneralOptimizer::visit(common::GreaterEq &Node) {
    Node.Left->accept(*this);
    Node.Right->accept(*this);
}

void GeneralOptimizer::visit(common::Add &Node) {
    Node.Left->accept(*this);
    Node.Right->accept(*this);
}

void GeneralOptimizer::visit(common::Sub &Node) {
    Node.Left->accept(*this);
    Node.Right->accept(*this);
}

void GeneralOptimizer::visit(common::Mul &Node) {
    Node.Left->accept(*this);
    Node.Right->accept(*this);
}

void GeneralOptimizer::visit(common::Div &Node) {
    Node.Left->accept(*this);
    Node.Right->accept(*this);
}

void GeneralOptimizer::visit(common::Mod &Node) {
    Node.Left->accept(*this);
    Node.Right->accept(*this);
}

void GeneralOptimizer::visit(common::ListAdd &Node) {
    Node.Left->accept(*this);
    Node.Right->accept(*this);
}

void GeneralOptimizer::visit(common::ProducerConsumer &Node) {
    Node.Left->accept(*this);
    Node.Right->accept(*this);
}

void GeneralOptimizer::visit(common::Concat &Node) {
    Node.Left->accept(*this);
    Node.Right->accept(*this);
}

void GeneralOptimizer::visit(common::To &Node) {
    Node.Child->accept(*this);
}

void GeneralOptimizer::visit(common::ParExpr &Node) {
    Node.Child->accept(*this);
}

void GeneralOptimizer::visit(common::Not &Node) {
    Node.Child->accept(*this);
}

void GeneralOptimizer::visit(common::Negative &Node) {
    Node.Child->accept(*this);
}

void GeneralOptimizer::visit(common::LambdaFunction &Node) {
    Node.Expr->accept(*this);
}

void GeneralOptimizer::determingParallelism() {
    while (!Calls.empty()) {
        if (Calls.back() == 1 && LastRecCall.back()) {
            LastRecCall.back()->DoParallel = false;
        } else if (Calls.back() > 1) {
            if (LastOtherCall.back())
                LastOtherCall.back()->DoParallel = false;
            else if (LastRecCall.back())
                LastRecCall.back()->DoParallel = false;
        }

        LastRecCall.pop_back();
        LastOtherCall.pop_back();
        Calls.pop_back();
    }

    CallDepth = 0;
}



