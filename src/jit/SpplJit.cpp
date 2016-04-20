#include "SpplJit.h"

namespace jit {

    SpplJit::SpplJit() :
            Machine(EngineBuilder().selectTarget()),
            Layout(Machine->createDataLayout()),
            CompileLayer(ObjectLayer, SimpleCompiler(*Machine)),
            ScopeGen(&Driver.Global),
            CodeGen(Driver)
    {
        llvm::sys::DynamicLibrary::LoadLibraryPermanently(nullptr);
        createModule();

        // Time is short, mortal
        Driver.setOutput("/dev/null");
        Driver.setHeaderOutput("/dev/null");
    }

    SpplJit::ModuleHandleT SpplJit::addModule(std::unique_ptr<llvm::Module> M) {
        auto Resolver = createLambdaResolver(
                [&](const std::string &Name) {
                    if (auto Sym = findMangledSymbol(Name))
                        return RuntimeDyld::SymbolInfo(Sym.getAddress(), Sym.getFlags());
                    return RuntimeDyld::SymbolInfo(nullptr);
                },
                [](const std::string &S) { return nullptr; });
        auto Handler = CompileLayer.addModuleSet(singletonSet(std::move(M)),
                                                 std::make_unique<SectionMemoryManager>(),
                                                 std::move(Resolver));

        ModuleHandles.push_back(Handler);
        return Handler;
    }

    void SpplJit::removeModule(ModuleHandleT Handler) {
        ModuleHandles.erase(std::find(ModuleHandles.begin(), ModuleHandles.end(), Handler));
        CompileLayer.removeModuleSet(Handler);
    }

    JITSymbol SpplJit::findSymbol(const std::string Name) {
        return findMangledSymbol(mangle(Name));
    }


    JITSymbol SpplJit::findMangledSymbol(const std::string &Name) {
        for (auto Handler : make_range(ModuleHandles.rbegin(), ModuleHandles.rend()))
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
        CodeGen.Module = llvm::make_unique<llvm::Module>("SpplJit", getGlobalContext());
        CodeGen.Module->setDataLayout(Machine->createDataLayout());

        // Create a new pass manager attached to it
        PassMgr = llvm::make_unique<legacy::FunctionPassManager>(CodeGen.Module.get());

        // Add optimization passes
        PassMgr->add(createInstructionCombiningPass());
        PassMgr->add(createReassociatePass());
        PassMgr->add(createGVNPass());
        PassMgr->add(createCFGSimplificationPass());
        PassMgr->doInitialization();
    }


    string SpplJit::getOutput(intptr_t data, common::Type Type) {
        switch (Type.Id) {
            case common::TypeId::INT:
                return to_string((int64_t) data);
            case common::TypeId::FLOAT:
                // WTF is double type a pointer
                return to_string(*(double *) data);
            case common::TypeId::STRING:
                return "\"" + string((char *) data) + "\"";
            case common::TypeId::BOOL:
                return to_string((bool) data);
            case common::TypeId::EMPTYLIST:
                return "[]";
            case common::TypeId::TUPLE:
                return getOutputTuple(data, Type.Subtypes);
            case common::TypeId::SIGNATURE:
                return Type.str();
            default:
                throw runtime_error("Cannot convert to C data: " + Type.str());
        }
    }

    string SpplJit::getOutputTuple(intptr_t addr, vector<common::Type> Subtypes) {
        string Out("(");
        for (size_t i = 0; i < Subtypes.size(); ++i) {
            switch (Subtypes[i].Id) {
                case common::TypeId::INT:
                    Out += getOutput(*(int64_t *) addr, Subtypes[i]);
                    addr += sizeof(int64_t);
                    break;
                case common::TypeId::FLOAT:
                    Out += getOutput(addr, Subtypes[i]);
                    addr += sizeof(double);
                    break;
                case common::TypeId::STRING:
                    Out += getOutput(*(intptr_t *) addr, Subtypes[i]);
                    addr += sizeof(intptr_t *);
                    break;
                case common::TypeId::TUPLE:
                    Out += getOutputTuple(*(intptr_t *) addr, Subtypes[i].Subtypes);
                    addr += sizeof(intptr_t *);
                    break;
                default:
                    throw runtime_error("Cannot convert to C data: " + Subtypes[i].str());
            }
            if (i + 1 != Subtypes.size())
                Out += ", ";
        }

        return  Out + ")";
    }


    int SpplJit::eval(std::string Str) {
        if (!Driver.parseString(Str))
            return 1;

        if (!Driver.accept(ScopeGen))
            return 2;

        if (!Driver.accept(TypeChecker))
            return 3;

        if (!Driver.accept(Optimizer))
            return 4;

        if (!Driver.accept(CodeGen))
            return 5;
        auto FuncNode = Driver.Prog->Funcs[0].get();
        auto FuncIR = CodeGen.Module->getFunction(FuncNode->Id);
        PassMgr->run(* FuncIR);

        // Store function in seperate module
        ModuleHandler = addModule(std::move(CodeGen.Module));
        createModule();

        // Only run anonymous functions
        if (FuncNode->Anon) {
            auto Func = findSymbol(FuncNode->Id);
            auto FuncJIT = (size_t (*)()) Func.getAddress();

            assert(FuncJIT != NULL);
            string Output = getOutput(FuncJIT(), FuncNode->Ty);
            cout << Output << "\t\ttype: " << FuncNode->Ty.str() << endl;

            // Remove module
            removeModule(ModuleHandler);
        }

        return 0;
    }
}
