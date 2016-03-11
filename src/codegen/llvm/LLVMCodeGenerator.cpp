#include "LLVMCodeGenerator.h"

using namespace llvm;

namespace codegen {
    LLVMCodeGenerator::LLVMCodeGenerator(std::ostream &out):
            common::CodeGenerator::CodeGenerator(out),
            Builder(getGlobalContext()),
            Module("SpplModule", getGlobalContext())
    {
    }

    void LLVMCodeGenerator::visit(common::Program &node) {
        for (auto& func: node.funcs) {
            func->accept(*this);
        }
    }


    void LLVMCodeGenerator::visit(common::Function &node) {
        for (auto& _case: node.cases) {
            current_func = &node;
            _case->accept(*this);
        }
    }

    void LLVMCodeGenerator::visit(common::Case &node) {

        std::vector<Type *> Doubles(node.patterns.size(), Type::getDoubleTy(getGlobalContext()));

        auto func_type = FunctionType::get(Type::getDoubleTy(getGlobalContext()), Doubles, false);

        auto func = Function::Create(func_type, Function::ExternalLinkage, current_func->id, &Module);

        unsigned i = 0;
        for (auto &arg : func->args())
            arg.setName("x"+i);

        BasicBlock *BB = BasicBlock::Create(getGlobalContext(), "entry", func);
        Builder.SetInsertPoint(BB);

        ContextValues.clear();
        for (auto &arg : func->args())
            ContextValues[arg.getName()] = &arg;

        node.expr->accept(*this);

        current_val = Builder.CreateRet(current_val);

        current_val->dump();

        verifyFunction(*func);
    }

    void LLVMCodeGenerator::visit(common::Add &node) {

        node.left->accept(*this);
        auto left = current_val;
        node.right->accept(*this);
        auto right = current_val;

        Builder.CreateFAdd(left, right, "addtmp");
    }

    void LLVMCodeGenerator::visit(common::Float &node)
    {
        current_val = ConstantFP::get(getGlobalContext(), APFloat(node.value));
    }
}