#include "LLVMCodeGen.h"

using namespace std;
using namespace llvm;
using namespace codegen;

LLVMCodeGen::LLVMCodeGen(parser::Driver &Drv)
        : CodeGenerator(Drv),
          Ctx(getGlobalContext()),
          Builder(Ctx),
          RawOut(*Drv.MOut) {

    // Initialize LLVM before TargetMachine and DataLayout
    initLLVM();
    Machine = std::unique_ptr<TargetMachine>(EngineBuilder().selectTarget());
    DataLayout = std::make_unique<llvm::DataLayout>(Machine->createDataLayout());
    Module = std::make_unique<llvm::Module>("SpplModule", Ctx);

    // Type constants
    Int1 = llvm::Type::getInt1Ty(Ctx);
    Int8 = llvm::Type::getInt8Ty(Ctx);
    Int32 = llvm::Type::getInt32Ty(Ctx);
    Int64 = llvm::Type::getInt64Ty(Ctx);
    Double = llvm::Type::getDoubleTy(Ctx);
    VoidPtr = PointerType::getUnqual(Int8);

    // Union type used for easy bitcast
    UnionType = StructType::create(Ctx, Int64, "union");

    // Runtime type
    RuntimeType = StructType::create(Ctx, "runtime_type");
    llvm::Type *Body[] = { Int64, PointerType::getUnqual( ArrayType::get(RuntimeType, 0) ) };
    RuntimeType->setBody(Body);

    // Main type
    MainType = FunctionType::get(Int32,
                                 vector<Type *> {
                                         Int32,
                                         PointerType::getUnqual(PointerType::getUnqual(Int8))
                                 },
                                 false);

}

void LLVMCodeGen::visit(common::Program &node) {
    // Clear runtime types
    RuntimeTypes.clear();

    // Helper functions
    ArgFunc = CreateArgFunc();
    PrintFunc = CreatePrintFunc();

    for (auto &func : node.Decls) {
        func->accept(*this);
    }

    // Don't output anything with JIT
    if (Drv.JIT)
        return;

    // Generate Binary or IR Source
    if (Drv.Binary) {
        raw_os_ostream RawOut(*Drv.Out);
        WriteBitcodeToFile(Module.get(), RawOut);
    } else {
        *Drv.Out << ModuleString();
    }
}

string LLVMCodeGen::ModuleString() {
    string ModuleStr;
    raw_string_ostream out(ModuleStr);
    out << *Module.get();
    return out.str();
}
