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

    // Set list length
    auto Length = Builder.CreateStructGEP(ListType, ListCast, 0);
    if (NextNode) {
        // Multi element list
        //CurVal = Builder.CreateLoad(ListType, NextNode, "loadtmp");
        auto Ty1 = CurVal->getType();
        CurVal = Builder.CreateStructGEP(ListType, NextNode, 0);
        auto NextLength = Builder.CreateLoad(Int64, CurVal, "loadtmp");
        CurVal = Builder.CreateAdd(NextLength, ConstantInt::get(Int64, 1));
        Builder.CreateStore(CurVal, Length);
    } else {
        if (Data)
            // Single element list
            Builder.CreateStore(ConstantInt::get(Int64, 1), Length);
        else
            // Empty list
            Builder.CreateStore(ConstantInt::get(Int64, 0), Length);
    }

    // Set list data
    if (Data) {
        CurVal = Builder.CreateStructGEP(ListType, ListCast, 1);
        Builder.CreateStore(Data, CurVal);
    }

    // Set next node
    if (NextNode) {
        CurVal = Builder.CreateStructGEP(ListType, ListCast, 2);
        Builder.CreateStore(NextNode, CurVal);
    }

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


