#pragma once

#include "Visitor.h"

#include <Node.h>
#include <stack>
#include <vector>

namespace semantics {
class TypeChecker : public common::Visitor {
  public:
    TypeChecker(common::Scope *Scp);

    void visit(common::Program &Node);

  private:
    void visit(common::Function &Node);
    void visit(common::AlgebraicDT &Node);
    void visit(common::Product &Node);
    void visit(common::Case &Node);
    void visit(common::LambdaArg &Node);
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
    void visit(common::UnPrint &Node);
    void visit(common::ParExpr &Node);
    void visit(common::Not &Node);
    void visit(common::Negative &Node);
    void visit(common::DoExpr &Node);
    void visit(common::Assosiate &Node);
    void visit(common::LambdaFunction &Node);
    void visit(common::IdPattern &Node);
    void visit(common::IntPattern &Node);
    void visit(common::FloatPattern &Node);
    void visit(common::CharPattern &Node);
    void visit(common::BoolPattern &Node);
    void visit(common::StringPattern &Node);
    void visit(common::ListPattern &Node);
    void visit(common::TuplePattern &Node);
    void visit(common::ListSplit &Node);
    void visit(common::WildPattern &Node);
    void visit(common::AlgebraicPattern &Node);
    void visit(common::ParPattern &Node);
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

    bool containsEmptyList(common::Type &Ty);
    void resolveEmptyList(common::Type &Ty, common::Type &Resolver);

    common::Scope *CurScope;
    common::Scope *GlobalScope;
    common::Function *CurFunc = nullptr;
};
}
