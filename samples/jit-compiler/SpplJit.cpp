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


void SpplJit::eval(std::string str) {

    // Parse/Generate jit expression
    Driver.parse_string(str);
    auto expr = Driver.main->debug_expr;
    expr->accept(ScopeGenerator);
    expr->accept(TypeChecker);

    auto anon_func = Generator.GreateAnonymousFunction(expr);

    // Verify jit expression
    PassManager->run(*anon_func);

    // Print jit expression
    Generator.Module->dump();

    auto handler = add_module(std::move(Generator.Module));
    init_module_passmanager();

    auto anon_expr = find_symbol("__anon_func");

    int64_t (*int64_ptr)();
    double (*double_ptr)();
    switch (expr->node_type->type)
    {
        case common::Types::INT:
            int64_ptr = (int64_t (*)()) (intptr_t) anon_expr.getAddress();
            cout << "Evaluated to " << std::to_string(int64_ptr()) << endl;
            break;
        case common::Types::FLOAT:
            double_ptr = (double (*)()) (intptr_t) anon_expr.getAddress();
            cout << "Evaluated to " << std::to_string(double_ptr()) << endl;
            break;
    }

    remove_module(handler);
}


