#pragma once

#include "Node.h"
#include "Visitor.h"

#include <ostream>
#include <string>
#include <stack>

namespace optimizer {

class GeneralOptimizer : public common::Visitor {
  public:
    void visit(common::Program &Node);

  private:
    void visit(common::Function &Node);
    void visit(common::Case &Node);
    void visit(common::IdExpr &Node);
    void visit(common::IntExpr &Node);
    void visit(common::FloatExpr &Node);
    void visit(common::CharExpr &Node);
    void visit(common::BoolExpr &Node);
    void visit(common::StringExpr &Node);
    void visit(common::ListExpr &Node);
    void visit(common::TupleExpr &Node);
    void visit(common::CallExpr &Node);
    void visit(common::AlgebraicExpr &Node);
    void visit(common::Or &Node);
    void visit(common::And &Node);
    void visit(common::Equal &Node);
    void visit(common::NotEqual &Node);
    void visit(common::Lesser &Node);
    void visit(common::Greater &Node);
    void visit(common::LesserEq &Node);
    void visit(common::GreaterEq &Node);
    void visit(common::Add &Node);
    void visit(common::Sub &Node);
    void visit(common::Mul &Node);
    void visit(common::Div &Node);
    void visit(common::Mod &Node);
    void visit(common::ListAdd &Node);
    void visit(common::ProducerConsumer &Node);
    void visit(common::Concat &Node);
    void visit(common::To &Node);
    void visit(common::ParExpr &Node);
    void visit(common::Not &Node);
    void visit(common::Negative &Node);
    void visit(common::LambdaFunction &Node);

    std::stack<common::CallExpr*> LastRecCall;
    std::stack<common::CallExpr*> LastOtherCall;
    common::Function* CurrFunc = nullptr;

    std::stack<int> Calls;

    void determingParallelism();
};
}
