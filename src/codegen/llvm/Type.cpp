#include "LLVMCodeGenerator.h"

using namespace codegen;
using namespace llvm;

llvm::Type *LLVMCodeGenerator::getType(common::Type Ty, bool Ptr) {
    llvm::Type *new_type;

    switch (Ty.Id) {
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
    case common::TypeId::TUPLE:
        return PointerType::getUnqual(getTupleType(Ty));
    case common::TypeId::LIST:
        return PointerType::getUnqual(getListType(Ty));
    case common::TypeId::SIGNATURE:
        return PointerType::getUnqual(getFuncType(Ty));
    case common::TypeId::STRING:
        // return llvm::Type::getInt8PtrTy(getGlobalContext());
        return PointerType::getUnqual(ArrayType::get(
            llvm::Type::getInt8Ty(getGlobalContext()), Ty.NumSubtypes));
    default:
        throw runtime_error("This should not happen!");
    }

    if (Ptr)
        return PointerType::getUnqual(new_type);
    else
        return new_type;
}

llvm::StructType *LLVMCodeGenerator::getTupleType(common::Type Ty) {
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

llvm::StructType *LLVMCodeGenerator::getListType(common::Type Ty) {
    auto OldType = ListTypes.find(Ty);

    if (OldType != ListTypes.end())
        return OldType->second;

    vector<llvm::Type *> TmpVec = {getType(Ty.Subtypes[0], true),
                                   llvm::Type::getInt64Ty(getGlobalContext())};
    llvm::ArrayRef<llvm::Type *> Subtypes(TmpVec);

    auto NewType = StructType::create(getGlobalContext(), Subtypes);
    ListTypes[Ty] = NewType;

    return NewType;
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

    auto NewType = FunctionType::get(OutputType, InputTypes, false);
    FuncTypes[Ty] = NewType;

    return NewType;
}
