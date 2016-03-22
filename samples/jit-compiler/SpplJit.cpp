#include "SpplJit.h"


SpplJit::SpplJit(istream *in, ostream *out) : Machine(EngineBuilder().selectTarget()),
                                              Layout(Machine->createDataLayout()),
                                              CompileLayer(ObjectLayer, SimpleCompiler(*Machine)),
                                              Generator(*out)
{
    llvm::sys::DynamicLibrary::LoadLibraryPermanently(nullptr);
    init_module_passmanager();
    Generator.jit_mode = true;
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
    ModuleHandles.erase(
            std::find(ModuleHandles.begin(), ModuleHandles.end(), handler));
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
    // Open a new module.
    Generator.Module = llvm::make_unique<llvm::Module>("SpplJit", getGlobalContext());
    Generator.Module->setDataLayout(Machine->createDataLayout());

    // Create a new pass manager attached to it.
    PassManager = llvm::make_unique<legacy::FunctionPassManager>(Generator.Module.get());

    // Optimization
    PassManager->add(createInstructionCombiningPass());
    PassManager->add(createReassociatePass());
    PassManager->add(createGVNPass());
    PassManager->add(createCFGSimplificationPass());

    PassManager->doInitialization();
}


size_t SpplJit::get_output(size_t addr, common::Type *node_type, string &out)
{
    switch (node_type->type) {
        case common::Types::INT:
            out += to_string(*(int64_t *)addr);
            return  addr + sizeof(int64_t);
        case common::Types::FLOAT:
            out += to_string(*(double *)addr);
            return addr + sizeof(double);
        case common::Types::STRING:
            out += "\"";
            out += *(char **) addr;
            out += "\"";
            return addr + sizeof(char *);
        case common::Types::TUPLE:
            return get_tuple_output(addr, node_type->types, out);
    }
}

size_t SpplJit::get_tuple_output(size_t addr, vector<common::Type *> node_types, string &out)
{
    out += "(";
    for (auto &node_type: node_types) {
        addr = get_output(addr, node_type, out);
        if (node_type != node_types.back())
            out += ",";
    }

    out += ")";

    return addr;
}

void SpplJit::eval(std::string str) {

    // Parse jit expression
    Driver.parse_string(str);
    auto expr = Driver.main->debug_expr;
    expr->accept(ScopeGenerator);
    expr->accept(TypeChecker);

    // Generate ir code for jit function
    auto ir_func = Generator.GreateAnonymousFunction(expr);

    // Verify ir code
    PassManager->run(*ir_func);

    // Print ir code
    Generator.Module->dump();

    // Setup new module for jit function
    auto handler = add_module(std::move(Generator.Module));
    init_module_passmanager();

    // Get jit function
    auto anon_func = find_symbol("__anon_func");
    auto func_ptr = (size_t (*)())anon_func.getAddress();

    string output;
    get_output(func_ptr(), expr->node_type, output);
    cout << "output: " << output << endl;

    remove_module(handler);
}


