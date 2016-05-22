#include "LLVMCodeGen.h"

using namespace std;
using namespace llvm;
using namespace codegen;
using namespace common;

void LLVMCodeGen::initTypes() {
    // Type constants
    Int1 = llvm::Type::getInt1Ty(Ctx);
    Int8 = llvm::Type::getInt8Ty(Ctx);
    Int32 = llvm::Type::getInt32Ty(Ctx);
    VoidPtr = PointerType::getUnqual(Int8);

    Int = llvm::Type::getInt64Ty(Ctx);
    Float = llvm::Type::getDoubleTy(Ctx);

    // Union type used for easy bitcast
    /*
    UnionType = StructType::create(Ctx, Int, "Union");

    // Runtime type
    RuntimeType = StructType::create(Ctx);
    llvm::Type *Body[] = { Int, PointerType::getUnqual(RuntimeType) };
    RuntimeType->setBody(Body);
     */

    // Main type
    MainType = FunctionType::get(Int32,
                                 vector<llvm::Type *> {
                                         Int32,
                                         PointerType::getUnqual(VoidPtr)
                                 },
                                 false);
}

llvm::Type *LLVMCodeGen::getLLVMType(common::Type Ty) {
    switch (Ty.Id) {
    case common::TypeId::FLOAT:
        return Float;
    case common::TypeId::INT:
        return Int;
    case common::TypeId::BOOL:
        return Int;
    case common::TypeId::CHAR:
        return Int;
    case common::TypeId::TUPLE:
        return PointerType::getUnqual(getLLVMTupleType(Ty));
    case common::TypeId::LIST:
    case common::TypeId::EMPTYLIST:
    case common::TypeId::STRING:
        return PointerType::getUnqual(getLLVMListType(Ty));
    case common::TypeId::SIGNATURE:
        return PointerType::getUnqual(getLLVMFuncType(Ty));
    case common::TypeId::VOID:
        return llvm::Type::getVoidTy(Ctx);
    default:
        assert(0 && "Type not supported");
    }
}

llvm::StructType *LLVMCodeGen::getLLVMTupleType(common::Type Ty) {
    auto CacheTy = TupleTypes.find(Ty);

    if (CacheTy != TupleTypes.end())
        return CacheTy->second;

    std::vector<llvm::Type *> Subtypes;
    for (auto &Subtype : Ty.Subtypes)
        Subtypes.push_back(getLLVMType(Subtype));

    return TupleTypes[Ty] = StructType::create(Ctx, Subtypes, Ty.str());
}

llvm::StructType *LLVMCodeGen::getLLVMListType(common::Type Ty) {
    auto CacheTy = ListTypes.find(Ty);

    if (CacheTy != ListTypes.end())
        return CacheTy->second;

    llvm::Type *ElementTy;
    StructType *ListTy;
    if (!Ty.Subtypes.empty()) {
        ElementTy = getLLVMType(Ty.Subtypes.front());
        ListTy = StructType::create(Ctx, Ty.str());
    } else {
        ElementTy = VoidPtr;
        ListTy = StructType::create(Ctx, "VoidPtr");
    }

    llvm::Type *Body[] = { ElementTy, PointerType::getUnqual(ListTy) };
    ListTy->setBody(Body);

    return ListTypes[Ty] = ListTy;
}

llvm::FunctionType *LLVMCodeGen::getLLVMFuncType(common::Type Ty) {
    auto CacheType = FuncTypes.find(Ty);

    if (CacheType != FuncTypes.end())
        return CacheType->second;

    auto OutputType = getLLVMType(Ty.Subtypes.back());
    std::vector<llvm::Type *> InputTypes;
    for (size_t i = 0; i < Ty.Subtypes.size() - 1; ++i) {
        InputTypes.push_back(getLLVMType(Ty.Subtypes[i]));
    }

    return FuncTypes[Ty] = FunctionType::get(OutputType, InputTypes, false);
}

llvm::GlobalVariable *LLVMCodeGen::getRuntimeType(common::Type Ty) {
    auto CacheType = RunTypes.find(Ty);

    if (CacheType != RunTypes.end())
        return CacheType->second;

    auto ElementTy = PointerType::getUnqual(RunType);
    auto ArrayTy = ArrayType::get(ElementTy, Ty.Subtypes.size() + 1);

    vector<Constant *> ArrayElements;
    for (auto Subtype : Ty.Subtypes) {
        auto Element = getRuntimeType(Subtype);
        ArrayElements.push_back(static_cast<Constant *>(Builder.CreateBitCast(Element, ElementTy)));
    }
    ArrayElements.push_back(ConstantPointerNull::get(ElementTy));

    // Create constant array with subtypes
    //auto ArrayPtrTy = PointerType::getUnqual(ArrayType::get(RuntimeType, 0));
    //Constant *Array;

    auto Array = ConstantArray::get(ArrayTy, ArrayElements);

    // Create runtime type
    //auto ArrayPtr = static_cast<Constant *>(Builder.CreateGEP(ElementTy, Array, ConstantInt::get(Int32, 0)));
    auto StructTy = StructType::get(Ctx, { Int, ArrayTy });
    auto ConstVal = ConstantStruct::get(StructTy, { ConstantInt::get(Int, (uint64_t)Ty.Id), Array } );

    return RunTypes[Ty] = new GlobalVariable(*Module, ConstVal->getType(), true,
                                GlobalVariable::ExternalLinkage, ConstVal);
}

common::Type LLVMCodeGen::getType(llvm::Type *Ty) {
    if (Ty == Int)
        return common::Type(TypeId::INT);
    if (Ty == Float)
        return common::Type(TypeId::FLOAT);
    if (Ty == VoidPtr)
        return common::Type(TypeId::STRING, vector<common::Type> { common::Type(TypeId::CHAR) });
    if (Ty->isVoidTy())
        return common::Type(TypeId::VOID);
    if (Ty->isFunctionTy())
        return getFuncType(static_cast<FunctionType *>(Ty));

    Ty->dump();
    assert(0);
}

common::Type LLVMCodeGen::getFuncType(FunctionType *FuncTy)
{
    common::Type Res(TypeId::SIGNATURE);

    auto Subtypes = FuncTy->subtypes();
    for (size_t i = 1; i != Subtypes.size(); ++i) {
        Res.Subtypes.push_back(getType(Subtypes[i]));
    }
    Res.Subtypes.push_back(getType(FuncTy->getReturnType()));

    return Res;
}

common::Type LLVMCodeGen::getFuncType(GlobalVariable *FuncRunTy)
{
    common::Type Res(TypeId::SIGNATURE);

    /*
    auto Subtypes = FuncRunTy->subtypes();
    for (size_t i = 1; i != Subtypes.size(); ++i) {
        Res.Subtypes.push_back(getType(Subtypes[i]));
    }
    Res.Subtypes.push_back(getType(FuncTy->getReturnType()));

    return Res;
     */
}
