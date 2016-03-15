#include "LLVMCodeGenerator.h"

using namespace llvm;

namespace codegen {
    LLVMCodeGenerator::LLVMCodeGenerator(std::ostream &out)
            : common::CodeGenerator::CodeGenerator(out),
              Builder(getGlobalContext()),
              Module(std::make_unique<llvm::Module>("SpplModule", getGlobalContext())) {}

    llvm::Function *LLVMCodeGenerator::create_function(common::Function *func) {
         // Get output type
        Type *output_type;
        switch (func->types[0]->type)
        {
            case common::Types::FLOAT:
                output_type = Type::getFloatTy(getGlobalContext());
                break;
            default:
                throw "Not supported";
        }

        // Get input types
        std::vector<Type *> input_types;
        for (unsigned i = 0; i < func->types.size() - 1; i++) {
            switch (func->types[i]->type) {
                case common::Types::FLOAT:
                    input_types.push_back(Type::getFloatTy(getGlobalContext()));
                    break;

                default:
                    throw "Not supported";
            }
        }
        
        // Setup function input/out types
        auto func_type = FunctionType::get(output_type, input_types, false);

        // Create anonymous or normal function
        if (jit_mode) {
            cur_func = Function::Create(func_type, Function::ExternalLinkage, "__anon_expr", Module.get());

            BasicBlock *block = BasicBlock::Create(getGlobalContext(), "entry", cur_func);
            Builder.SetInsertPoint(block);

            func->cases[0]->expr->accept(*this);

            Builder.CreateRet(cur_val);
        }
        else
            cur_func = Function::Create(func_type, Function::ExternalLinkage, func->id, Module.get());

        // Setup names for arguments
        unsigned i = 0;
        for (auto &arg : cur_func->args()) arg.setName("x" + to_string(i++));

        return cur_func;
    }

void LLVMCodeGenerator::visit(common::Program &node) {
    for (auto &func : node.funcs) {
        func->accept(*this);
    }

    Module->dump();
}

void LLVMCodeGenerator::visit(common::Function &node) {
    // Input/output types
    // std::vector<Type *> Doubles(node.patterns.size(),
    //                            Type::getDoubleTy(getGlobalContext()));

    create_function(&node);

    for (auto &_case : node.cases) {
        _case->accept(*this);
    }
}

void LLVMCodeGenerator::visit(common::Case &node) {
    // Create function block
    BasicBlock *block =
        BasicBlock::Create(getGlobalContext(), "entry", cur_func);
    Builder.SetInsertPoint(block);

    ContextValues.clear();
    for (auto &arg : cur_func->args()) ContextValues[arg.getName()] = &arg;

    node.expr->accept(*this);

    cur_val = Builder.CreateRet(cur_val);

    verifyFunction(*cur_func);
}

void LLVMCodeGenerator::visit(common::Add &node) {
    node.left->accept(*this);
    auto left = cur_val;
    node.right->accept(*this);
    auto right = cur_val;

    cur_val = Builder.CreateFAdd(left, right, "addtmp");
}

void LLVMCodeGenerator::visit(common::Float &node) {
    cur_val = ConstantFP::get(getGlobalContext(), APFloat(node.value));
}

void LLVMCodeGenerator::visit(common::Call &node) {
    node.callee->accept(*this);
    auto callee = cur_val;

    std::vector<Value *> args;
    for (auto &arg : node.exprs) {
        arg->accept(*this);
        args.push_back(cur_val);
    }

    cur_val = Builder.CreateCall(callee, args, "calltmp");
}

void LLVMCodeGenerator::visit(common::Id &node) {
    cur_val = Module->getFunction(node.id);
}
}
