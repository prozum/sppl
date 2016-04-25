#include "LLVMCodeGenerator.h"

using namespace std;
using namespace llvm;
using namespace codegen;

LLVMCodeGenerator::LLVMCodeGenerator(parser::Driver &driver)
    : CodeGenerator(driver), Builder(getGlobalContext()),
      Module(std::make_unique<llvm::Module>("SpplModule", getGlobalContext())) {
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
