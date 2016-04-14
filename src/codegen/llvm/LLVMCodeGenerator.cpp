#include "LLVMCodeGenerator.h"

using namespace llvm;

namespace codegen {
    LLVMCodeGenerator::LLVMCodeGenerator(parser::Driver &driver)
            : CodeGenerator(driver),
              Builder(getGlobalContext()),
              Module(std::make_unique<llvm::Module>("SpplModule", getGlobalContext())) {}

    void LLVMCodeGenerator::visit(common::Program &node) {
        for (auto &func : node.funcs) {
            func->accept(*this);
        }

        driver.codeout << ModuleString();
    }

    llvm::Type *LLVMCodeGenerator::get_type(common::Type type, bool ptr)
    {
        llvm::Type *new_type;

        switch (type.id)
        {
            case common::TypeId::FLOAT:
                new_type = llvm::Type::getDoubleTy(getGlobalContext());
                break;
            case common::TypeId::INT:
                new_type = llvm::Type::getInt64Ty(getGlobalContext());
                break;
            case common::TypeId::BOOL:
                new_type = llvm::Type::getInt1Ty(getGlobalContext());
                break;
            case common::TypeId::CHAR:
                new_type = llvm::Type::getInt8Ty(getGlobalContext());
                break;
            case common::TypeId::STRING:
                return llvm::Type::getInt8PtrTy(getGlobalContext());
            case common::TypeId::TUPLE:
                return PointerType::getUnqual(get_tuple_type(type));
            case common::TypeId::LIST:
                return PointerType::getUnqual(get_list_type(type));
            case common::TypeId::SIGNATURE:
                return PointerType::getUnqual(get_func_type(type));
            default:
                throw "Not supported";
        }

        if (ptr)
            return PointerType::getUnqual(new_type);
        else
            return new_type;
    }

    llvm::StructType *LLVMCodeGenerator::get_tuple_type(common::Type type)
    {
        auto old_type = tuple_types.find(type);

        if (old_type != tuple_types.end())
            return old_type->second;
        
        std::vector<llvm::Type *> tmp_vec;
        for (auto &subtype: type.subtypes)
            tmp_vec.push_back(get_type(subtype));
        llvm::ArrayRef<llvm::Type *> subtypes(tmp_vec);

        auto new_type = StructType::create(getGlobalContext(), subtypes);
        tuple_types[type] = new_type;

        return new_type;
    }

    llvm::StructType *LLVMCodeGenerator::get_list_type(common::Type type)
    {
        auto old_type = list_types.find(type);

        if (old_type != list_types.end())
            return old_type->second;

        vector<llvm::Type *> tmp_vec = { get_type(type.subtypes[0], true), llvm::Type::getInt64Ty(getGlobalContext()) };
        llvm::ArrayRef<llvm::Type *> subtypes(tmp_vec);

        auto new_type = StructType::create(getGlobalContext(), subtypes);
        list_types[type] = new_type;

        return new_type;
    }

    llvm::FunctionType *LLVMCodeGenerator::get_func_type(common::Type type)
    {
        auto old_type = func_types.find(type);

        if (old_type != func_types.end())
            return old_type->second;

        auto output_type = get_type(type.subtypes[0]);
        std::vector<llvm::Type *> input_types;
        for (size_t i = 1; i < type.subtypes.size(); i++) {
            input_types.push_back(get_type(type.subtypes[i]));
        }

        auto new_type = FunctionType::get(output_type, input_types, false);
        func_types[type] = new_type;

        return new_type;
    }

