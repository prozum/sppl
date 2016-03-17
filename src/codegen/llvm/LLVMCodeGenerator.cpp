#include "LLVMCodeGenerator.h"

using namespace llvm;

namespace codegen {
    LLVMCodeGenerator::LLVMCodeGenerator(std::ostream &out)
            : common::CodeGenerator::CodeGenerator(out),
              Builder(getGlobalContext()),
              Module(std::make_unique<llvm::Module>("SpplModule", getGlobalContext())) {}

    void LLVMCodeGenerator::visit(common::Program &node) {
        for (auto &func : node.funcs) {
            func->accept(*this);
        }

        Module->dump();
    }

    llvm::Function *LLVMCodeGenerator::GetFunction()
    {
        return cur_func;
    }

    Type* LLVMCodeGenerator::get_type(common::Types type)
    {
        switch (type)
        {
            case common::Types::FLOAT:
                return Type::getFloatTy(getGlobalContext());
            case common::Types::INT:
                return Type::getInt64Ty(getGlobalContext());
            case common::Types::BOOL:
                // TODO: Implement BOOL
            case common::Types::CHAR:
                // TODO: Implement CHAR
            case common::Types::STRING:
                // TODO: Implement STRING
            default:
                throw "Not supported";
        }
    }

    void LLVMCodeGenerator::visit(common::Function &node) {

        // Get output type
        Type *output_type = get_type(node.types[0]->type);

        // Get input types
        std::vector<Type *> input_types;
        for (unsigned i = 0; i < node.types.size() - 1; i++) {
            input_types.push_back(get_type(node.types[i]->type));
        }

        // Setup function input/output types
        auto func_type = FunctionType::get(output_type, input_types, false);

        // Create function and entry block
        cur_func = Function::Create(func_type, Function::ExternalLinkage, node.id, Module.get());
        BasicBlock *entry = BasicBlock::Create(getGlobalContext(), "entry", cur_func);

        // Create error block
        cur_error_block = BasicBlock::Create(getGlobalContext(), "error", cur_func);
        Builder.SetInsertPoint(cur_error_block);
        Builder.CreateRet(ConstantInt::get(IntegerType::get(getGlobalContext(), 64), 255));

        // Setup return block and phi node
        cur_ret_block = BasicBlock::Create(getGlobalContext(), "ret", cur_func);
        Builder.SetInsertPoint(cur_ret_block);
        cur_phi_node = Builder.CreatePHI(output_type, node.cases.size(), "rettmp");
        Builder.CreateRet(cur_phi_node);

        // Setup names for arguments
        auto i = 0;
        for (auto &arg : cur_func->args()) {
            arg.setName("_arg" + to_string(i++));
            arguments.push_back(&arg);
        }

        // Setup case and pattern blocks
        //CaseBlocks.clear();
        /*for (size_t i = 0; i < node.cases.size(); i++) {
            PatternBlocks[i].clear();
            for (size_t j = 0; j < node.cases[i]->patterns.size() ; j++)
                PatternBlocks[i].push_back(BasicBlock::Create(getGlobalContext(), "case" + to_string(i) + "_pattern" + to_string(j), cur_func));
            CaseBlocks[i] = BasicBlock::Create(getGlobalContext(), "case" + to_string(i), cur_func);
        }*/

        // Visit cases
        cur_case_id = last_case_id = node.cases.size() - 1;
        for (auto &_case : node.cases) {
            _case->accept(*this);
            cur_case_id--;
        }

        Builder.SetInsertPoint(entry);
        Builder.CreateBr(cur_pattern_block);

        verifyFunction(*cur_func);
    }

