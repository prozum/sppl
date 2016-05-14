#include "LLVMCodeGen.h"

using namespace codegen;
using namespace llvm;
using namespace std;

Value *LLVMCodeGen::CreateListNode(common::Type Type, Value *Data, Value *NextNode, BasicBlock *Block, bool Const)
{
    auto ListType = getListType(Type);
    auto ListPtrType = getType(Type);

    // Null indicates list end
    if (!NextNode)
        NextNode = ConstantPointerNull::get(static_cast<PointerType *>(ListPtrType));

    // Const list
    if (Const) {
        auto ConstVal = ConstantStruct::get(ListType, Data, NextNode, nullptr);
        return new GlobalVariable(*Module, ListType, true, GlobalVariable::LinkageTypes::ExternalLinkage, ConstVal);
    }

    // Non-const list
    auto ListMalloc = CreateMalloc(ListType, Block);
    auto List = Builder.CreateBitCast(ListMalloc, ListPtrType);

    // Set list data
    CurVal = Builder.CreateStructGEP(ListType, List, 0);
    Builder.CreateStore(Data, CurVal);

    // Set next node
    CurVal = Builder.CreateStructGEP(ListType, List, 1);
    Builder.CreateStore(NextNode, CurVal);

    return List;
}

Instruction *LLVMCodeGen::CreateMalloc(llvm::Type *Type, BasicBlock *Block)
{
    auto Size = DataLayout->getPointerTypeSize(Type);
    auto AllocSize = ConstantInt::get(Int64, Size);
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



