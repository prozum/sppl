#include "LLVMCodeGenerator.h"

using namespace std;
using namespace llvm;
using namespace codegen;

llvm::Type *LLVMCodeGenerator::getType(common::Type Ty, bool Ptr) {
    llvm::Type *NewType;

    switch (Ty.Id) {
    case common::TypeId::FLOAT:
        NewType = llvm::Type::getDoubleTy(getGlobalContext());
        break;
    case common::TypeId::INT:
        NewType = llvm::Type::getInt64Ty(getGlobalContext());
        break;
    case common::TypeId::BOOL:
        NewType = llvm::Type::getInt1Ty(getGlobalContext());
        break;
    case common::TypeId::CHAR:
        NewType = llvm::Type::getInt8Ty(getGlobalContext());
        break;
    case common::TypeId::TUPLE:
        return PointerType::getUnqual(getTupleType(Ty));
    case common::TypeId::LIST:
        return PointerType::getUnqual(getListType(Ty));
    case common::TypeId::SIGNATURE:
        return PointerType::getUnqual(getFuncType(Ty));
    case common::TypeId::STRING:
        return PointerType::getUnqual(ArrayType::get(
            llvm::Type::getInt8Ty(getGlobalContext()), Ty.subtypeCount()));
    default:
        throw runtime_error("This should not happen!");
    }

    if (Ptr)
        return PointerType::getUnqual(NewType);
    else
        return NewType;
}

llvm::StructType *LLVMCodeGenerator::getTupleType(common::Type Ty) {
    auto OldType = TupleTypes.find(Ty);

    if (OldType != TupleTypes.end())
        return OldType->second;

    std::vector<llvm::Type *> TmpVec;
    for (auto &Subtype : Ty.Subtypes)
        TmpVec.push_back(getType(Subtype));
    llvm::ArrayRef<llvm::Type *> Subtypes(TmpVec);

    return TupleTypes[Ty] = StructType::create(getGlobalContext(), Subtypes);
}

llvm::ArrayType *LLVMCodeGenerator::getListType(common::Type Ty) {
    auto OldType = ListTypes.find(Ty);

    if (OldType != ListTypes.end())
        return OldType->second;

    return ListTypes[Ty] = ArrayType::get(getType(Ty.Subtypes[0]), 0);
}

llvm::FunctionType *LLVMCodeGenerator::getFuncType(common::Type Ty) {
    auto OldType = FuncTypes.find(Ty);

    if (OldType != FuncTypes.end())
        return OldType->second;

    auto OutputType = getType(Ty.Subtypes.back());
    std::vector<llvm::Type *> InputTypes;
    for (size_t i = 0; i < Ty.Subtypes.size() - 1; ++i) {
        InputTypes.push_back(getType(Ty.Subtypes[i]));
    }

    return FuncTypes[Ty] = FunctionType::get(OutputType, InputTypes, false);
}
