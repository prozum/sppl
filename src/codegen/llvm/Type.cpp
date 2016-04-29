#include "LLVMCodeGenerator.h"

using namespace std;
using namespace llvm;
using namespace codegen;

llvm::Type *LLVMCodeGenerator::getType(common::Type Ty) {
    switch (Ty.Id) {
    case common::TypeId::FLOAT:
        return llvm::Type::getDoubleTy(getGlobalContext());
    case common::TypeId::INT:
        return llvm::Type::getInt64Ty(getGlobalContext());
    case common::TypeId::BOOL:
        return llvm::Type::getInt1Ty(getGlobalContext());
    case common::TypeId::CHAR:
        return llvm::Type::getInt8Ty(getGlobalContext());
    case common::TypeId::TUPLE:
        return PointerType::getUnqual(getTupleType(Ty));
    case common::TypeId::LIST:
        return PointerType::getUnqual(getListType(Ty));
    case common::TypeId::SIGNATURE:
        return PointerType::getUnqual(getFuncType(Ty));
    case common::TypeId::STRING:
        return getListType(common::Type(common::TypeId::LIST, vector<common::Type> {common::Type(common::TypeId::CHAR)}));
        //return PointerType::getUnqual(ArrayType::get(
        //    llvm::Type::getInt8Ty(getGlobalContext()), Ty.subtypeCount()));
    case common::TypeId::VOID:
        return llvm::Type::getVoidTy(getGlobalContext());
    default:
        throw runtime_error("This should not happen!");
    }
}

llvm::StructType *LLVMCodeGenerator::getTupleType(common::Type Ty) {
    auto CacheTy = TupleTypes.find(Ty);

    if (CacheTy != TupleTypes.end())
        return CacheTy->second;

    std::vector<llvm::Type *> TmpVec;
    for (auto &Subtype : Ty.Subtypes)
        TmpVec.push_back(getType(Subtype));
    llvm::ArrayRef<llvm::Type *> Subtypes(TmpVec);

    return TupleTypes[Ty] = StructType::create(getGlobalContext(), Subtypes, "tuple");
}

llvm::StructType *LLVMCodeGenerator::getListType(common::Type Ty) {
    auto CacheTy = ListTypes.find(Ty);

    if (CacheTy != ListTypes.end())
        return CacheTy->second;

    std::vector<llvm::Type *> TmpVec;
    TmpVec.push_back(IntegerType::get(getGlobalContext(), 32));
    TmpVec.push_back(PointerType::getUnqual(ArrayType::get(getType(Ty.Subtypes[0]), 0)));
    llvm::ArrayRef<llvm::Type *> Subtypes(TmpVec);

    return ListTypes[Ty] = StructType::create(getGlobalContext(), Subtypes, "list");
}

llvm::FunctionType *LLVMCodeGenerator::getFuncType(common::Type Ty) {
    auto CacheType = FuncTypes.find(Ty);

    if (CacheType != FuncTypes.end())
        return CacheType->second;

    auto OutputType = getType(Ty.Subtypes.back());
    std::vector<llvm::Type *> InputTypes;
    for (size_t i = 0; i < Ty.Subtypes.size() - 1; ++i) {
        InputTypes.push_back(getType(Ty.Subtypes[i]));
    }

    return FuncTypes[Ty] = FunctionType::get(OutputType, InputTypes, false);
}
