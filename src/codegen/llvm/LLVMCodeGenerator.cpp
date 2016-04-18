#include "LLVMCodeGenerator.h"

using namespace llvm;

namespace codegen {
    LLVMCodeGenerator::LLVMCodeGenerator(parser::Driver &driver)
            : CodeGenerator(driver),
              Builder(getGlobalContext()),
              Module(std::make_unique<llvm::Module>("SpplModule", getGlobalContext())) {}

    void LLVMCodeGenerator::visit(common::Program &node) {
        for (auto &func : node.Decls) {
            func->accept(*this);
        }

        *Drv.Out << ModuleString();
    }

    llvm::Type *LLVMCodeGenerator::getType(common::Type Ty, bool Ptr)
    {
        llvm::Type *new_type;

        switch (Ty.Id)
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
                return PointerType::getUnqual(getTupleType(Ty));
            case common::TypeId::LIST:
                return PointerType::getUnqual(getListType(Ty));
            case common::TypeId::SIGNATURE:
                return PointerType::getUnqual(getFuncType(Ty));
            default:
                throw runtime_error("This should not happen!");
        }

        if (Ptr)
            return PointerType::getUnqual(new_type);
        else
            return new_type;
    }

    llvm::StructType *LLVMCodeGenerator::getTupleType(common::Type Ty)
    {
        auto OldType = TupleTypes.find(Ty);

        if (OldType != TupleTypes.end())
            return OldType->second;
        
        std::vector<llvm::Type *> TmpVec;
        for (auto &Subtype : Ty.Subtypes)
            TmpVec.push_back(getType(Subtype));
        llvm::ArrayRef<llvm::Type *> Subtypes(TmpVec);

        auto NewType = StructType::create(getGlobalContext(), Subtypes);
        TupleTypes[Ty] = NewType;

        return NewType;
    }

    llvm::StructType *LLVMCodeGenerator::getListType(common::Type Ty)
    {
        auto OldType = ListTypes.find(Ty);

        if (OldType != ListTypes.end())
            return OldType->second;

        vector<llvm::Type *> TmpVec = {getType(Ty.Subtypes[0], true), llvm::Type::getInt64Ty(getGlobalContext()) };
        llvm::ArrayRef<llvm::Type *> Subtypes(TmpVec);

        auto NewType = StructType::create(getGlobalContext(), Subtypes);
        ListTypes[Ty] = NewType;

        return NewType;
    }

    llvm::FunctionType *LLVMCodeGenerator::getFuncType(common::Type Ty)
    {
        auto OldType = FuncTypes.find(Ty);

        if (OldType != FuncTypes.end())
            return OldType->second;

        auto OutputType = getType(Ty.Subtypes.back());
        std::vector<llvm::Type *> InputTypes;
        for (size_t i = 0; i < Ty.Subtypes.size() - 1; ++i) {
            InputTypes.push_back(getType(Ty.Subtypes[i]));
        }

        auto NewType = FunctionType::get(OutputType, InputTypes, false);
        FuncTypes[Ty] = NewType;

        return NewType;
    }

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
        CurPhiNode = Builder.CreatePHI(CurFunc->getReturnType(), (unsigned int)Node.Cases.size(), "rettmp");
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
        Functions[Node.Id] = move(CurFunc);
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
            Ctx = PATTERN;
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
        Ctx = EXPR;
        Node.Expr->accept(*this);

        // Add return value to phi node
        CurPhiNode->addIncoming(CurVal, CurCaseBlock);
        Builder.CreateBr(CurRetBlock);
    }

    void LLVMCodeGenerator::visit(common::Add &Node) {
        Node.Left->accept(*this);
        auto left = CurVal;
        Node.Right->accept(*this);
        auto right = CurVal;

        if ( Node.Left->Ty.Id == common::TypeId::FLOAT && Node.Right->Ty.Id == common::TypeId::FLOAT) {
            CurVal = Builder.CreateFAdd(left, right, "addtmp");
        } else {
            CurVal = Builder.CreateAdd(left, right, "addtmp");
        }
    }

    void LLVMCodeGenerator::visit(common::Sub &Node) {
        Node.Left->accept(*this);
        auto left = CurVal;
        Node.Right->accept(*this);
        auto right = CurVal;

        if ( Node.Left->Ty.Id == common::TypeId::FLOAT && Node.Right->Ty.Id == common::TypeId::FLOAT) {
            CurVal = Builder.CreateFSub(left, right, "subtmp");
        } else {
            CurVal = Builder.CreateSub(left, right, "subtmp");
        }
    }

    void LLVMCodeGenerator::visit(common::Mul &Node) {
        Node.Left->accept(*this);
        auto left = CurVal;
        Node.Right->accept(*this);
        auto right = CurVal;

        if ( Node.Left->Ty.Id == common::TypeId::FLOAT && Node.Right->Ty.Id == common::TypeId::FLOAT) {
            CurVal = Builder.CreateFMul(left, right, "multmp");
        } else {
            CurVal = Builder.CreateMul(left, right, "multmp");
        }
    }

    void LLVMCodeGenerator::visit(common::Div &Node) {
        Node.Left->accept(*this);
        auto left = CurVal;
        Node.Right->accept(*this);
        auto right = CurVal;

        if ( Node.Left->Ty.Id == common::TypeId::FLOAT && Node.Right->Ty.Id == common::TypeId::FLOAT) {
            CurVal = Builder.CreateFDiv(left, right, "divtmp");
        } else {
            CurVal = Builder.CreateSDiv(left, right, "divtmp");
        }
    }

    void LLVMCodeGenerator::visit(common::Mod &Node) {
        Node.Left->accept(*this);
        auto left = CurVal;
        Node.Right->accept(*this);
        auto right = CurVal;

        if ( Node.Left->Ty.Id == common::TypeId::FLOAT && Node.Right->Ty.Id == common::TypeId::FLOAT) {
            CurVal = Builder.CreateFRem(left, right, "modtmp");
        } else {
            CurVal = Builder.CreateSRem(left, right, "modtmp");
        }
    }

    void LLVMCodeGenerator::visit(common::Equal &Node) {
        Node.Left->accept(*this);
        auto left = CurVal;
        Node.Right->accept(*this);
        auto right = CurVal;

        if ( Node.Left->Ty.Id == common::TypeId::FLOAT && Node.Right->Ty.Id == common::TypeId::FLOAT) {
            CurVal = Builder.CreateFCmpOEQ(left, right, "eqtmp");
        } else {
            CurVal = Builder.CreateICmpEQ(left, right, "eqtmp");
        }
    }

    void LLVMCodeGenerator::visit(common::NotEqual &Node) {
        Node.Left->accept(*this);
        auto left = CurVal;
        Node.Right->accept(*this);
        auto right = CurVal;

        if ( Node.Left->Ty.Id == common::TypeId::FLOAT && Node.Right->Ty.Id == common::TypeId::FLOAT) {
            CurVal = Builder.CreateFCmpONE(left, right, "neqtmp");
        } else {
            CurVal = Builder.CreateICmpNE(left, right, "neqtmp");
        }
    }

    void LLVMCodeGenerator::visit(common::Lesser &Node) {
        Node.Left->accept(*this);
        auto left = CurVal;
        Node.Right->accept(*this);
        auto right = CurVal;

        if ( Node.Left->Ty.Id == common::TypeId::FLOAT && Node.Right->Ty.Id == common::TypeId::FLOAT) {
            CurVal = Builder.CreateFCmpOLT(left, right, "lttmp");
        } else {
            CurVal = Builder.CreateICmpSLT(left, right, "lttmp");
        }
    }

    void LLVMCodeGenerator::visit(common::Greater &Node) {
        Node.Left->accept(*this);
        auto left = CurVal;
        Node.Right->accept(*this);
        auto right = CurVal;

        if ( Node.Left->Ty.Id == common::TypeId::FLOAT && Node.Right->Ty.Id == common::TypeId::FLOAT) {
            CurVal = Builder.CreateFCmpOGT(left, right, "lttmp");
        } else {
            CurVal = Builder.CreateICmpSGT(left, right, "lttmp");
        }
    }

    void LLVMCodeGenerator::visit(common::LesserEq &Node) {
        Node.Left->accept(*this);
        auto left = CurVal;
        Node.Right->accept(*this);
        auto right = CurVal;

        if ( Node.Left->Ty.Id == common::TypeId::FLOAT && Node.Right->Ty.Id == common::TypeId::FLOAT) {
            CurVal = Builder.CreateFCmpOLE(left, right, "lttmp");
        } else {
            CurVal = Builder.CreateICmpSLE(left, right, "lttmp");
        }
    }

    void LLVMCodeGenerator::visit(common::GreaterEq &Node) {
        Node.Left->accept(*this);
        auto left = CurVal;
        Node.Right->accept(*this);
        auto right = CurVal;

        if ( Node.Left->Ty.Id == common::TypeId::FLOAT && Node.Right->Ty.Id == common::TypeId::FLOAT) {
            CurVal = Builder.CreateFCmpOGE(left, right, "lttmp");
        } else {
            CurVal = Builder.CreateICmpSGE(left, right, "lttmp");
        }
    }

    void LLVMCodeGenerator::visit(common::FloatPattern &Node) {
        CurVal = ConstantFP::get(getType(Node.Ty), Node.Val);
    }

    void LLVMCodeGenerator::visit(common::IntPattern &Node) {
        CurVal = ConstantInt::get(getType(Node.Ty), (uint64_t)Node.Val);
    }

    void LLVMCodeGenerator::visit(common::Bool &Node) {
        CurVal = ConstantInt::get(getType(Node.Ty), (uint64_t)Node.Val);
    }

    void LLVMCodeGenerator::visit(common::CharPattern &Node) {
        CurVal = ConstantInt::get(getType(Node.Ty), (uint64_t)Node.Val);
    }

    void LLVMCodeGenerator::visit(common::String &Node) {
        CurVal = Builder.CreateGlobalString(Node.Val, CurFunc->getName() + ".str");

    }

    void LLVMCodeGenerator::visit(common::Call &Node) {
        Node.Callee->accept(*this);
        auto Callee = CurVal;

        std::vector<Value *> Args;
        for (auto &Arg : Node.Args) {
            Arg->accept(*this);
            Args.push_back(CurVal);
        }

        CurVal = Builder.CreateCall(Callee, Args, "calltmp");
    }

    void LLVMCodeGenerator::visit(common::IdPattern &Node) {
        switch (Ctx)
        {
            case PATTERN:
                ContextValues[Node.Val] = CurVal;
                break;
            case EXPR:
                // Pattern value
                CurVal = ContextValues[Node.Val];
                if (CurVal)
                    return;

                // Current module
                CurVal = Module->getFunction(Node.Val);
                if (CurVal)
                    return;

                // External module
                if (Drv.Global.Decls.count(Node.Val))
                    CurVal = llvm::Function::Create(getFuncType(Drv.Global.Decls[Node.Val]), llvm::Function::ExternalLinkage, Node.Val, Module.get());

                // TODO ERROR NOT FOUND
                break;
        }
    }


    void LLVMCodeGenerator::visit(common::Par &Node) {
        Node.Child->accept(*this);
    }

    void LLVMCodeGenerator::visit(common::TupleExpression &Node) {
        std::vector<llvm::Constant *> TmpVec;

        for (auto &Element : Node.Elements) {
            Element->accept(*this);
            TmpVec.push_back((Constant *)CurVal);
        }

        ArrayRef<Constant *> TupleVal(TmpVec);

        auto ConstVal = ConstantStruct::get(getTupleType(Node.Ty), TupleVal);
        CurVal = new GlobalVariable(*Module.get(), ConstVal->getType(), true, GlobalVariable::ExternalLinkage, ConstVal);

    }

    void LLVMCodeGenerator::visit(common::ListExpression &Node) {
        std::vector<llvm::Constant *> TmpVec;

        for (auto &Element : Node.Elements) {
            Element->accept(*this);
            TmpVec.push_back(dynamic_cast<Constant *>(CurVal));
        }

        ArrayRef<Constant *> ListData(TmpVec);

        auto ListType = ArrayType::get(getType(Node.Ty), Node.Elements.size());
        auto ConstVal = ConstantArray::get(ListType, ListData);

        CurVal = new GlobalVariable(*Module.get(), ConstVal->getType(), true, GlobalVariable::ExternalLinkage, ConstVal);
    }

    string LLVMCodeGenerator::ModuleString() {
        string ModuleStr;
        raw_string_ostream out(ModuleStr);
        out << *Module.get();
        return out.str();
    }
}
