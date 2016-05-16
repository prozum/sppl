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

    return TupleTypes[Ty] = StructType::create(Ctx, Subtypes, Ty.str());
}

llvm::StructType *LLVMCodeGen::getListType(common::Type Ty) {
    auto CacheTy = ListTypes.find(Ty);

    if (CacheTy != ListTypes.end())
        return CacheTy->second;

    llvm::Type *ElementTy;
    StructType *ListTy;
    if (!Ty.Subtypes.empty()) {
        ElementTy = getType(Ty.Subtypes.front());
        ListTy = StructType::create(Ctx, Ty.str());
    } else {
        ElementTy = VoidPtr;
        ListTy = StructType::create(Ctx, "VoidPtr");
    }

    llvm::Type *Body[] = { ElementTy, PointerType::getUnqual(ListTy) };
    ListTy->setBody(Body);

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

llvm::GlobalVariable *LLVMCodeGen::getRuntimeType(common::Type Ty) {
    auto CacheType = RuntimeTypes.find(Ty);

    if (CacheType != RuntimeTypes.end())
        return CacheType->second;

    vector<Constant *> ArrayElements;
    for (auto Subtype : Ty.Subtypes) {
        ArrayElements.push_back(getRuntimeType(Subtype));
    }
    ArrayElements.push_back(ConstantPointerNull::get(PointerType::getUnqual(RuntimeType)));

    // Create constant array with subtypes
    auto ConstArray = ConstantArray::get(ArrayType::get(RuntimeType, 0), ArrayElements);
    auto GlobalArray = new GlobalVariable(*Module.get(), ConstArray->getType(), true,
                                          GlobalVariable::ExternalLinkage, ConstArray);

    // Create runtime type
    auto ConstVal = ConstantStruct::get(RuntimeType, { ConstantInt::get(Int64, (uint64_t)Ty.Id), GlobalArray } );

    return RuntimeTypes[Ty] = new GlobalVariable(*Module.get(), ConstVal->getType(), true,
                                GlobalVariable::ExternalLinkage, ConstVal);
}




