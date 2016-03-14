#include "SpplJit.h"



SpplJit::SpplJit(istream *in, ostream *out) : Machine(EngineBuilder().selectTarget()),
                                              Layout(Machine->createDataLayout()),
                                              CompileLayer(ObjectLayer, SimpleCompiler(*Machine)),
                                              Generator(*out)
{
    llvm::sys::DynamicLibrary::LoadLibraryPermanently(nullptr);
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
    auto pass_manager = llvm::make_unique<legacy::FunctionPassManager>(Generator.Module.get());

    // Optimization
    pass_manager->add(createInstructionCombiningPass());
    pass_manager->add(createReassociatePass());
    pass_manager->add(createGVNPass());
    pass_manager->add(createCFGSimplificationPass());

    pass_manager->doInitialization();
}


void SpplJit::eval(std::string str) {
    Driver.parse_string(str);

    auto jit_expr = Generator.create_function(Driver.main->funcs[0]);

    auto handler = add_module(std::move(Generator.Module));
    init_module_passmanager();


    auto anon_func = find_symbol("__anon_expr");

    double (*df_ptr)();
    int (*if_ptr)();
    switch (jit_expr->getReturnType()->getTypeID())
    {
        case llvm::Type::DoubleTyID:
            df_ptr = (double (*)()) (intptr_t)anon_func.getAddress();
            cout << "Evaluated to " << std::to_string(df_ptr()) << endl;
            break;
        default:
            throw "Not supported";
    }


    remove_module(handler);
}


