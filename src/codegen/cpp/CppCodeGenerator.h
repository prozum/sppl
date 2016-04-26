#pragma once
#include "ScopeGenerator.h"
#include "TypeChecker.h"
#include "CodeGenerator.h"
#include "Node.h"
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

namespace codegen {
enum class IdContext { PATTERN, EXPR };

class CCodeGenerator : public parser::CodeGenerator {
  public:
    CCodeGenerator(parser::Driver &Drv);
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
    void visit(common::ParExpr &Node);
    void visit(common::Not &Node);
    void visit(common::Negative &Node);
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

    const std::string GGenerated = "generated_";
    const std::string GUser = "user_";
    const std::string GAdd = "add_";
    const std::string GCreate = "create_";
    const std::string GCompare = "compare_";
    const std::string GConcat = "concat_";
    const std::string GPrint = "print_";
    const std::string GAt = "at_";
    const std::string GValueAt = "valueat_";
    const std::string GToString = "tostd::string_";
    const std::string GFloat = "double";
    const std::string GInt = "int64_t";
    const std::string GChar = "int";
    const std::string GBool = "int";
    const std::string GString = "std::string";
    const std::string GList = "list";
    const std::string GSignature = "signature";
    const std::string GTuple = "tuple";
    const std::string GNext = "next";
    const std::string GValue = "value";
    const std::string GEmpty = "empty";
    const std::string GSize = "size";
    const std::string GItem = "item";
    const std::string GArg = "arg";
    const std::string GClosure = "closure";
    const std::string GGlobal = "global";
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
    std::vector<std::string> ArgNames;
    std::vector<std::string> GetValueBuilder;
    std::vector<std::string> Assignments;
    std::vector<int> ListOffsets;
    IdContext IdCtx;
    std::string LastPattern;
    std::string StringTypeName;

    std::stack<std::stringstream> ExprStack;

    common::Type StringList;
    common::Type RealString;

    common::Program *Prog;
    common::Function *CurFunc;

    std::string getType(common::Type &Ty);
    std::string generateList(common::Type &Ty);
    std::string generateTuple(common::Type &Ty);
    std::string generateEnvironment(common::Type &Ty);
    std::string getList(common::Type &Ty);
    std::string getTuple(common::Type &Ty);
    std::string getEnvironment(common::Type &Ty);
    void generateStd();
    void outputBuffer();
    void outputEqual(common::Type &Ty, common::Expression &Left, common::Expression &Right);
};
}
