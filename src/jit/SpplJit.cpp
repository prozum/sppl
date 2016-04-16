#include "SpplJit.h"

namespace jit {

    SpplJit::SpplJit() :
            Machine(EngineBuilder().selectTarget()),
            Layout(Machine->createDataLayout()),
            CompileLayer(ObjectLayer, SimpleCompiler(*Machine)),
            Generator(Driver),
            ScopeGenerator(&Driver.global)
    {
        llvm::sys::DynamicLibrary::LoadLibraryPermanently(nullptr);
        createModule();

        // Time is short, mortal
        Driver.set_output("/dev/null");
        Driver.set_header_output("/dev/null");
    }

    SpplJit::ModuleHandleT SpplJit::addModule(std::unique_ptr<llvm::Module> module) {
        auto resolver = createLambdaResolver(
                [&](const std::string &Name) {
                    if (auto Sym = findMangledSymbol(Name))
                        return RuntimeDyld::SymbolInfo(Sym.getAddress(), Sym.getFlags());
                    return RuntimeDyld::SymbolInfo(nullptr);
                },
                [](const std::string &S) { return nullptr; });
        auto handler = CompileLayer.addModuleSet(singletonSet(std::move(module)),
                                                 std::make_unique<SectionMemoryManager>(),
                                                 std::move(resolver));

        ModuleHandles.push_back(handler);
        return handler;
    }

    void SpplJit::removeModule(ModuleHandleT handler) {
        ModuleHandles.erase(std::find(ModuleHandles.begin(), ModuleHandles.end(), handler));
        CompileLayer.removeModuleSet(handler);
    }

    JITSymbol SpplJit::findSymbol(const std::string name) {
        return findMangledSymbol(mangle(name));
    }


    JITSymbol SpplJit::findMangledSymbol(const std::string &name) {
        for (auto handler : make_range(ModuleHandles.rbegin(), ModuleHandles.rend()))
            if (auto Sym = CompileLayer.findSymbolIn(handler, name, true))
                return Sym;

        if (auto sym_addr = RTDyldMemoryManager::getSymbolAddressInProcess(name))
            return JITSymbol(sym_addr, JITSymbolFlags::Exported);

        return nullptr;
    }

    std::string SpplJit::mangle(const std::string &name) {
        std::string mangled_name;
        {
            raw_string_ostream MangledNameStream(mangled_name);
            Mangler::getNameWithPrefix(MangledNameStream, name, Layout);
        }
        return mangled_name;
    }

    void SpplJit::createModule() {
        // Open a new module
        Generator.Module = llvm::make_unique<llvm::Module>("SpplJit", getGlobalContext());
        Generator.Module->setDataLayout(Machine->createDataLayout());

        // Create a new pass manager attached to it
        PassMgr = llvm::make_unique<legacy::FunctionPassManager>(Generator.Module.get());

        // Add optimization passes
        PassMgr->add(createInstructionCombiningPass());
        PassMgr->add(createReassociatePass());
        PassMgr->add(createGVNPass());
        PassMgr->add(createCFGSimplificationPass());
        PassMgr->doInitialization();
    }


    string SpplJit::getOutput(intptr_t data, common::Type type) {
        string out;

        switch (type.id) {
            case common::TypeId::INT:
                return to_string((int64_t) data);
            case common::TypeId::FLOAT:
                // WTF is double type a pointer
                return to_string(*(double *) data);
            case common::TypeId::STRING:
                out += "\"";
                out += (char *) data;
                out += "\"";
                return out;
            case common::TypeId::TUPLE:
                return getOutputTuple(data, type.subtypes);
            case common::TypeId::SIGNATURE:
                return type.str();
            default:
                throw Error::NotImplemented("Cannot convert to C data: " + type.str());
        }
    }

    string SpplJit::getOutputTuple(intptr_t addr, vector<common::Type> types) {
        string out("(");
        for (size_t i = 0; i < types.size(); i++) {
            switch (types[i].id) {
                case common::TypeId::INT:
                    out += getOutput(*(int64_t *) addr, types[i]);
                    addr += sizeof(int64_t);
                    break;
                case common::TypeId::FLOAT:
                    out += getOutput(addr, types[i]);
                    addr += sizeof(double);
                    break;
                case common::TypeId::STRING:
                    out += getOutput(*(intptr_t *) addr, types[i]);
                    addr += sizeof(intptr_t *);
                    break;
                case common::TypeId::TUPLE:
                    out += getOutputTuple(*(intptr_t *) addr, types[i].subtypes);
                    addr += sizeof(intptr_t *);
                    break;
                default:
                    throw Error::NotImplemented("Cannot convert to C data: " + types[i].str());
            }
            if (types[i] != types[i].subtypes.back())
                out += ", ";
        }

        return  out + ")";
    }


    void SpplJit::eval(std::string str) {
        if (!Driver.parse_string(str))
            return;

        if (!Driver.accept(ScopeGenerator)) {
            return;
        }

        if (!Driver.accept(TypeChecker)) {
            return;
        }

        // Generate ir_func
        if (!Driver.accept(Generator)) {
            return;
        }
        auto FuncNode = Driver.program->funcs[0].get();
        auto FuncIR = Generator.Module->getFunction(FuncNode->id);
        PassMgr->run(* FuncIR);

        // Store function in seperate module
        moduleHandler = addModule(std::move(Generator.Module));
        createModule();

        // Only run anonymous functions
        if (FuncNode->is_anon) {
            auto Func = findSymbol(FuncNode->id);
            auto FuncJIT = (size_t (*)()) Func.getAddress();

            assert(FuncJIT != NULL);
            string output = getOutput(FuncJIT(), FuncNode->type);
            cout << output << "\t\ttype: " << FuncNode->type.str() << endl;

            // Remove module
            removeModule(moduleHandler);
        }

    }
}
