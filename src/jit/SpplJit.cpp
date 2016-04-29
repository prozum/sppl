#include "SpplJit.h"

using namespace std;
using namespace llvm;
using namespace llvm::orc;
using namespace jit;
using namespace common;

SpplJit::SpplJit()
    : CodeGen(Drv), ScopeGen(&Drv.Global), TypeChecker(&Drv.Global),
      CompileLayer(ObjectLayer, SimpleCompiler(*CodeGen.Machine)) {

    //CodeGen.Module->getOrInsertFunction("getchar", IntegerType::getInt32Ty(C), NULL);

    sys::DynamicLibrary::LoadLibraryPermanently(nullptr);
    createModule();

    // Build-in functions
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

    // Time is short, mortal
    Drv.setOutput("/dev/null");
    Drv.setHeaderOutput("/dev/null");
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
        Mangler::getNameWithPrefix(MangledNameStream, Name, CodeGen.DataLayout);
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
    case common::TypeId::LIST:
        return getOutputList(Data, Type);
    case common::TypeId::SIGNATURE:
        return Type.str();
    case common::TypeId::VOID:
        return "";
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
        case common::TypeId::CHAR:
            Out += getOutput(*(char *)Addr, Subtypes[i]);
            Addr += sizeof(char);
            break;
        case common::TypeId::BOOL:
            Out += getOutput(*(bool *)Addr, Subtypes[i]);
            Addr += sizeof(bool);
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

string SpplJit::getOutputList(intptr_t Addr, common::Type Type)
{
    string Out("[");
    auto Subtype = Type.Subtypes[0];

    auto Count = *(int32_t *)Addr;
    Addr += sizeof(int64_t);
    Addr = *(int64_t *)Addr;

    for (int32_t i = 0; i < Count; ++i) {
        switch (Subtype.Id) {
        case common::TypeId::INT:
            Out += getOutput(*(int64_t *)Addr, Subtype);
            Addr += sizeof(int64_t);
            break;
        case common::TypeId::FLOAT:
            Out += getOutput(Addr, Subtype);
            Addr += sizeof(double);
            break;
        case common::TypeId::CHAR:
            Out += getOutput(*(char *)Addr, Subtype);
            Addr += sizeof(char);
            break;
        case common::TypeId::BOOL:
            Out += getOutput(*(bool *)Addr, Subtype);
            Addr += sizeof(bool);
            break;
        case common::TypeId::STRING:
            Out += getOutput(*(intptr_t *)Addr, Subtype);
            Addr += sizeof(intptr_t *);
            break;
        case common::TypeId::TUPLE:
            Out += getOutputTuple(*(intptr_t *)Addr, Subtype.Subtypes);
            Addr += sizeof(intptr_t *);
            break;
        default:
            throw runtime_error("Cannot convert to C data: " +
                                Subtype.str());
        }
        if (i + 1 != Count)
            Out += ", ";
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

    // if (!Drv.accept(Optimizer))
    //    return 4;

    if (!Drv.accept(CodeGen))
        return 5;
    auto FuncNode = static_cast<common::Function *>(Drv.Prog->Decls[0].get());
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
