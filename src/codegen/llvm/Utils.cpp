#include "LLVMCodeGen.h"

using namespace codegen;
using namespace llvm;


Value *LLVMCodeGen::CreateList(common::Type Type, Value *Data, Value *Size, BasicBlock *Block)
{
    auto ListType = getListType(Type);
    auto ListPtrType = getType(Type);

    // Malloc list container
    auto ListMalloc = CreateMalloc(ListType, Block);
    auto ListCast = Builder.CreateBitCast(ListMalloc, ListPtrType);

    // Set list length
    CurVal = Builder.CreateStructGEP(ListType, ListCast, 0);
    Builder.CreateStore(Size, CurVal);

    // Set list data
    CurVal = Builder.CreateStructGEP(ListType, ListCast, 1);
    auto DataCast = Builder.CreateBitCast(Data, PointerType::getUnqual(ArrayType::get(getType(Type.Subtypes.front()), 0)));
    Builder.CreateStore(DataCast, CurVal);

    return ListMalloc;
}

Instruction *LLVMCodeGen::CreateMalloc(llvm::Type *Type, BasicBlock *Block)
{
    auto Size = DataLayout.getPointerTypeSize(Type);
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
