#pragma once

#include "CodeGenerator.h"
#include "Node.h"
#include "Scope.h"

//#include <bits/stl_map.h>
#include <iostream>
#include <map>
#include <string>
#include <vector>

namespace codegen {
class GasCodeGen : public parser::CodeGenerator {
  public:
    GasCodeGen(parser::Driver &Drv);

    void visit(common::Program &Node);
    void visit(common::Function &Node);
    void visit(common::Case &Node);
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
    void visit(common::ParExpr &Node);
    void visit(common::Not &Node);
    void visit(common::IntExpr &Node);
    void visit(common::FloatExpr &Node);
    void visit(common::BoolExpr &Node);
    void visit(common::CharExpr &Node);
    void visit(common::StringExpr &Node);
    void visit(common::ListExpr &Node);
    void visit(common::TupleExpr &Node);
    void visit(common::ListSplit &Node);
    void visit(common::IdExpr &Node);
    void visit(common::CallExpr &Node);
    void visit(common::Type &Node);

  private:
    std::string getType(common::Type *Ty);
    std::string buildSource();

    std::string Func;     // Contains the current function
    std::string FuncName; // Contains the name of the current function

    std::vector<std::string> FuncVector; // Contains all functions that have been read
    std::vector<std::string> FuncGlobl;  // Contains a globl for all functions

    size_t CaseCount = 0; // Used to count the current case
    size_t Cases = 0;     // Number of cases
                          // These may be moved into function later

    std::map<std::string, std::string> VarMap;

    typedef struct {
        std::string TypeName;
        std::string TypeValue;
    } Helper;

    Helper Hpr;
};
}
