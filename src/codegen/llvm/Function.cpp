#include "LLVMCodeGenerator.h"

using namespace llvm;
using namespace codegen;

void LLVMCodeGenerator::visit(common::Function &Node) {

    // Create function and entry block
    CurFunc = llvm::Function::Create(getFuncType(Node.Signature), llvm::Function::ExternalLinkage, Node.Id, Module.get());
    BasicBlock *Entry = BasicBlock::Create(getGlobalContext(), "entry", CurFunc);

    // Create error block
    CurErrorBlock = BasicBlock::Create(getGlobalContext(), "error", CurFunc);
    Builder.SetInsertPoint(CurErrorBlock);
    Builder.CreateRet(ConstantInt::get(IntegerType::get(getGlobalContext(), 64), 255));

    // Setup return block and phi node
    CurRetBlock = BasicBlock::Create(getGlobalContext(), "ret", CurFunc);
    Builder.SetInsertPoint(CurRetBlock);
    CurPhiNode = Builder.CreatePHI(CurFunc->getReturnType(), (unsigned)Node.Cases.size(), "rettmp");
    Builder.CreateRet(CurPhiNode);

    // Setup names for arguments
    auto i = 0;
    for (auto &Arg : CurFunc->args()) {
        Arg.setName("_arg" + to_string(i++));
        Arguments.push_back(&Arg);
    }

    // Setup case and pattern blocks
    //CaseBlocks.clear();
    /*for (size_t i = 0; i < node.cases.size(); ++i) {
        PatternBlocks[i].clear();
        for (size_t j = 0; j < node.cases[i]->patterns.size() ; j++)
            PatternBlocks[i].push_back(BasicBlock::Create(getGlobalContext(), "case" + to_string(i) + "_pattern" + to_string(j), cur_func));
        CaseBlocks[i] = BasicBlock::Create(getGlobalContext(), "case" + to_string(i), cur_func);
    }*/

    // Visit cases
    CurCaseId = LastCaseId = Node.Cases.size() - 1;
    for (auto &Case : Node.Cases) {
        Case->accept(*this);
        CurCaseId--;
    }

    // Make entry point to first pattern
    Builder.SetInsertPoint(Entry);
    Builder.CreateBr(CurPatternBlock);

    verifyFunction(*CurFunc);
}

Value *LLVMCodeGenerator::compare(Value *Val1, Value *Val2)
{
    if (Val1->getType()->isFloatTy())
        return Builder.CreateFCmpONE(Val1, Val2, "cmptmp");
    else if (Val1->getType()->isIntegerTy())
        return Builder.CreateICmpEQ(Val1, Val2, "cmptmp");
    else
        throw runtime_error("This should not happen!");
}

void LLVMCodeGenerator::visit(common::Case &Node) {

    CurCaseBlock = BasicBlock::Create(getGlobalContext(), "case" + to_string(CurCaseId), CurFunc);

    BasicBlock *TrueBlock;
    BasicBlock *FalseBlock;
    if (CurCaseId == LastCaseId)
        FalseBlock = CurErrorBlock;
    else
        FalseBlock = CurPatternBlock;


    if (Node.Patterns.size()) {
        ContextValues.clear();
        for (size_t i = Node.Patterns.size(); i != 0; --i) {

            // Last pattern should branch to next case
            if (i == Node.Patterns.size())
                TrueBlock = CurCaseBlock;
            else
                TrueBlock = CurPatternBlock;

            // Create new branch
            CurPatternBlock = BasicBlock::Create(getGlobalContext(),
                                                   "case" + to_string(CurCaseId) + "_pattern" + to_string(i - 1),
                                                   CurFunc);
            Builder.SetInsertPoint(CurPatternBlock);

            // Check arguments
            CurVal = Arguments[i - 1];
            Node.Patterns[i - 1]->accept(*this);
            CurVal = compare(CurVal, Arguments[i - 1]);

            // Create condition
            Builder.CreateCondBr(CurVal, TrueBlock, FalseBlock);
        }
    }
    else
    {
        CurPatternBlock = BasicBlock::Create(getGlobalContext(),
                                               "case" + to_string(CurCaseId) + "_pattern" + to_string(0),
                                               CurFunc);
        Builder.SetInsertPoint(CurPatternBlock);
        Builder.CreateBr(CurCaseBlock);
    }

    // Generate expression in case block
    Builder.SetInsertPoint(CurCaseBlock);
    Node.Expr->accept(*this);

    // Add return value to phi node
    CurPhiNode->addIncoming(CurVal, CurCaseBlock);
    Builder.CreateBr(CurRetBlock);
}

