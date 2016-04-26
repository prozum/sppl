#include "SpplJit.h"

using namespace std;
using namespace llvm;
using namespace llvm::orc;
using namespace jit;

SpplJit::SpplJit()
    : Machine(EngineBuilder().selectTarget()),
      Layout(Machine->createDataLayout()),
      CompileLayer(ObjectLayer, SimpleCompiler(*Machine)), CodeGen(Drv),
      ScopeGen(&Drv.Global), TypeChecker(&Drv.Global) {
    llvm::sys::DynamicLibrary::LoadLibraryPermanently(nullptr);
    createModule();

    // Time is short, mortal
    Drv.setOutput("/dev/null");
    Drv.setHeaderOutput("/dev/null");
}

SpplJit::ModuleHandleT SpplJit::addModule(std::unique_ptr<llvm::Module> M) {
    auto Resolver = createLambdaResolver(
        [&](const std::string &Name) {
            if (auto Sym = findMangledSymbol(Name))
                return RuntimeDyld::SymbolInfo(Sym.getAddress(),
                                               Sym.getFlags());
            return RuntimeDyld::SymbolInfo(nullptr);
        },
        [](const std::string &S) { return nullptr; });
    auto Handler = CompileLayer.addModuleSet(
        singletonSet(std::move(M)), std::make_unique<SectionMemoryManager>(),
        std::move(Resolver));

    ModuleHandles.push_back(Handler);
    return Handler;
}

void SpplJit::removeModule(ModuleHandleT Handler) {
    ModuleHandles.erase(
        std::find(ModuleHandles.begin(), ModuleHandles.end(), Handler));
    CompileLayer.removeModuleSet(Handler);
}

JITSymbol SpplJit::findSymbol(const std::string Name) {
    return findMangledSymbol(mangle(Name));
}

JITSymbol SpplJit::findMangledSymbol(const std::string &Name) {
    for (auto Handler :
         make_range(ModuleHandles.rbegin(), ModuleHandles.rend()))
        if (auto Sym = CompileLayer.findSymbolIn(Handler, Name, true))
            return Sym;

    if (auto SymAddr = RTDyldMemoryManager::getSymbolAddressInProcess(Name))
        return JITSymbol(SymAddr, JITSymbolFlags::Exported);

    return nullptr;
}

std::string SpplJit::mangle(const std::string &Name) {
    std::string MangledName;
    {
        raw_string_ostream MangledNameStream(MangledName);
        Mangler::getNameWithPrefix(MangledNameStream, Name, Layout);
    }
    return MangledName;
}

void SpplJit::createModule() {
    // Open a new module
    CodeGen.Module =
        llvm::make_unique<llvm::Module>("SpplJit", getGlobalContext());
    CodeGen.Module->setDataLayout(Machine->createDataLayout());

    // Create a new pass manager attached to it
    PassMgr =
        llvm::make_unique<legacy::FunctionPassManager>(CodeGen.Module.get());

    // Add optimization passes
    PassMgr->add(createInstructionCombiningPass());
    PassMgr->add(createReassociatePass());
    PassMgr->add(createGVNPass());
    PassMgr->add(createCFGSimplificationPass());
    PassMgr->doInitialization();
}

string SpplJit::getOutput(intptr_t Data, common::Type Type) {
    switch (Type.Id) {
    case common::TypeId::INT:
        return to_string((int64_t)Data);
    case common::TypeId::FLOAT:
        // WTF is double type a pointer
        return to_string(*(double *)Data);
    case common::TypeId::CHAR:
        return "'" + string(1, (char)Data) + "'";
    case common::TypeId::STRING:
        return "\"" + string((char *)Data) + "\"";
    case common::TypeId::BOOL:
        return to_string((bool)Data);
    case common::TypeId::TUPLE:
        return getOutputTuple(Data, Type.Subtypes);
    case common::TypeId::SIGNATURE:
        return Type.str();
    default:
        throw runtime_error("Cannot convert to C data: " + Type.str());
    }
}

string SpplJit::getOutputTuple(intptr_t Addr, vector<common::Type> Subtypes) {
    string Out("(");
    for (size_t i = 0; i < Subtypes.size(); ++i) {
        switch (Subtypes[i].Id) {
        case common::TypeId::INT:
            Out += getOutput(*(int64_t *)Addr, Subtypes[i]);
            Addr += sizeof(int64_t);
            break;
        case common::TypeId::FLOAT:
            Out += getOutput(Addr, Subtypes[i]);
            Addr += sizeof(double);
            break;
        case common::TypeId::STRING:
            Out += getOutput(*(intptr_t *)Addr, Subtypes[i]);
            Addr += sizeof(intptr_t *);
            break;
        case common::TypeId::TUPLE:
            Out += getOutputTuple(*(intptr_t *)Addr, Subtypes[i].Subtypes);
            Addr += sizeof(intptr_t *);
            break;
        default:
            throw runtime_error("Cannot convert to C data: " +
                                Subtypes[i].str());
        }
        if (i + 1 != Subtypes.size())
            Out += ", ";
    }

    return Out + ")";
}

int SpplJit::eval(std::string Str) {
    if (!Drv.parseString(Str))
        return 1;

    if (!Drv.accept(ScopeGen))
        return 2;

    if (!Drv.accept(TypeChecker))
        return 3;

    // if (!Drv.accept(Optimizer))
    //    return 4;

    if (!Drv.accept(CodeGen))
        return 5;
    auto FuncNode = static_cast<common::Function *>(Drv.Prog->Decls[0].get());

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
    PassMgr->run(*CodeGen.Module->getFunction(FuncNode->Id));
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
    ModuleHandler = addModule(std::move(CodeGen.Module));
    createModule();

    // Only run anonymous functions
    if (FuncNode->Anon) {
        auto Func = findSymbol(FuncNode->Id);
        auto FuncJIT = (size_t(*)())Func.getAddress();

        assert(FuncJIT != NULL);
        string Output =
            getOutput(FuncJIT(), FuncNode->Signature.Subtypes.back());
        cout << Output
             << "\t\ttype: " << FuncNode->Signature.Subtypes.back().str()
             << endl;

        // Remove module
        removeModule(ModuleHandler);
    }

    return 0;
}
