#include "SpplJit.h"

using namespace std;
using namespace llvm;
using namespace llvm::orc;
using namespace jit;
using namespace common;

SpplJit::SpplJit()
    : CodeGen(Drv), ScopeGen(&Drv.Global), TypeChecker(&Drv.Global),
      CompileLayer(ObjectLayer, SimpleCompiler(*CodeGen.Machine)) {

    sys::DynamicLibrary::LoadLibraryPermanently(nullptr);

    // Set Driver to JIT mode
    Drv.JIT = true;

    // Create initial module
    createModule();

    // Print functions
    Drv.addExternFunc("printff", common::Type(TypeId::SIGNATURE,
                                             vector<common::Type> {
                                                     common::Type(TypeId::FLOAT),
                                                     common::Type(TypeId::VOID)
                                             }));
    Drv.addExternFunc("printii", common::Type(TypeId::SIGNATURE,
                                         vector<common::Type> {
                                                 common::Type(TypeId::INT),
                                                 common::Type(TypeId::VOID)
                                         }));

    // Trigonometry function
    Drv.addExternFunc("sin", common::Type(TypeId::SIGNATURE,
                                         vector<common::Type> {
                                                 common::Type(TypeId::FLOAT),
                                                 common::Type(TypeId::FLOAT)
                                         }));
    Drv.addExternFunc("cos", common::Type(TypeId::SIGNATURE,
                                         vector<common::Type> {
                                                 common::Type(TypeId::FLOAT),
                                                 common::Type(TypeId::FLOAT)
                                         }));
    Drv.addExternFunc("tan", common::Type(TypeId::SIGNATURE,
                                         vector<common::Type> {
                                                 common::Type(TypeId::FLOAT),
                                                 common::Type(TypeId::FLOAT)
                                         }));
}

SpplJit::ModuleHandleT SpplJit::addModule(unique_ptr<Module> M) {
    auto Resolver = createLambdaResolver(
        [&](const string &Name) {
            if (auto Sym = findMangledSymbol(Name))
                return RuntimeDyld::SymbolInfo(Sym.getAddress(),
                                               Sym.getFlags());
            return RuntimeDyld::SymbolInfo(nullptr);
        },
        [](const string &S) { return nullptr; });
    auto Handler = CompileLayer.addModuleSet(
        singletonSet(move(M)), std::make_unique<SectionMemoryManager>(),
        move(Resolver));

    ModuleHandles.push_back(Handler);
    return Handler;
}

void SpplJit::removeModule(ModuleHandleT Handler) {
    ModuleHandles.erase(
        find(ModuleHandles.begin(), ModuleHandles.end(), Handler));
    CompileLayer.removeModuleSet(Handler);
}

JITSymbol SpplJit::findSymbol(const string Name) {
    return findMangledSymbol(mangle(Name));
}

JITSymbol SpplJit::findMangledSymbol(const string &Name) {
    for (auto Handler :
         make_range(ModuleHandles.rbegin(), ModuleHandles.rend()))
        if (auto Sym = CompileLayer.findSymbolIn(Handler, Name, true))
            return Sym;

    if (auto SymAddr = RTDyldMemoryManager::getSymbolAddressInProcess(Name))
        return JITSymbol(SymAddr, JITSymbolFlags::Exported);

    return nullptr;
}

string SpplJit::mangle(const string &Name) {
    string MangledName;
    {
        raw_string_ostream MangledNameStream(MangledName);
        Mangler::getNameWithPrefix(MangledNameStream, Name, *CodeGen.DataLayout);
    }
    return MangledName;
}

void SpplJit::createModule() {
    // Open a new module
    CodeGen.Module =
        std::make_unique<Module>("SpplJit", CodeGen.Ctx);
    CodeGen.Module->setDataLayout(CodeGen.Machine->createDataLayout());

    // Create a new pass manager attached to it
    PassMgr =
        std::make_unique<legacy::FunctionPassManager>(CodeGen.Module.get());

    // Add optimization passes
    PassMgr->add(createTailCallEliminationPass());
    PassMgr->add(createInstructionCombiningPass());
    PassMgr->add(createReassociatePass());
    PassMgr->add(createGVNPass());
    PassMgr->add(createCFGSimplificationPass());
    PassMgr->doInitialization();
}

string SpplJit::getOutput(intptr_t Data, common::Type Type) {
    switch (Type.Id) {
    case TypeId::INT:
        return to_string((int64_t)Data);
    case TypeId::FLOAT:
        // WTF is double type a pointer
        return to_string(*(double *)Data);
    case TypeId::CHAR:
        return "'" + string(1, (char)Data) + "'";
    case TypeId::STRING:
        // Ignore list container
        Data += sizeof(int64_t);
        Data = *(int64_t *)Data;
        return "\"" + string((char *)Data) + "\"";
    case TypeId::BOOL:
        return ((bool)Data) ? "True" : "False";
    case TypeId::TUPLE:
        return getOutputTuple(Data, Type);
    case TypeId::LIST:
        return getOutputList(Data, Type);
    case TypeId::SIGNATURE:
        return Type.str();
    case TypeId::VOID:
        return "";
    default:
        assert(0 && "Type not supported");
    }
}

