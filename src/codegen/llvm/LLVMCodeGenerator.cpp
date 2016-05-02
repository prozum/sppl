#include "LLVMCodeGenerator.h"

using namespace std;
using namespace llvm;
using namespace codegen;

LLVMCodeGenerator::LLVMCodeGenerator(parser::Driver &Drv)
        : CodeGenerator(Drv),
          Ctx(getGlobalContext()),
          Builder(Ctx),
          Machine(EngineBuilder().selectTarget()),
          DataLayout(Machine->createDataLayout()),
          Module(std::make_unique<llvm::Module>("SpplModule", Ctx)) {

    Int1 = llvm::Type::getInt1Ty(Ctx);
    Int8 = llvm::Type::getInt8Ty(Ctx);
    Int32 = llvm::Type::getInt32Ty(Ctx);
    Int64 = llvm::Type::getInt64Ty(Ctx);
    Double = llvm::Type::getDoubleTy(Ctx);
    MainType = FunctionType::get(Int32,
                                 vector<Type *> {
                                         Int32,
                                         PointerType::getUnqual(PointerType::getUnqual(Int8))
                                 },
                                 false);
}

void LLVMCodeGenerator::visit(common::Program &node) {
    for (auto &func : node.Decls) {
        func->accept(*this);
    }

    *Drv.Out << ModuleString();
}

string LLVMCodeGenerator::ModuleString() {
    string ModuleStr;
    raw_string_ostream out(ModuleStr);
    out << *Module.get();
    return out.str();
}