    void LLVMCodeGenerator::visit(common::Function &node) {

        // Create function and entry block
        cur_func = llvm::Function::Create(get_func_type(node.signature), llvm::Function::ExternalLinkage, node.id, Module.get());
        BasicBlock *entry = BasicBlock::Create(getGlobalContext(), "entry", cur_func);

        // Create error block
        cur_error_block = BasicBlock::Create(getGlobalContext(), "error", cur_func);
        Builder.SetInsertPoint(cur_error_block);
        Builder.CreateRet(ConstantInt::get(IntegerType::get(getGlobalContext(), 64), 255));

        // Setup return block and phi node
        cur_ret_block = BasicBlock::Create(getGlobalContext(), "ret", cur_func);
        Builder.SetInsertPoint(cur_ret_block);
        cur_phi_node = Builder.CreatePHI(cur_func->getReturnType(), node.cases.size(), "rettmp");
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

        // Make entry point to first pattern
        Builder.SetInsertPoint(entry);
        Builder.CreateBr(cur_pattern_block);

        verifyFunction(*cur_func);
        Functions[node.id] = move(cur_func);
    }

    Value *LLVMCodeGenerator::compare(Value *val1, Value *val2)
    {
        if (val1->getType()->isFloatTy())
            return Builder.CreateFCmpONE(val1, val2, "cmptmp");
        else if (val1->getType()->isIntegerTy())
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

        if ( node.left->type.id == common::TypeId::FLOAT && node.right->type.id == common::TypeId::FLOAT) {
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

        if ( node.left->type.id == common::TypeId::FLOAT && node.right->type.id == common::TypeId::FLOAT) {
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

        if ( node.left->type.id == common::TypeId::FLOAT && node.right->type.id == common::TypeId::FLOAT) {
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

        if ( node.left->type.id == common::TypeId::FLOAT && node.right->type.id == common::TypeId::FLOAT) {
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

        if ( node.left->type.id == common::TypeId::FLOAT && node.right->type.id == common::TypeId::FLOAT) {
            cur_val = Builder.CreateFRem(left, right, "modtmp");
        } else {
            cur_val = Builder.CreateSRem(left, right, "modtmp");
        }
    }

    void LLVMCodeGenerator::visit(common::Equal &node) {
        node.left->accept(*this);
        auto left = cur_val;
        node.right->accept(*this);
        auto right = cur_val;

        if ( node.left->type.id == common::TypeId::FLOAT && node.right->type.id == common::TypeId::FLOAT) {
            cur_val = Builder.CreateFCmpOEQ(left, right, "eqtmp");
        } else {
            cur_val = Builder.CreateICmpEQ(left, right, "eqtmp");
        }
    }

    void LLVMCodeGenerator::visit(common::NotEqual &node) {
        node.left->accept(*this);
        auto left = cur_val;
        node.right->accept(*this);
        auto right = cur_val;

        if ( node.left->type.id == common::TypeId::FLOAT && node.right->type.id == common::TypeId::FLOAT) {
            cur_val = Builder.CreateFCmpONE(left, right, "neqtmp");
        } else {
            cur_val = Builder.CreateICmpNE(left, right, "neqtmp");
        }
    }

    void LLVMCodeGenerator::visit(common::Lesser &node) {
        node.left->accept(*this);
        auto left = cur_val;
        node.right->accept(*this);
        auto right = cur_val;

        if ( node.left->type.id == common::TypeId::FLOAT && node.right->type.id == common::TypeId::FLOAT) {
            cur_val = Builder.CreateFCmpOLT(left, right, "lttmp");
        } else {
            cur_val = Builder.CreateICmpSLT(left, right, "lttmp");
        }
    }

    void LLVMCodeGenerator::visit(common::Greater &node) {
        node.left->accept(*this);
        auto left = cur_val;
        node.right->accept(*this);
        auto right = cur_val;

        if ( node.left->type.id == common::TypeId::FLOAT && node.right->type.id == common::TypeId::FLOAT) {
            cur_val = Builder.CreateFCmpOGT(left, right, "lttmp");
        } else {
            cur_val = Builder.CreateICmpSGT(left, right, "lttmp");
        }
    }

    void LLVMCodeGenerator::visit(common::LesserEq &node) {
        node.left->accept(*this);
        auto left = cur_val;
        node.right->accept(*this);
        auto right = cur_val;

        if ( node.left->type.id == common::TypeId::FLOAT && node.right->type.id == common::TypeId::FLOAT) {
            cur_val = Builder.CreateFCmpOLE(left, right, "lttmp");
        } else {
            cur_val = Builder.CreateICmpSLE(left, right, "lttmp");
        }
    }

    void LLVMCodeGenerator::visit(common::GreaterEq &node) {
        node.left->accept(*this);
        auto left = cur_val;
        node.right->accept(*this);
        auto right = cur_val;

        if ( node.left->type.id == common::TypeId::FLOAT && node.right->type.id == common::TypeId::FLOAT) {
            cur_val = Builder.CreateFCmpOGE(left, right, "lttmp");
        } else {
            cur_val = Builder.CreateICmpSGE(left, right, "lttmp");
        }
    }

    void LLVMCodeGenerator::visit(common::Float &node) {
        cur_val = ConstantFP::get(get_type(node.type), node.value);
    }

    void LLVMCodeGenerator::visit(common::Int &node) {
        cur_val = ConstantInt::get(get_type(node.type), node.value);
    }

    void LLVMCodeGenerator::visit(common::Bool &node) {
        cur_val = ConstantInt::get(get_type(node.type), node.value);
    }

    void LLVMCodeGenerator::visit(common::Char &node) {
        cur_val = ConstantInt::get(get_type(node.type), node.value);
    }

    void LLVMCodeGenerator::visit(common::String &node) {
        cur_val = Builder.CreateGlobalString(node.value, cur_func->getName() + ".str");

    }

    void LLVMCodeGenerator::visit(common::Call &node) {
        node.callee->accept(*this);
        auto callee = cur_val;

        std::vector<Value *> args;
        for (auto &arg : node.exprs) {
            arg->accept(*this);
            args.push_back(cur_val);
        }

        auto type = callee->getType();
        cur_val = Builder.CreateCall(callee, args, "calltmp");
    }

    void LLVMCodeGenerator::visit(common::Id &node) {
        switch (ctx)
        {
            case PATTERN:
                ContextValues[node.id] = cur_val;
                break;
            case EXPR:
                // Pattern value
                cur_val = ContextValues[node.id];
                if (cur_val)
                    return;

                // Current module
                cur_val = Module->getFunction(node.id);
                if (cur_val)
                    return;

                // External module
                if (driver.global->decls.count(node.id))
                    cur_val = llvm::Function::Create(get_func_type(driver.global->decls[node.id]), llvm::Function::ExternalLinkage, node.id, Module.get());

                // TODO ERROR NOT FOUND
                break;
        }
    }


    void LLVMCodeGenerator::visit(common::Par &node) {
        node.child->accept(*this);
    }

    void LLVMCodeGenerator::visit(common::Tuple &node) {
        std::vector<llvm::Constant *> tmp;

        for (auto &expr: node.exprs) {
            expr->accept(*this);
            tmp.push_back((Constant *)cur_val);
        }

        ArrayRef<Constant *> tuple_val(tmp);

        auto const_val = ConstantStruct::get(get_tuple_type(node.type), tuple_val);
        cur_val = new GlobalVariable(*Module.get(), const_val->getType(), true, GlobalVariable::ExternalLinkage, const_val);

    }

    void LLVMCodeGenerator::visit(common::List &node) {
        std::vector<llvm::Constant *> tmp;

        for (auto &expr: node.exprs) {
            expr->accept(*this);
            tmp.push_back(dynamic_cast<Constant *>(cur_val));
        }

        ArrayRef<Constant *> list_data(tmp);

        auto list_type = ArrayType::get(get_type(node.type), node.exprs.size());
        auto const_val = ConstantArray::get(list_type, list_data);

        cur_val = new GlobalVariable(*Module.get(), const_val->getType(), true, GlobalVariable::ExternalLinkage, const_val);
    }

    string LLVMCodeGenerator::ModuleString() {
        string module_str;
        raw_string_ostream out(module_str);
        out << *Module.get();
        return out.str();
    }
}