string SpplJit::getOutputTuple(intptr_t Addr, common::Type Ty) {
    auto Alignment = CodeGen.getAlignment(Ty);
    auto Subtypes = Ty.Subtypes;
    string Out("(");
    for (size_t i = 0; i < Subtypes.size(); ++i) {
        auto Offset = Addr % Alignment ? Alignment - Addr % Alignment : 0;
        switch (Subtypes[i].Id) {
        case TypeId::INT:
            Addr += Offset;
            Out += getOutput(*(int64_t *)Addr, Subtypes[i]);
            Addr += sizeof(int64_t);
            break;
        case TypeId::FLOAT:
            Addr += Offset;
            Out += getOutput(Addr, Subtypes[i]);
            Addr += sizeof(double);
            break;
        case TypeId::CHAR:
            Out += getOutput(*(char *)Addr, Subtypes[i]);
            Addr += sizeof(char);
            break;
        case TypeId::BOOL:
            Out += getOutput(*(bool *)Addr, Subtypes[i]);
            Addr += sizeof(bool);
            break;
        case TypeId::STRING:
            Addr += Offset;
            Out += getOutput(*(intptr_t *)Addr, Subtypes[i]);
            Addr += sizeof(intptr_t *);
            break;
        case TypeId::TUPLE:
            Addr += Offset;
            Out += getOutputTuple(*(intptr_t *)Addr, Subtypes[i]);
            Addr += sizeof(intptr_t *);
            break;
        default:
            assert(0 && "Type not supported");
        }
        if (i + 1 != Subtypes.size())
            Out += ", ";
    }

    return Out + ")";
}

string SpplJit::getOutputList(intptr_t Addr, common::Type Ty)
{
    string Out("[");
    auto Subtype = Ty.Subtypes.front();

    auto Count  = *(int64_t *)Addr;

    for (int64_t i = 0; i < Count; ++i) {
        Addr += sizeof(int64_t *);

        auto Data = *(intptr_t *)Addr;
        Out += getOutput(Data, Subtype);

        if (i + 1 != Count) {
            Out += ", ";
            Addr += sizeof(intptr_t *);
            Addr = *(intptr_t * )Addr;
        }
    }

    return Out + "]";
}

extern "C" void printff(double Float) {
    fprintf(stderr, "%f", Float);
}

extern "C" void printii(long Int) {
    fprintf(stderr, "%li", Int);
}

int SpplJit::eval(string Str) {
    if (!Drv.parseString(Str))
        return 1;

    if (!Drv.accept(ScopeGen))
        return 2;

    if (!Drv.accept(TypeChecker))
        return 3;

#if OPTIMIZER
    if (!Drv.accept(Optimizer))
        return 4;
#endif

    if (!Drv.accept(CodeGen))
        return 5;
    auto FuncNode = static_cast<common::Function *>(Drv.Prog->Decls.front().get());
    auto RetTy = FuncNode->Signature.Subtypes.back();

// Optimize
#if SPPLDEBUG
    cout << "#-----------------------------------------------------------------"
            "-------------#"
         << endl;
    cout << "|                                 Unoptimized                     "
            "             |"
         << endl;
    cout << "#-----------------------------------------------------------------"
            "-------------#"
         << endl;
    cout << CodeGen.ModuleString();
#endif
    auto FuncIR = CodeGen.Module->getFunction(FuncNode->Id);
    assert(FuncIR && "Function generation failed.");
    PassMgr->run(*FuncIR);
#if SPPLDEBUG
    cout << "#-----------------------------------------------------------------"
            "-------------#"
         << endl;
    cout << "|                                  Optimized                      "
            "             |"
         << endl;
    cout << "#-----------------------------------------------------------------"
            "-------------#"
         << endl;
    cout << CodeGen.ModuleString();
#endif

    // Store function in seperate module
    ModuleHandler = addModule(move(CodeGen.Module));
    createModule();

    // Only run anonymous functions
    if (FuncNode->Anon) {
        auto Func = findSymbol(FuncNode->Id);
        auto FuncJIT = (size_t(*)())Func.getAddress();

        assert(FuncJIT);

        auto Res = FuncJIT();
        string Output = getOutput(Res, RetTy);
        cout << Output
             << endl
             << "type: " << RetTy.str()
             << endl;

        // Remove module
        removeModule(ModuleHandler);
    }

    return 0;
}
