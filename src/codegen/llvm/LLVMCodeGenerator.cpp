#include "LLVMCodeGenerator.h"

using namespace llvm;

namespace codegen {
    LLVMCodeGenerator::LLVMCodeGenerator(std::ostream &out)
            : common::CodeGenerator::CodeGenerator(out),
              Builder(getGlobalContext()),
              Module(std::make_unique<llvm::Module>("SpplModule", getGlobalContext())) {}

    AllocaInst *LLVMCodeGenerator::CreateAlloca(const std::string &name) {
        IRBuilder<> TmpB(&cur_func->getEntryBlock(), cur_func->getEntryBlock().begin());
        return TmpB.CreateAlloca(Type::getDoubleTy(getGlobalContext()), nullptr, name.c_str());
    }

    void LLVMCodeGenerator::visit(common::Program &node) {
        for (auto &func : node.funcs) {
            func->accept(*this);
        }

        Module->dump();
    }

    void LLVMCodeGenerator::visit(common::Function &node) {

        // Get output type
        Type *output_type;
        switch (node.types[0]->type)
        {
            case common::Types::FLOAT:
                output_type = Type::getFloatTy(getGlobalContext());
                break;
            case common::Types::INT:
                output_type = Type::getInt64Ty(getGlobalContext());
                break;
            default:
                throw "Not supported";
        }

        // Get input types
        std::vector<Type *> input_types;
        for (unsigned i = 0; i < node.types.size() - 1; i++) {
            switch (node.types[i]->type) {
                case common::Types::FLOAT:
                    input_types.push_back(Type::getFloatTy(getGlobalContext()));
                    break;
                case common::Types::INT:
                    input_types.push_back(Type::getInt64Ty(getGlobalContext()));
                    break;

                default:
                    throw "Not supported";
            }
        }

        // Setup function input/output types
        auto func_type = FunctionType::get(output_type, input_types, false);

        // Create function and entry block
        cur_func = Function::Create(func_type, Function::ExternalLinkage, node.id, Module.get());
        BasicBlock *block = BasicBlock::Create(getGlobalContext(), "entry", cur_func);
        Builder.SetInsertPoint(block);

        // Setup names for arguments
        auto i = 0;
        for (auto &arg : cur_func->args()) {
            arg.setName("__arg" + to_string(i));
            ContextValues["__arg" + to_string(i)] = &arg;
            i++;
        }

        // Visit cases
        case_id = 0;
        for (auto &_case : node.cases) {
            _case->accept(*this);
            case_id++;
        }
    }

void LLVMCodeGenerator::visit(common::Case &node) {

    string case_name = cur_func->getName().str() + "_case" + std::to_string(case_id);



    ctx = PATTERN;
    //ContextValues.clear();
    for (size_t i = 0; i < node.patterns.size(); i++) {
        cur_val = ContextValues["__arg" + to_string(i)];
        node.patterns[i]->accept(*this);
        //ContextValues["__arg" + to_string(i)] = cur_val;
        //auto cond = Builder.CreateICmpEQ(cur_val, ConstantFP::get(getGlobalContext(), APFloat(0.0)), case_name);
    }

    // Create case block
    BasicBlock *block = BasicBlock::Create(getGlobalContext(), case_name, cur_func);
    Builder.SetInsertPoint(block);


    // Generate expression in case block
    ctx = EXPR;
    node.expr->accept(*this);
    cur_val = Builder.CreateRet(cur_val);

    //Builder.CreateStore (Value *Val, Value *Ptr
    //Builder.CreateAlloca(cur_val->getType(), nullptr, "hej");
    //Builder.CreateLoad(cur_val, "hej");

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

    void LLVMCodeGenerator::visit(common::Int &node) {
        cur_val = ConstantInt::get(IntegerType::get(getGlobalContext(), 64), node.value);
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
        switch (ctx)
        {
            case PATTERN:
                ContextValues[node.id] = cur_val;
                break;
            case EXPR:
                cur_val = ContextValues[node.id];
                if (cur_val)
                    return;
                cur_val = Module->getFunction(node.id);
                break;
        }
    }
}
