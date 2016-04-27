#include "GeneralOptimizer.h"

#include <iostream>
#include <string>
#include <typeinfo>

using namespace std;
using namespace common;
using namespace optimizer;

void GeneralOptimizer::visit(Program &Node) {
    for (auto &Func : Node.Decls) {
        Func->accept(*this);
    }
}

void GeneralOptimizer::visit(Function &Node) {
    for (auto &Case : Node.Cases) {
        auto Expr = Case->Expr.get();

        if (typeid(*Expr) == typeid(CallExpr) &&
            typeid(*static_cast<CallExpr *>(Expr)->Callee.get()) == typeid(IdExpr) &&
            static_cast<IdExpr *>(static_cast<CallExpr *>(Expr)->Callee.get())->Val == Node.Id)
        {
            Case->TailRec = true;
        }

        //Case->accept(*this);
    }
}
