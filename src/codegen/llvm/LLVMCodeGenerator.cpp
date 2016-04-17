#include "LLVMCodeGenerator.h"

using namespace llvm;

namespace codegen {
    LLVMCodeGenerator::LLVMCodeGenerator(parser::Driver &driver)
            : CodeGenerator(driver),
              Builder(getGlobalContext()),
              Module(std::make_unique<llvm::Module>("SpplModule", getGlobalContext())) {}

    void LLVMCodeGenerator::visit(common::Program &node) {
        for (auto &func : node.Funcs) {
            func->accept(*this);
        }

        *Drv.Out << ModuleString();
    }

    llvm::Type *LLVMCodeGenerator::getType(common::Type type, bool ptr)
    {
        llvm::Type *new_type;

        switch (type.Id)
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
                return PointerType::getUnqual(getTupleType(type));
            case common::TypeId::LIST:
                return PointerType::getUnqual(getListType(type));
            case common::TypeId::SIGNATURE:
                return PointerType::getUnqual(getFuncType(type));
            default:
                throw runtime_error("This should not happen!");
        }

        if (ptr)
            return PointerType::getUnqual(new_type);
        else
            return new_type;
    }

    llvm::StructType *LLVMCodeGenerator::getTupleType(common::Type type)
    {
        auto old_type = TupleTypes.find(type);

        if (old_type != TupleTypes.end())
            return old_type->second;
        
        std::vector<llvm::Type *> tmp_vec;
        for (auto &subtype: type.Subtypes)
            tmp_vec.push_back(getType(subtype));
        llvm::ArrayRef<llvm::Type *> subtypes(tmp_vec);

        auto new_type = StructType::create(getGlobalContext(), subtypes);
        TupleTypes[type] = new_type;

        return new_type;
    }

    llvm::StructType *LLVMCodeGenerator::getListType(common::Type type)
    {
        auto old_type = ListTypes.find(type);

        if (old_type != ListTypes.end())
            return old_type->second;

        vector<llvm::Type *> tmp_vec = {getType(type.Subtypes[0], true), llvm::Type::getInt64Ty(getGlobalContext()) };
        llvm::ArrayRef<llvm::Type *> subtypes(tmp_vec);

        auto new_type = StructType::create(getGlobalContext(), subtypes);
        ListTypes[type] = new_type;

        return new_type;
    }

    llvm::FunctionType *LLVMCodeGenerator::getFuncType(common::Type type)
    {
        auto old_type = FuncTypes.find(type);

        if (old_type != FuncTypes.end())
            return old_type->second;

        auto output_type = getType(type.Subtypes.back());
        std::vector<llvm::Type *> input_types;
        for (size_t i = 0; i < type.Subtypes.size() - 1; i++) {
            input_types.push_back(getType(type.Subtypes[i]));
        }

        auto new_type = FunctionType::get(output_type, input_types, false);
        FuncTypes[type] = new_type;

        return new_type;
    }

    void LLVMCodeGenerator::visit(common::Function &node) {

        // Create function and entry block
        CurFunc = llvm::Function::Create(getFuncType(node.Signature), llvm::Function::ExternalLinkage, node.Id, Module.get());
        BasicBlock *entry = BasicBlock::Create(getGlobalContext(), "entry", CurFunc);

        // Create error block
        CurErrorBlock = BasicBlock::Create(getGlobalContext(), "error", CurFunc);
        Builder.SetInsertPoint(CurErrorBlock);
        Builder.CreateRet(ConstantInt::get(IntegerType::get(getGlobalContext(), 64), 255));

        // Setup return block and phi node
        CurRetBlock = BasicBlock::Create(getGlobalContext(), "ret", CurFunc);
        Builder.SetInsertPoint(CurRetBlock);
        CurPhiNode = Builder.CreatePHI(CurFunc->getReturnType(), node.Cases.size(), "rettmp");
        Builder.CreateRet(CurPhiNode);

        // Setup names for arguments
        auto i = 0;
        for (auto &arg : CurFunc->args()) {
            arg.setName("_arg" + to_string(i++));
            Arguments.push_back(&arg);
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
        CurCaseId = LastCaseId = node.Cases.size() - 1;
        for (auto &_case : node.Cases) {
            _case->accept(*this);
            CurCaseId--;
        }

        // Make entry point to first pattern
        Builder.SetInsertPoint(entry);
        Builder.CreateBr(CurPatternBlock);

        verifyFunction(*CurFunc);
        Functions[node.Id] = move(CurFunc);
    }

    Value *LLVMCodeGenerator::compare(Value *val1, Value *val2)
    {
        if (val1->getType()->isFloatTy())
            return Builder.CreateFCmpONE(val1, val2, "cmptmp");
        else if (val1->getType()->isIntegerTy())
            return Builder.CreateICmpEQ(val1, val2, "cmptmp");
        else
            throw runtime_error("This should not happen!");
    }

    void LLVMCodeGenerator::visit(common::Case &node) {

        CurCaseBlock = BasicBlock::Create(getGlobalContext(), "case" + to_string(CurCaseId), CurFunc);

        BasicBlock *true_block;
        BasicBlock *false_block;
        if (CurCaseId == LastCaseId)
            false_block = CurErrorBlock;
        else
            false_block = CurPatternBlock;


        if (node.Patterns.size()) {
            Ctx = PATTERN;
            ContextValues.clear();
            for (size_t i = node.Patterns.size(); i != 0; --i) {

                // Last pattern should branch to next case
                if (i == node.Patterns.size())
                    true_block = CurCaseBlock;
                else
                    true_block = CurPatternBlock;

                // Create new branch
                CurPatternBlock = BasicBlock::Create(getGlobalContext(),
                                                       "case" + to_string(CurCaseId) + "_pattern" + to_string(i - 1),
                                                       CurFunc);
                Builder.SetInsertPoint(CurPatternBlock);

                // Check arguments
                CurVal = Arguments[i - 1];
                node.Patterns[i - 1]->accept(*this);
                CurVal = compare(CurVal, Arguments[i - 1]);

                // Create condition
                Builder.CreateCondBr(CurVal, true_block, false_block);
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
        Ctx = EXPR;
        node.Expr->accept(*this);

        // Add return value to phi node
        CurPhiNode->addIncoming(CurVal, CurCaseBlock);
        Builder.CreateBr(CurRetBlock);
    }

    void LLVMCodeGenerator::visit(common::Add &node) {
        node.Left->accept(*this);
        auto left = CurVal;
        node.Right->accept(*this);
        auto right = CurVal;

        if ( node.Left->Ty.Id == common::TypeId::FLOAT && node.Right->Ty.Id == common::TypeId::FLOAT) {
            CurVal = Builder.CreateFAdd(left, right, "addtmp");
        } else {
            CurVal = Builder.CreateAdd(left, right, "addtmp");
        }
    }

    void LLVMCodeGenerator::visit(common::Sub &node) {
        node.Left->accept(*this);
        auto left = CurVal;
        node.Right->accept(*this);
        auto right = CurVal;

        if ( node.Left->Ty.Id == common::TypeId::FLOAT && node.Right->Ty.Id == common::TypeId::FLOAT) {
            CurVal = Builder.CreateFSub(left, right, "subtmp");
        } else {
            CurVal = Builder.CreateSub(left, right, "subtmp");
        }
    }

    void LLVMCodeGenerator::visit(common::Mul &node) {
        node.Left->accept(*this);
        auto left = CurVal;
        node.Right->accept(*this);
        auto right = CurVal;

        if ( node.Left->Ty.Id == common::TypeId::FLOAT && node.Right->Ty.Id == common::TypeId::FLOAT) {
            CurVal = Builder.CreateFMul(left, right, "multmp");
        } else {
            CurVal = Builder.CreateMul(left, right, "multmp");
        }
    }

    void LLVMCodeGenerator::visit(common::Div &node) {
        node.Left->accept(*this);
        auto left = CurVal;
        node.Right->accept(*this);
        auto right = CurVal;

        if ( node.Left->Ty.Id == common::TypeId::FLOAT && node.Right->Ty.Id == common::TypeId::FLOAT) {
            CurVal = Builder.CreateFDiv(left, right, "divtmp");
        } else {
            CurVal = Builder.CreateSDiv(left, right, "divtmp");
        }
    }

    void LLVMCodeGenerator::visit(common::Mod &node) {
        node.Left->accept(*this);
        auto left = CurVal;
        node.Right->accept(*this);
        auto right = CurVal;

        if ( node.Left->Ty.Id == common::TypeId::FLOAT && node.Right->Ty.Id == common::TypeId::FLOAT) {
            CurVal = Builder.CreateFRem(left, right, "modtmp");
        } else {
            CurVal = Builder.CreateSRem(left, right, "modtmp");
        }
    }

    void LLVMCodeGenerator::visit(common::Equal &node) {
        node.Left->accept(*this);
        auto left = CurVal;
        node.Right->accept(*this);
        auto right = CurVal;

        if ( node.Left->Ty.Id == common::TypeId::FLOAT && node.Right->Ty.Id == common::TypeId::FLOAT) {
            CurVal = Builder.CreateFCmpOEQ(left, right, "eqtmp");
        } else {
            CurVal = Builder.CreateICmpEQ(left, right, "eqtmp");
        }
    }

    void LLVMCodeGenerator::visit(common::NotEqual &node) {
        node.Left->accept(*this);
        auto left = CurVal;
        node.Right->accept(*this);
        auto right = CurVal;

        if ( node.Left->Ty.Id == common::TypeId::FLOAT && node.Right->Ty.Id == common::TypeId::FLOAT) {
            CurVal = Builder.CreateFCmpONE(left, right, "neqtmp");
        } else {
            CurVal = Builder.CreateICmpNE(left, right, "neqtmp");
        }
    }

    void LLVMCodeGenerator::visit(common::Lesser &node) {
        node.Left->accept(*this);
        auto left = CurVal;
        node.Right->accept(*this);
        auto right = CurVal;

        if ( node.Left->Ty.Id == common::TypeId::FLOAT && node.Right->Ty.Id == common::TypeId::FLOAT) {
            CurVal = Builder.CreateFCmpOLT(left, right, "lttmp");
        } else {
            CurVal = Builder.CreateICmpSLT(left, right, "lttmp");
        }
    }

    void LLVMCodeGenerator::visit(common::Greater &node) {
        node.Left->accept(*this);
        auto left = CurVal;
        node.Right->accept(*this);
        auto right = CurVal;

        if ( node.Left->Ty.Id == common::TypeId::FLOAT && node.Right->Ty.Id == common::TypeId::FLOAT) {
            CurVal = Builder.CreateFCmpOGT(left, right, "lttmp");
        } else {
            CurVal = Builder.CreateICmpSGT(left, right, "lttmp");
        }
    }

    void LLVMCodeGenerator::visit(common::LesserEq &node) {
        node.Left->accept(*this);
        auto left = CurVal;
        node.Right->accept(*this);
        auto right = CurVal;

        if ( node.Left->Ty.Id == common::TypeId::FLOAT && node.Right->Ty.Id == common::TypeId::FLOAT) {
            CurVal = Builder.CreateFCmpOLE(left, right, "lttmp");
        } else {
            CurVal = Builder.CreateICmpSLE(left, right, "lttmp");
        }
    }

    void LLVMCodeGenerator::visit(common::GreaterEq &node) {
        node.Left->accept(*this);
        auto left = CurVal;
        node.Right->accept(*this);
        auto right = CurVal;

        if ( node.Left->Ty.Id == common::TypeId::FLOAT && node.Right->Ty.Id == common::TypeId::FLOAT) {
            CurVal = Builder.CreateFCmpOGE(left, right, "lttmp");
        } else {
            CurVal = Builder.CreateICmpSGE(left, right, "lttmp");
        }
    }

    void LLVMCodeGenerator::visit(common::Float &node) {
        CurVal = ConstantFP::get(getType(node.Ty), node.Val);
    }

    void LLVMCodeGenerator::visit(common::Int &node) {
        CurVal = ConstantInt::get(getType(node.Ty), node.Val);
    }

    void LLVMCodeGenerator::visit(common::Bool &node) {
        CurVal = ConstantInt::get(getType(node.Ty), node.Val);
    }

    void LLVMCodeGenerator::visit(common::Char &node) {
        CurVal = ConstantInt::get(getType(node.Ty), node.Val);
    }

    void LLVMCodeGenerator::visit(common::String &node) {
        CurVal = Builder.CreateGlobalString(node.Val, CurFunc->getName() + ".str");

    }

    void LLVMCodeGenerator::visit(common::Call &node) {
        node.Callee->accept(*this);
        auto callee = CurVal;

        std::vector<Value *> args;
        for (auto &arg : node.Args) {
            arg->accept(*this);
            args.push_back(CurVal);
        }

        CurVal = Builder.CreateCall(callee, args, "calltmp");
    }

    void LLVMCodeGenerator::visit(common::Id &node) {
        switch (Ctx)
        {
            case PATTERN:
                ContextValues[node.Val] = CurVal;
                break;
            case EXPR:
                // Pattern value
                CurVal = ContextValues[node.Val];
                if (CurVal)
                    return;

                // Current module
                CurVal = Module->getFunction(node.Val);
                if (CurVal)
                    return;

                // External module
                if (Drv.Global.Decls.count(node.Val))
                    CurVal = llvm::Function::Create(getFuncType(Drv.Global.Decls[node.Val]), llvm::Function::ExternalLinkage, node.Val, Module.get());

                // TODO ERROR NOT FOUND
                break;
        }
    }


    void LLVMCodeGenerator::visit(common::Par &node) {
        node.Child->accept(*this);
    }

    void LLVMCodeGenerator::visit(common::Tuple &node) {
        std::vector<llvm::Constant *> tmp;

        for (auto &expr: node.Elements) {
            expr->accept(*this);
            tmp.push_back((Constant *)CurVal);
        }

        ArrayRef<Constant *> tuple_val(tmp);

        auto const_val = ConstantStruct::get(getTupleType(node.Ty), tuple_val);
        CurVal = new GlobalVariable(*Module.get(), const_val->getType(), true, GlobalVariable::ExternalLinkage, const_val);

    }

    void LLVMCodeGenerator::visit(common::List &node) {
        std::vector<llvm::Constant *> tmp;

        for (auto &expr: node.Elements) {
            expr->accept(*this);
            tmp.push_back(dynamic_cast<Constant *>(CurVal));
        }

        ArrayRef<Constant *> list_data(tmp);

        auto list_type = ArrayType::get(getType(node.Ty), node.Elements.size());
        auto const_val = ConstantArray::get(list_type, list_data);

        CurVal = new GlobalVariable(*Module.get(), const_val->getType(), true, GlobalVariable::ExternalLinkage, const_val);
    }

    string LLVMCodeGenerator::ModuleString() {
        string module_str;
        raw_string_ostream out(module_str);
        out << *Module.get();
        return out.str();
    }
}
