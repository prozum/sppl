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

    // Add standard library to JIT
    addModule(CodeGen.getStdModule());

    // Set Driver to JIT mode
    Drv.JIT = true;
    Drv.Silent = true;
}

SpplJit::ModuleHandleT SpplJit::addModule(unique_ptr<Module> M) {
    auto Resolver = createLambdaResolver(
        [&](const std::string &Name) {
            if (auto Sym = CompileLayer.findSymbol(Name, false))
                return Sym;
            return JITSymbol(nullptr);
        },
        [](const std::string &Name) {
            if (auto SymAddr = RTDyldMemoryManager::getSymbolAddressInProcess(Name))
                return JITSymbol(SymAddr, JITSymbolFlags::Exported);
            return JITSymbol(nullptr);
        });

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

string SpplJit::getOutput(OutputData Data, common::Type Ty) {
    switch (Ty.Id) {
    case TypeId::INT:
        return to_string(Data.Int);
    case TypeId::FLOAT:
        return to_string(Data.Float);
    case TypeId::CHAR:
        return "'" + string(1, Data.Char) + "'";
    case TypeId::STRING:
        return getOutputString(Data, Ty);
    case TypeId::BOOL:
        return Data.Bool ? "True" : "False";
    case TypeId::TUPLE:
        return getOutputTuple(Data, Ty);
    case TypeId::LIST:
        return getOutputList(Data, Ty);
    case TypeId::SIGNATURE:
        return Ty.str();
    case TypeId::EMPTYLIST:
        return "[]";
    case TypeId::VOID:
        return "";
    default:
        assert(0 && "Type not supported");
    }
}

string SpplJit::getOutputTuple(OutputData Addr, common::Type Ty) {
    auto Alignment = CodeGen.getAlignment(Ty);
    auto Subtypes = Ty.Subtypes;
    string Out("(");
    for (size_t i = 0; i < Subtypes.size(); ++i) {
        auto Offset = Addr.Size % Alignment ? Alignment - Addr.Size % Alignment : 0;
        switch (Subtypes[i].Id) {
        case TypeId::INT:
            Addr.Size += Offset;
            Out += getOutput(*Addr.Ptr, Subtypes[i]);
            Addr.Size += sizeof(Addr.Ptr);
            break;
        case TypeId::FLOAT:
            Addr.Size += Offset;
            Out += getOutput(Addr, Subtypes[i]);
            Addr.Size += sizeof(Addr.Float);
            break;
        case TypeId::CHAR:
            Out += getOutput(*Addr.Ptr, Subtypes[i]);
            Addr.Size += sizeof(Addr.Char);
            break;
        case TypeId::BOOL:
            Out += getOutput(*Addr.Ptr, Subtypes[i]);
            Addr.Size += sizeof(Addr.Bool);
            break;
        case TypeId::STRING:
            Addr.Size += Offset;
            Out += getOutputString(*Addr.Ptr, Subtypes[i]);
            Addr.Size += sizeof(Addr.Ptr);
            break;
        case TypeId::TUPLE:
            Addr.Size += Offset;
            Out += getOutputTuple(*Addr.Ptr, Subtypes[i]);
            Addr.Size += sizeof(Addr.Ptr);
            break;
        case TypeId::LIST:
            Addr.Size += Offset;
            Out += getOutputList(*Addr.Ptr, Subtypes[i]);
            Addr.Size += sizeof(Addr.Ptr);
            break;
        default:
            assert(0 && "Type not supported");
        }
        if (i + 1 != Subtypes.size())
            Out += ", ";
    }

    return Out + ")";
}


string SpplJit::getOutputList(OutputData Addr, common::Type Ty)
{
    string Out("[");
    auto Subtype = Ty.Subtypes.front();

    OutputData Data;
    do {
        if (Subtype.Id != TypeId::FLOAT)
            Data = *Addr.Ptr;
        else
            Data = Addr;
        Out += getOutput(Data, Subtype);

        Addr.Size += sizeof(Addr.Ptr);
        Addr = *Addr.Ptr;

        if (Addr.Ptr) {
            Out += ", ";
        }

    } while (Addr.Ptr);

    return Out + "]";
}

string SpplJit::getOutputString(OutputData Addr, common::Type Ty)
{
    string Out("\"");

    do {
        auto Data = *Addr.Ptr;
        Out += string(1, Data.Char);

        Addr.Size += sizeof(Addr.Ptr);
        Addr = *Addr.Ptr;

    } while (Addr.Ptr);

    return Out + "\"";
}

struct String {
    char Char;
    String *Next;
};

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

    if (!Drv.Prog->Decls.size())
        return 0;

    auto FuncNode = static_cast<common::Function *>(Drv.Prog->Decls.front().get());
    auto RetTy = FuncNode->Signature.Subtypes.back();

    // Store function in separate module
    ModuleHandler = addModule(move(CodeGen.Module));

    // Only run anonymous functions
    if (FuncNode->Anon) {
        auto Func = findSymbol(FuncNode->Id);
        auto FuncJIT = (OutputData(*)())Func.getAddress();

        assert(FuncJIT);

        OutputData Res = FuncJIT();
        
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
