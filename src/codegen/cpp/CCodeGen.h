#pragma once
#include "ScopeGenerator.h"
#include "TypeChecker.h"
#include "CodeGenerator.h"
#include "Parser.h"
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <cassert>

namespace codegen {
class CCodeGen : public parser::CodeGenerator {
  public:
    CCodeGen(parser::Driver &Drv);
    virtual void visit(common::Program &Node);

  protected:
    virtual void visit(common::Function &Node);
    virtual void visit(common::AlgebraicDT &Node);
    virtual void visit(common::Product &Node);
    virtual void visit(common::Case &Node);
    virtual void visit(common::LambdaArg &Node);
    virtual void visit(common::Or &Node);
    virtual void visit(common::And &Node);
    virtual void visit(common::Equal &Node);
    virtual void visit(common::NotEqual &Node);
    virtual void visit(common::Lesser &Node);
    virtual void visit(common::Greater &Node);
    virtual void visit(common::LesserEq &Node);
    virtual void visit(common::GreaterEq &Node);
    virtual void visit(common::Add &Node);
    virtual void visit(common::Sub &Node);
    virtual void visit(common::Mul &Node);
    virtual void visit(common::Div &Node);
    virtual void visit(common::Mod &Node);
    virtual void visit(common::ListAdd &Node);
    virtual void visit(common::ProducerConsumer &Node);
    virtual void visit(common::Concat &Node);
    virtual void visit(common::To &Node);
    virtual void visit(common::ParExpr &Node);
    virtual void visit(common::Not &Node);
    virtual void visit(common::Negative &Node);
    virtual void visit(common::LambdaFunction &Node);
    virtual void visit(common::IdPattern &Node);
    virtual void visit(common::IntPattern &Node);
    virtual void visit(common::FloatPattern &Node);
    virtual void visit(common::CharPattern &Node);
    virtual void visit(common::BoolPattern &Node);
    virtual void visit(common::StringPattern &Node);
    virtual void visit(common::ListPattern &Node);
    virtual void visit(common::TuplePattern &Node);
    virtual void visit(common::ListSplit &Node);
    virtual void visit(common::WildPattern &Node);
    virtual void visit(common::AlgebraicPattern &Node);
    virtual void visit(common::ParPattern &Node);
    virtual void visit(common::IdExpr &Node);
    virtual void visit(common::IntExpr &Node);
    virtual void visit(common::FloatExpr &Node);
    virtual void visit(common::CharExpr &Node);
    virtual void visit(common::BoolExpr &Node);
    virtual void visit(common::StringExpr &Node);
    virtual void visit(common::ListExpr &Node);
    virtual void visit(common::TupleExpr &Node);
    virtual void visit(common::CallExpr &Node);
    virtual void visit(common::AlgebraicExpr &Node);

    const std::string GGenerated = "g_";
    const std::string GUser = "u_";
    const std::string GAdd = "add_";
    const std::string GCreate = "crt_";
    const std::string GCompare = "comp_";
    const std::string GConcat = "con_";
    const std::string GPrint = "print_";
    const std::string GAt = "at_";
    const std::string GToString = "tostr_";
    const std::string GFloat = "double";
    const std::string GInt = "int64_t";
    const std::string GChar = "int";
    const std::string GBool = "int";
    const std::string GString = "str";
    const std::string GList = "list";
    const std::string GSignature = "sig";
    const std::string GTuple = "tuple";
    const std::string GNext = "nxt";
    const std::string GValue = "val";
    const std::string GEmpty = "empty";
    const std::string GSize = "size";
    const std::string GItem = "item";
    const std::string GArg = "arg";
    //const std::string GClosure = "closure";
    //const std::string GGlobal = "global";
    const std::string GMain = "main";

    std::stringstream Buffer;
    std::ostream *Output;
    std::ostream *Header;

    int TupleCount = 0;
    int ListCount = 0;
    int SigCount = 0;
    int OobCount = 0;
    int EnvCount = 0;
    std::unordered_map<common::Type, std::string> Tuples;
    std::unordered_map<common::Type, std::string> Lists;
    std::unordered_map<common::Type, std::string> Closures;
    std::unordered_map<common::Type, std::string> ToStrings;
    std::unordered_map<common::Type, std::string> Prints;
    std::vector<std::string> PatternBuilder;
    std::vector<std::string> Assignments;
    std::vector<int> ListOffsets;
    std::string LastPattern;
    std::string StringTypeName;

    std::stack<std::stringstream> ExprStack;

    common::Type StringList;
    common::Type RealString;
    common::Type FakeString;

    common::Program *Prog;
    common::Function *CurFunc;

    virtual std::string getType(common::Type &Ty);
    virtual std::string generateList(common::Type &Ty);
    virtual std::string generateTuple(common::Type &Ty);
    virtual std::string generateEnvironment(common::Type &Ty);
    virtual std::string getList(common::Type &Ty);
    virtual std::string getTuple(common::Type &Ty);
    virtual std::string getEnvironment(common::Type &Ty);
    virtual void generateStd();
    virtual void outputBuffer();
    virtual void outputEqual(common::Type &Ty, common::Expression &Left, common::Expression &Right);
};
}
