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
        init_module_passmanager();

        // Life is too short
        Driver.set_output("/dev/null");
        Driver.set_header_output("/dev/null");
    }

    SpplJit::ModuleHandleT SpplJit::add_module(std::unique_ptr<llvm::Module> module) {
        auto resolver = createLambdaResolver(
                [&](const std::string &Name) {
                    if (auto Sym = find_mangled_symbol(Name))
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

    void SpplJit::remove_module(ModuleHandleT handler) {
        ModuleHandles.erase(std::find(ModuleHandles.begin(), ModuleHandles.end(), handler));
        CompileLayer.removeModuleSet(handler);
    }

    JITSymbol SpplJit::find_symbol(const std::string name) {
        return find_mangled_symbol(mangle(name));
    }


    JITSymbol SpplJit::find_mangled_symbol(const std::string &name) {
        for (auto handler: make_range(ModuleHandles.rbegin(), ModuleHandles.rend()))
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

    void SpplJit::init_module_passmanager() {
        // Open a new module
        Generator.Module = llvm::make_unique<llvm::Module>("SpplJit", getGlobalContext());
        Generator.Module->setDataLayout(Machine->createDataLayout());

        // Create a new pass manager attached to it
        PassManager = llvm::make_unique<legacy::FunctionPassManager>(Generator.Module.get());

        // Optimization
        PassManager->add(createInstructionCombiningPass());
        PassManager->add(createReassociatePass());
        PassManager->add(createGVNPass());
        PassManager->add(createCFGSimplificationPass());

        PassManager->doInitialization();
    }


    string SpplJit::get_output(intptr_t data, common::Type type) {
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
                return get_tuple_output(data, type.subtypes);
            case common::TypeId::SIGNATURE:
                return type.str();
            default:
                throw Error::NotImplemented("Cannot convert to C data: " + type.str());
        }
    }

    string SpplJit::get_tuple_output(intptr_t addr, vector<common::Type> types) {
        string out("(");
        for (size_t i = 0; i < types.size(); i++) {
            switch (types[i].id) {
                case common::TypeId::INT:
                    out += get_output(*(int64_t *) addr, types[i]);
                    addr += sizeof(int64_t);
                    break;
                case common::TypeId::FLOAT:
                    out += get_output(addr, types[i]);
                    addr += sizeof(double);
                    break;
                case common::TypeId::STRING:
                    out += get_output(*(intptr_t *) addr, types[i]);
                    addr += sizeof(intptr_t *);
                    break;
                case common::TypeId::TUPLE:
                    out += get_tuple_output(*(intptr_t *) addr, types[i].subtypes);
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


    void SpplJit::Eval(std::string str) {
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
        auto func_node = Driver.program->funcs[0].get();
        auto func_ir = Generator.Module->getFunction(func_node->id);
        PassManager->run(*func_ir);

        // Store function in seperate module
        module_handler = add_module(std::move(Generator.Module));
        init_module_passmanager();

        // Only run anonymous functions
        if (func_node->is_anon) {
            auto func = find_symbol(func_node->id);
            auto func_jit = (size_t (*)()) func.getAddress();

            assert(func_jit != NULL);
            string output = get_output(func_jit(), func_node->type);
            cout << output << "\t\ttype: " << func_node->type.str() << endl;

            // Remove module
            remove_module(module_handler);
        }

    }
}