    Value *LLVMCodeGenerator::compare(Value *val1, Value *val2)
    {
        if (val1->getType()->isFloatTy())
            return Builder.CreateFCmpONE(val1, val2, "cmptmp");
        else if (val1->getType()->isIntegerTy() || val1->getType()->isIntegerTy())
            return Builder.CreateICmpEQ(val1, val2, "cmptmp");
        else
            throw "Not supported!";

    }

void LLVMCodeGenerator::visit(common::Case &node) {

    cur_case_block = BasicBlock::Create(getGlobalContext(), "case" + to_string(cur_case_id), cur_func);

    BasicBlock *true_block;
    BasicBlock *false_block;
    if (cur_case_id == last_case_id)
        false_block = cur_error_block;
    else
        false_block = cur_pattern_block;


    if (node.patterns.size()) {
        ctx = PATTERN;
        ContextValues.clear();
        for (size_t i = node.patterns.size(); i != 0; --i) {

            // Last pattern should branch to next case
            if (i == node.patterns.size())
                true_block = cur_case_block;
            else
                true_block = cur_pattern_block;

            // Create new branch
            cur_pattern_block = BasicBlock::Create(getGlobalContext(),
                                                   "case" + to_string(cur_case_id) + "_pattern" + to_string(i - 1),
                                                   cur_func);
            Builder.SetInsertPoint(cur_pattern_block);

            // Check arguments
            cur_val = arguments[i - 1];
            node.patterns[i - 1]->accept(*this);
            cur_val = compare(cur_val, arguments[i - 1]);

            // Create condition
            Builder.CreateCondBr(cur_val, true_block, false_block);
        }
    }
    else
    {
        cur_pattern_block = BasicBlock::Create(getGlobalContext(),
                                               "case" + to_string(cur_case_id) + "_pattern" + to_string(0),
                                               cur_func);
        Builder.SetInsertPoint(cur_pattern_block);
        Builder.CreateBr(cur_case_block);
    }

    // Generate expression in case block
    Builder.SetInsertPoint(cur_case_block);
    ctx = EXPR;
    node.expr->accept(*this);

    // Add return value to phi node
    cur_phi_node->addIncoming(cur_val, cur_case_block);
    Builder.CreateBr(cur_ret_block);
}

    void LLVMCodeGenerator::visit(common::Add &node) {
        node.left->accept(*this);
        auto left = cur_val;
        node.right->accept(*this);
        auto right = cur_val;

        if ( node.left->node_type->type == common::Types::FLOAT && node.right->node_type->type == common::Types::FLOAT) {
            cur_val = Builder.CreateFAdd(left, right, "addtmp");
        } else {
            cur_val = Builder.CreateAdd(left, right, "addtmp");
        }
    }

    void LLVMCodeGenerator::visit(common::Sub &node) {
        node.left->accept(*this);
        auto left = cur_val;
        node.right->accept(*this);
        auto right = cur_val;

        if ( node.left->node_type->type == common::Types::FLOAT && node.right->node_type->type == common::Types::FLOAT) {
            cur_val = Builder.CreateFSub(left, right, "subtmp");
        } else {
            cur_val = Builder.CreateSub(left, right, "subtmp");
        }
    }

    void LLVMCodeGenerator::visit(common::Mul &node) {
        node.left->accept(*this);
        auto left = cur_val;
        node.right->accept(*this);
        auto right = cur_val;

        if ( node.left->node_type->type == common::Types::FLOAT && node.right->node_type->type == common::Types::FLOAT) {
            cur_val = Builder.CreateFMul(left, right, "multmp");
        } else {
            cur_val = Builder.CreateMul(left, right, "multmp");
        }
    }

    void LLVMCodeGenerator::visit(common::Div &node) {
        node.left->accept(*this);
        auto left = cur_val;
        node.right->accept(*this);
        auto right = cur_val;

        if ( node.left->node_type->type == common::Types::FLOAT && node.right->node_type->type == common::Types::FLOAT) {
            cur_val = Builder.CreateFDiv(left, right, "divtmp");
        } else {
            cur_val = Builder.CreateSDiv(left, right, "divtmp");
        }
    }

    void LLVMCodeGenerator::visit(common::Mod &node) {
        node.left->accept(*this);
        auto left = cur_val;
        node.right->accept(*this);
        auto right = cur_val;

        if ( node.left->node_type->type == common::Types::FLOAT && node.right->node_type->type == common::Types::FLOAT) {
            cur_val = Builder.CreateFRem(left, right, "modtmp");
        } else {
            cur_val = Builder.CreateSRem(left, right, "modtmp");
        }
    }

    void LLVMCodeGenerator::visit(common::Float &node) {
        cur_val = ConstantFP::get(getGlobalContext(), APFloat(node.value));
    }

    void LLVMCodeGenerator::visit(common::Int &node) {
        cur_val = ConstantInt::get(IntegerType::get(getGlobalContext(), 64), node.value);
    }

    void LLVMCodeGenerator::visit(common::Bool &node) {
        cur_val = ConstantInt::get(IntegerType::get(getGlobalContext(), 64), node.value);   // TODO: Better way?
    }

    void LLVMCodeGenerator::visit(common::Char &node) {
        cur_val = ConstantInt::get(IntegerType::get(getGlobalContext(), 64), node.value);   // TODO: Better way?
    }

    void LLVMCodeGenerator::visit(common::String &node) {
        cur_val = ConstantDataArray::getString(getGlobalContext(), node.value, true);       // TODO: Correct way?
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
