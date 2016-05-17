#include "LLVMCodeGen.h"

using namespace codegen;
using namespace llvm;
using namespace std;

Value *LLVMCodeGen::createListNode(common::Type Type, Value *Data, Value *NextNode, BasicBlock *Block, bool Const)
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
    auto UnionCast = Builder.CreateBitCast(UnionAlloca, PointerType::getUnqual(getType(Ty)));
    Builder.CreateStore(Data, UnionCast);

    auto UnionArgGEP = Builder.CreateStructGEP(UnionType, UnionAlloca, 0);
    auto UnionArgCast = Builder.CreateBitCast(UnionArgGEP, PointerType::getUnqual(Int));
    auto UnionArgLoad = Builder.CreateLoad(Int, UnionArgCast);

    Builder.CreateCall(PrintFunc, vector<Value *> { UnionArgLoad, getRuntimeType(Ty) });
}

unsigned LLVMCodeGen::getAlignment(common::Type Ty) {
    return DataLayout->getPrefTypeAlignment(getType(Ty));
}




