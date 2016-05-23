#include "LLVMCodeGen.h"
#include "std/std-bin.h"

using namespace std;
using namespace llvm;
using namespace codegen;
using namespace common;
static std::unique_ptr<llvm::Module> ModuleStd;

llvm::Module *LLVMCodeGen::getStdLib() {
    return ModuleStd.get();
}

std::unique_ptr<llvm::Module> LLVMCodeGen::getStdModule() {
    return move(ModuleStd);
}

void LLVMCodeGen::setTriple() {
    // Set triple from target machine
    Module->setTargetTriple(Machine->getTargetTriple().getTriple());
}

void LLVMCodeGen::initStdLib() {
    // Load standard library implemented in C
    if (!ModuleStd) {
        SMDiagnostic Dia;
        auto MemBuf = MemoryBuffer::getMemBuffer(StringRef((char *) std_bc, std_bc_len));
        ModuleStd = parseIR(MemBuf->getMemBufferRef(), Dia, Ctx);
        ModuleStd->setTargetTriple(Machine->getTargetTriple().getTriple());

        //ModuleStd->dump();
        assert(ModuleStd && "Standard Library could not be parsed");


        // Get SPPL_DECL annotations
        auto Annos = ModuleStd->getNamedGlobal("llvm.global.annotations");
        if (Annos) {
            auto Struct1 = static_cast<ConstantArray *>(Annos->getOperand(0));
            for (size_t i = 0; i < Struct1->getNumOperands(); ++i) {
                auto Struct2 = static_cast<ConstantStruct *>(Struct1->getOperand(i));
                if (auto Func = static_cast<llvm::Function *>(Struct2->getOperand(0)->getOperand(0))) {
                    auto Anno = cast<ConstantDataArray>(
                            cast<GlobalVariable>(Struct2->getOperand(1)->getOperand(0))->getOperand(0))->getAsCString();
                    Func->addFnAttr(SPPL_DECL);
                }
            }
        }
    }

    // Load standard library function signatures
    for (auto &Func: ModuleStd->functions()) {
        addStdFunc(Func.getName(), Func.getFunctionType(), Func.hasFnAttribute(SPPL_DECL));
    }

    // Load standard library types
    RunType = ModuleStd->getTypeByName("struct.type");
    UnionType = ModuleStd->getTypeByName("union.value");
    ListType = ModuleStd->getTypeByName("struct.list");

    // Load standard library implemented in IR
    // Nothing is implemented i IR right now

    // SPPL declaration to access standard library
    auto DDSig = common::Type(TypeId::SIGNATURE, vector<common::Type> { common::Type(TypeId::FLOAT), common::Type(TypeId::FLOAT)});
    auto DDDSig = common::Type(TypeId::SIGNATURE, vector<common::Type> { common::Type(TypeId::FLOAT), common::Type(TypeId::FLOAT), common::Type(TypeId::FLOAT)});
    vector<string> Funcs = {
        "sin", "asin",  "sinh",
        "cos", "acos",  "cosh",
        "tan", "atan",  "tanh",
        "pow", "exp",   "exp2",
        "log", "log10", "fabs",
        "floor", "ceil", "sqrt"
    };
    addStdFunc("tan", DDSig);
    addStdFunc("pow", DDDSig);

    for (auto &Func : Funcs) {
        addStdFunc(Func, DDSig, true);
    }
}

void LLVMCodeGen::addStdFunc(std::string FuncName, common::Type Ty, bool Decl) {
    assert(Ty.Id == TypeId::SIGNATURE);

    StdFuncs[FuncName] = getLLVMFuncType(Ty);

    if (Decl)
        Drv.addDecl(FuncName, Ty);
}



void LLVMCodeGen::addStdFunc(string FuncName, FunctionType *Ty, bool Decl) {
    StdFuncs[FuncName] = Ty;

    if (Decl)
        Drv.addDecl(FuncName, getFuncType(Ty));
}

llvm::Function *LLVMCodeGen::getStdFunc(string FuncName) {
    auto Decl = StdFuncDecls.find(FuncName);

    if (Decl != StdFuncDecls.end())
        return Decl->second;

    auto FuncType = StdFuncs[FuncName];

    return StdFuncDecls[FuncName] = llvm::Function::Create(FuncType, llvm::Function::ExternalLinkage, FuncName, Module.get());
}
