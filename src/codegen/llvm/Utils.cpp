#include "LLVMCodeGen.h"

using namespace codegen;
using namespace llvm;


Value *LLVMCodeGen::CreateListNode(common::Type Type, Value *Data, Value *NextNode, BasicBlock *Block)
{
    auto ListType = getListType(Type);
    auto ListPtrType = getType(Type);

    // Malloc list container
    auto ListMalloc = CreateMalloc(ListType, Block);
    auto ListCast = Builder.CreateBitCast(ListMalloc, ListPtrType);

    // Set list data
    CurVal = Builder.CreateStructGEP(ListType, ListCast, 0);
    Builder.CreateStore(Data, CurVal);

    // Set next node
    CurVal = Builder.CreateStructGEP(ListType, ListCast, 1);
    if (!NextNode)
        NextNode = ConstantPointerNull::get(static_cast<PointerType *>(ListPtrType));
    Builder.CreateStore(NextNode, CurVal);

    return ListCast;
}

Instruction *LLVMCodeGen::CreateMalloc(llvm::Type *Type, BasicBlock *Block)
{
    auto Size = DataLayout->getPointerTypeSize(Type);
    auto AllocSize = ConstantInt::get(Int64, APInt(64, Size));
    return CreateMalloc(AllocSize, Block);
}

Instruction *LLVMCodeGen::CreateMalloc(Value *Size, BasicBlock *Block)
{
    auto Malloc = CallInst::CreateMalloc(Block,
                                         Int64, Int8, Size,
                                         nullptr, nullptr, "malloccall");
    Block->getInstList().push_back(Malloc);

    return Malloc;
}

unsigned LLVMCodeGen::getAlignment(common::Type Ty) {
    return DataLayout->getPrefTypeAlignment(getType(Ty));
}


