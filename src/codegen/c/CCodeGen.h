#pragma once
#include "ScopeGenerator.h"
#include "TypeChecker.h"
#include "CodeGenerator.h"
#include "Parser.h"
#include "CTree.h"

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
    virtual void visit(common::UnPrint &Node);
    virtual void visit(common::ParExpr &Node);
    virtual void visit(common::Not &Node);
    virtual void visit(common::Negative &Node);
    virtual void visit(common::DoExpr &Node);
    virtual void visit(common::Assosiate &Node);
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

    const std::string GGenerated = "g";
    const std::string GUser = "u";
    const std::string GCreate = GGenerated + "ctr_";
    const std::string GAdd = GGenerated + "add_";
    const std::string GAt = GGenerated + "at_";
    const std::string GCompare = GGenerated + "compare_";
    const std::string GPrint = GGenerated + "print_";
    const std::string GConcat = GGenerated + "concat_";


    const std::string GString = GGenerated + "string";
    const std::string GList = GGenerated + "list";
    const std::string GSignature = GGenerated + "sig";
    const std::string GTuple = GGenerated + "tuple";
    const std::string GFloat = "double";
    const std::string GInt = "int64_t";
    const std::string GChar = "int";
    const std::string GBool = "int";

    const std::string GHead = "head";
    const std::string GLength = "length";
    const std::string GNode = "node";
    const std::string GNext = "next";
    const std::string GValue = "value";
    const std::string GItem = "item";
    const std::string GArg = GGenerated + "arg";
    const std::string GRes = GGenerated + "res";

    const bool GCed = false;
    const std::string GAlloc = "malloc";

    std::unordered_map<common::Type, std::string> GenTypes;
    std::unordered_map<common::Type, std::string> ToStrings;
    std::unordered_map<common::Type, std::string> Prints;

    std::vector<int> ListOffsets;
    common::Type StringList;
    common::Type RealString;
    common::Type FakeString;

    std::string StringName;
    std::string StringType;

    int ListCount = 0;
    int TupleCount = 0;
    int SigCount = 0;

    common::Program* Prog = nullptr;
    common::Function* CurrFunc = nullptr;

    // Data assosiated with the C tree
    ctree::Program* CProg = nullptr;
    ctree::Block* CurrBlock = nullptr;
    ctree::Expression* PatternBuilder = nullptr;
    ctree::Expression* LastExpr = nullptr;

    virtual std::string typePostfix(common::Type &Ty);
    virtual std::string getName(common::Type &Ty);
    virtual std::string getOrGen(common::Type &Ty);
    virtual ctree::Expression* visitBinOp(common::BinaryOp& Op, std::string OpStr);
    ctree::Expression* visitEqual(common::Type &Ty, common::Expression &Left, common::Expression &Right);

    std::string generateList(common::Type &Ty);
    std::string generateSignature(common::Type &Ty);
    std::string generateTuple(common::Type &Ty);
    void generateStd();
};
}
