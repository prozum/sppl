#include "LLVMCodeGen.h"

using namespace std;
using namespace llvm;
using namespace codegen;

llvm::Type *LLVMCodeGen::getType(common::Type Ty) {
    switch (Ty.Id) {
    case common::TypeId::FLOAT:
        return Double;
    case common::TypeId::INT:
        return Int64;
    case common::TypeId::BOOL:
        return Int1;
    case common::TypeId::CHAR:
        return Int8;
    case common::TypeId::TUPLE:
        return PointerType::getUnqual(getTupleType(Ty));
    case common::TypeId::LIST:
    case common::TypeId::EMPTYLIST:
    case common::TypeId::STRING:
        return PointerType::getUnqual(getListType(Ty));
    case common::TypeId::SIGNATURE:
        return PointerType::getUnqual(getFuncType(Ty));
    case common::TypeId::VOID:
        return llvm::Type::getVoidTy(Ctx);
    default:
        assert(0 && "Type not supported");
    }
}

llvm::StructType *LLVMCodeGen::getTupleType(common::Type Ty) {
    auto CacheTy = TupleTypes.find(Ty);

    if (CacheTy != TupleTypes.end())
        return CacheTy->second;

    std::vector<llvm::Type *> Subtypes;
    for (auto &Subtype : Ty.Subtypes)
        Subtypes.push_back(getType(Subtype));

    return TupleTypes[Ty] = StructType::create(Ctx, Subtypes, "tuple");
}

llvm::StructType *LLVMCodeGen::getListType(common::Type Ty) {
    auto CacheTy = ListTypes.find(Ty);

    if (CacheTy != ListTypes.end())
        return CacheTy->second;

    auto ListTy = StructType::create(Ctx, "list");
    llvm::Type *Subtypes[] = { getType(Ty.Subtypes.front()), PointerType::getUnqual(ListTy) };
    ListTy->setBody(Subtypes);

    return ListTypes[Ty] = ListTy;
}

llvm::FunctionType *LLVMCodeGen::getFuncType(common::Type Ty) {
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




