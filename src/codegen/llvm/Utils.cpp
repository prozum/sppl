#include "LLVMCodeGen.h"

using namespace codegen;
using namespace llvm;
using namespace std;

Value *LLVMCodeGen::createListNode(common::Type Type, Value *Data, Value *NextNode, BasicBlock *Block, bool Const)
{
    auto ListType = getLLVMListType(Type);
    auto ListPtrType = PointerType::getUnqual(ListType);

    // Null indicates list end
    if (!NextNode)
        NextNode = ConstantPointerNull::get(ListPtrType);

    // Const list
    if (Const) {
        auto ConstVal = ConstantStruct::get(ListType, Data, NextNode, nullptr);
        return new GlobalVariable(*Module, ListType, true, GlobalVariable::LinkageTypes::ExternalLinkage, ConstVal);
    }

    // Non-const list
    auto ListMalloc = createMalloc(ListType, Block);
    auto List = Builder.CreateBitCast(ListMalloc, ListPtrType);

    // Set list data
    CurVal = Builder.CreateStructGEP(ListType, List, 0);
    Builder.CreateStore(Data, CurVal);

    // Set next node
    CurVal = Builder.CreateStructGEP(ListType, List, 1);
    Builder.CreateStore(NextNode, CurVal);

    return List;
}

Instruction *LLVMCodeGen::createMalloc(llvm::Type *Type, BasicBlock *Block)
{
    auto Size = DataLayout->getPointerTypeSize(Type);
    auto AllocSize = ConstantInt::get(Int, Size);
    return createMalloc(AllocSize, Block);
}

Instruction *LLVMCodeGen::createMalloc(Value *Size, BasicBlock *Block)
{
    auto Malloc = CallInst::CreateMalloc(Block,
                                         Int, Int, Size,
                                         nullptr, nullptr, "malloccall");
    Block->getInstList().push_back(Malloc);

    return Malloc;
}

void LLVMCodeGen::createPrint(Value *Data, common::Type Ty) {
    auto UnionAlloca = Builder.CreateAlloca(UnionType);
    auto UnionCast = Builder.CreateBitCast(UnionAlloca, PointerType::getUnqual(getLLVMType(Ty)));
    Builder.CreateStore(Data, UnionCast);

    auto UnionArgGEP = Builder.CreateStructGEP(UnionType, UnionAlloca, 0);
    auto UnionArgCast = Builder.CreateBitCast(UnionArgGEP, PointerType::getUnqual(UnionType));
    //auto UnionArgLoad = Builder.CreateLoad(UnionType, UnionArgCast);

    auto TyArg = getRuntimeType(Ty);
    auto TyArgCast = Builder.CreateBitCast(TyArg, PointerType::getUnqual(RunType));

    Builder.CreateCall(getStdFunc("_print"), vector<Value *> { UnionArgCast, TyArgCast, ConstantInt::get(Int32, 1) });
}

unsigned LLVMCodeGen::getAlignment(common::Type Ty) {
    return DataLayout->getPrefTypeAlignment(getLLVMType(Ty));
}




