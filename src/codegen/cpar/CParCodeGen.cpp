#include "CParCodeGen.h"

using namespace ctree;
using namespace std;
using namespace common;

codegen::CParCodeGen::CParCodeGen(parser::Driver &Drv)
        : CCodeGen(Drv) { }

void codegen::CParCodeGen::visit(common::Program &Node) {
    common::Function *Main = nullptr;

    CProg = new ctree::Program();

    generateStd();
    CProg->Directives.push_back(new Define("printfunc", "printf"));

    // Find the main function
    for (auto &Decl : Node.Decls) {
        if (typeid(*Decl.get()) == typeid(common::Function) &&
            ((common::Function *)Decl.get())->Id == "main") {
            Main = (common::Function *)Decl.get();
            break;
        }
    }

    if (!Main) {
        addError(Error("No main function"));
        return;
    }

    /* The main function of the C program will be build below.
     * For readablity, the code that the C tree will generate,
     * can be seen below.
     *
     * int main(int argc, char** argv)
     * {
     *     strlist* args = gcreate_strlist(0);
     *     int i;
     *
     *     for (i = argc - 1; i >= 0; i--)
     *     {
     *         args = gadd_strlist(args, gctr_str(argv[i]));
     *     }
     *
     *     sig_arg* main_args = malloc(sizeof(sig_arg));
     *     main_args->arg0 = args;
     *     task_t* main_task = taskcreate((void (*)(void*))umain.par, (void*)main_args);
     *
     *     rmain(4, main_task);
     *
     *     taskdealloc(main_task);
     *     return 0;
     */

    string StrListName = getName(StringList);
    string StrListType = getType(StringList);

    // int main(int argc, char** argv)
    auto MainBlock = new Block();
    auto MainFunc = new ctree::Function("int", "main", MainBlock);
    MainFunc->Args.push_back({ "int", "argc" });
    MainFunc->Args.push_back({ "char**", "argv" });
    CProg->Functions.push_back(MainFunc);

    // strlist args = gcreate_strlist(0);
    // int i;
    auto StrListCreate = new Call(new Ident(GCreate + StrListName));
    StrListCreate->Args.push_back(new Int(0));
    MainBlock->Stmts.push_back(new Decl(StrListType, new BinOp("=", new Ident("args"), StrListCreate)));
    MainBlock->Stmts.push_back(new Decl(GIntType, new Ident("i")));

    // for (i = argc - 1; i >= 0; --i)
    auto Expr1 = new BinOp("=", new Ident("i"), new BinOp("-", new Ident("argc"), new Int(1)));
    auto Expr2 = new BinOp(">=", new Ident("i"), new Int(0));
    auto Expr3 = new UnOp("--", new Ident("i"));
    auto ForBlock = new Block();
    MainBlock->Stmts.push_back(new For(Expr1, Expr2, Expr3, ForBlock));

    // args = gadd_strlist(args, gctr_str(argv[i]));
    auto StrCreate = new Call(new Ident(GCreate + GString));
    StrCreate->Args.push_back(new UnOp("[i]", new Ident("argv"), false));
    auto AddFunc = new Call(new Ident(GAdd + StrListName));
    AddFunc->Args.push_back(new Ident("args"));
    AddFunc->Args.push_back(StrCreate);
    ForBlock->Stmts.push_back(new ExprStmt(new BinOp("=", new Ident("args"), AddFunc)));

    // sig_arg main_args;
    MainBlock->Stmts.push_back(new Decl(getArgName(Main->Signature), new Ident("main_args")));

    // main_args.arg0 = args;
    auto MainArgs = new BinOp(".", new Ident("main_args"), new Ident(GArg + to_string(0)));
    MainBlock->Stmts.push_back(new ExprStmt(new BinOp("=", MainArgs, new Ident("args"))));

    // task_t* main_task = taskcreate((void (*)(void*))umain.par, (void*)main_args);
    auto TaskCrt = new Call(new Ident("taskcreate"));
    auto UMainPar = new BinOp(".", new Ident(GUser + Main->Id), new Ident(GPar));
    TaskCrt->Args.push_back(new UnOp("(void (*)(void*))", UMainPar));
    TaskCrt->Args.push_back(new UnOp("(void*)", new UnOp("&", new Ident("main_args"))));
    MainBlock->Stmts.push_back(new Decl("task_t*", new BinOp("=", new Ident("main_task"), TaskCrt)));

    // rmain(4, main_task);
    auto RMain = new Call(new Ident("rmain"));
    RMain->Args.push_back(new Int(4));
    RMain->Args.push_back(new Ident("main_task"));
    MainBlock->Stmts.push_back(new ExprStmt(RMain));

    // taskdealloc(main_task);
    auto TaskDealloc = new Call(new Ident("taskdealloc"));
    TaskDealloc->Args.push_back(new Ident("main_task"));
    MainBlock->Stmts.push_back(new ExprStmt(TaskDealloc));

    // return 0;
    MainBlock->Stmts.push_back(new Return(new Int(0)));

    // Visit Functions
    for (auto &Func : Node.Decls) {
        Func->accept(*this);
    }

    CProg->outputCode(*Drv.Out, 0);
}

void codegen::CParCodeGen::visit(common::Function &Node) {
    auto &Signature = Node.Signature;

    string SigType = getType(Signature);
    string ParFuncName = GGenerated + GPar + Node.Id;
    string SeqFuncName = GGenerated + GSeq + Node.Id;

    CurrFunc = &Node;
    CurrentArg = getArgType(Signature);

    // sig ufuncname = { gparfuncname, gseqfuncname };
    auto Val = new ArrayLiteral();
    Val->Exprs.push_back(new Ident(ParFuncName));
    Val->Exprs.push_back(new Ident(SeqFuncName));
    CProg->Globals.push_back(new Decl(SigType, new BinOp("=", new Ident(GUser + Node.Id), Val)));

    // ---------------------------------------

    GenerateParallel = true;
    CurrBlock = new Block();
    auto ParFunc = new ctree::Function("void", ParFuncName, CurrBlock);
    ParFunc->Args.push_back({ "task_t*", GTask });
    CProg->Functions.push_back(ParFunc);

    for (size_t i = 0; i < Signature.Subtypes.size() - 1; ++i) {
        string ArgName = GArg + to_string(i);
        string ArgType = getType(Signature.Subtypes[i]);

        auto TaskArgs = new BinOp("->", new Ident(GTask), new Ident("startarg"));
        auto CastArgs = new UnOp("(" + CurrentArg + ")", TaskArgs);
        auto GetArg = new BinOp("->", CastArgs, new Ident(GArg + to_string(i)));
        auto CastArg = new UnOp("(" + getType(Signature.Subtypes[i]) + ")", GetArg);
        CurrBlock->Stmts.push_back(new Decl(ArgType, new BinOp("=", new Ident(ArgName), CastArg)));
    }

    // Entry:
    CurrBlock->Stmts.push_back(new Label("Entry"));

    for (auto &Case: Node.Cases) {
        Case->accept(*this);
    }

    // print("No cases realized in funcname\n");
    auto Print = new Call(new Ident(PrintFunc));
    Print->Args.push_back(new String("No cases realized in " + Node.Id + "\\n"));
    CurrBlock->Stmts.push_back(new ExprStmt(Print));

    // exit(1);
    auto Exit = new Call(new Ident("exit"));
    Exit->Args.push_back(new Int(1));
    CurrBlock->Stmts.push_back(new ExprStmt(Exit));

    // ---------------------------------------

    GenerateParallel = false;
    CurrBlock = new Block();
    auto SeqFunc = new ctree::Function(getType(Signature.Subtypes.back()), SeqFuncName, CurrBlock);
    CProg->Functions.push_back(SeqFunc);

    for (size_t i = 0; i < Signature.Subtypes.size() - 1; ++i) {
        SeqFunc->Args.push_back({ getType(Signature.Subtypes[i]), GArg + to_string(i) });
    }

    // Entry:
    CurrBlock->Stmts.push_back(new Label("Entry"));

    // Visit Cases
    for (auto &Case: Node.Cases) {
        Case->accept(*this);
    }

    // print("No cases realized in funcname\n");
    Print = new Call(new Ident(PrintFunc));
    Print->Args.push_back(new String("No cases realized in " + Node.Id + "\\n"));
    CurrBlock->Stmts.push_back(new ExprStmt(Print));

    // exit(1);
    Exit = new Call(new Ident("exit"));
    Exit->Args.push_back(new Int(1));
    CurrBlock->Stmts.push_back(new ExprStmt(Exit));
}

void codegen::CParCodeGen::visit(common::Case &Node) {
    vector<ctree::Expression*> Patterns;

    CurrBlock = new Block(CurrBlock);

    // Visit patterns and get all none null expressions
    // generated from visiting them.
    for (size_t i = 0; i < Node.Patterns.size(); ++i) {
        PatternBuilder = new Ident(GArg + to_string(i));

        Node.Patterns[i]->accept(*this);

        if (LastExpr) {
            Patterns.push_back(LastExpr);
        }

        delete PatternBuilder;
    }

    auto Compare = generateAndChain(Patterns);

    // if (pattern_1 && (pattern_2 && (... && pattern_n ...))
    if (Compare) {
        auto If = new IfElse(Compare, CurrBlock);
        CurrBlock->Parent->Stmts.push_back(If);
    } else {
        CurrBlock->Parent->Stmts.push_back(CurrBlock);
    }

    if (Node.When) {
        Node.When->accept(*this);

        if (GenerateParallel) {
            outputParallel();
        }

        CurrBlock = new Block(CurrBlock);

        // if (when_expr)
        auto If = new IfElse(LastExpr, CurrBlock);
        CurrBlock->Parent->Stmts.push_back(If);
    }

    if (Node.TailRec) {
        // We know, that if a case is tail recursive, then the expression
        // of that case is a call.
        auto Call = static_cast<CallExpr*>(Node.Expr.get());

        // Reset all the arguments to the arguments of the call.
        for (size_t i = 0; i < Call->Args.size(); ++i) {
            Call->Args[i]->accept(*this);

            if (GenerateParallel) {
                outputParallel();
            }

            // arg_i = callarg_i
            auto Assign = new BinOp("=", new Ident(GArg + to_string(i)), LastExpr);
            CurrBlock->Stmts.push_back(new ExprStmt(Assign));
        }

        // goto Entry;
        CurrBlock->Stmts.push_back(new Goto("Entry"));
    } else {
        string RetType = getType(CurrFunc->Signature.Subtypes.back());

        Node.Expr->accept(*this);

        if (GenerateParallel) {
            outputParallel();

            auto GetArgs = new BinOp("->", new Ident(GTask), new Ident("startarg"));
            auto CastArgs = new UnOp("(" + CurrentArg + ")", GetArgs);
            auto GetRes = new BinOp("->", CastArgs, new Ident(GRes));
            auto Assign = new BinOp("=", GetRes, LastExpr);
            CurrBlock->Stmts.push_back(new ExprStmt(Assign));

            auto TaskExit = new Call(new Ident("taskexit"));
            TaskExit->Args.push_back(new Ident(GTask));
            CurrBlock->Stmts.push_back(new ExprStmt(TaskExit));
        } else {
            // rettype res = expr;
            auto Assign = new BinOp("=", new Ident(GRes), LastExpr);
            CurrBlock->Stmts.push_back(new Decl(RetType, Assign));

            // return res;
            CurrBlock->Stmts.push_back(new Return(new Ident(GRes)));
        }
    }

    // Reset back to the old CurrBlock
    if (Node.When) {
        CurrBlock = CurrBlock->Parent;
    }

    CurrBlock = CurrBlock->Parent;
}

void codegen::CParCodeGen::visit(common::CallExpr &Node) {
    string TaskName = GTask + to_string(TaskCount++);

    if (GenerateParallel) {
        if (ParallelCalls.size() == CallDepth) {
            SequentialCall.push_back(nullptr);
            ParallelCalls.push_back(vector<Statement*>());
        }

        if (Node.DoParallel) {
            string TaskArgsName = TaskName + GArg;
            string ArgsName = getArgName(Node.Callee->RetTy);

            ParallelCalls[CallDepth].push_back(new Decl(ArgsName, new Ident(TaskArgsName)));


            CallDepth++;

            for (size_t i = 0; i < Node.Args.size(); ++i) {
                Node.Args[i]->accept(*this);

                auto GetArg = new BinOp(".", new Ident(TaskArgsName), new Ident(GArg + to_string(i)));
                ParallelCalls[CallDepth - 1].push_back(new ExprStmt(new BinOp("=", GetArg, LastExpr)));
            }

            Node.Callee->accept(*this);
            CallDepth--;

            auto TaskCrt = new Call(new Ident("taskcreate"));
            TaskCrt->Args.push_back(new UnOp("(void (*)(void*))", new BinOp(".", LastExpr, new Ident(GPar))));
            TaskCrt->Args.push_back(new UnOp("(void*)", new UnOp("&", new Ident(TaskArgsName))));
            ParallelCalls[CallDepth].push_back(new Decl("task_t*", new BinOp("=", new Ident(TaskName), TaskCrt)));

            auto SubTaskAdd = new Call(new Ident("subtaskadd"));
            SubTaskAdd->Args.push_back(new Ident(GTask));
            SubTaskAdd->Args.push_back(new Ident(TaskName));
            ParallelCalls[CallDepth].push_back(new ExprStmt(SubTaskAdd));

            LastExpr = new BinOp(".", new Ident(TaskArgsName), new Ident(GRes));
        } else {
            string TaskRes = TaskName + GRes;

            CallDepth++;
            CCodeGen::visit(Node);
            CallDepth--;

            if (SequentialCall[CallDepth] != nullptr) {
                addError(Error("The C Parallel Code Generator failed for unknown reasons"));
                return;
            }

            auto Res = new BinOp("=", new Ident(TaskRes), LastExpr);
            SequentialCall[CallDepth] = new Decl(getType(Node.RetTy), Res);

            LastExpr = new Ident(TaskRes);
        }
    } else {
        CCodeGen::visit(Node);
    }
}

std::string codegen::CParCodeGen::generateSignature(common::Type &Ty) {
    auto &SubTypes = Ty.Subtypes;
    string Name = GSignature + to_string(SigCount++);
    string Type = Name;

    GenTypes[Ty] = Name;

    /* struct sig
     * {
     *     void (*par)(task_t*);
     *     type_n (*call)(type_1, type_2, ... , type_n-1);
     * };
     * */

    string Args = "(";

    for (size_t i = 0; i < SubTypes.size() - 1; ++i) {
        Args += getType(SubTypes[i]);

        if (i < Ty.Subtypes.size() - 2)
            Args += ", ";
    }

    Args += ")";

    auto SigStrcut = new Struct(Name);
    SigStrcut->Fields.push_back({ "void (*" + GPar + ")", "(task_t*)" });
    SigStrcut->Fields.push_back({ getType(SubTypes.back()) + " (*" + GCall + ")", Args });
    CProg->Structs.push_back(SigStrcut);

    //----------------------------------------------

    /* struct sig_arg
     * {
     *     type_1 arg_1;
     *     type_2 arg_2;
     *     ...
     *     type_n arg_n;
     *     type_ret = res;
     * };
     * */

    auto SigArgStrcut = new Struct(Name + GArg);

    for (size_t i = 0; i < SubTypes.size() - 1; ++i) {
        SigArgStrcut->Fields.push_back({ getType(SubTypes[i]), GArg + to_string(i) });
    }

    SigArgStrcut->Fields.push_back({ getType(SubTypes.back()), GRes });

    CProg->Structs.push_back(SigArgStrcut);

    //----------------------------------------------

    /* signature gprint_signature(signature s, int is_top)
     * {
     *     print("(Type_1 -> Type_2 -> ... -> Type_n)");
     *     return s;
     * }
     * */

    // signature gprint_signature(signature s, int is_top)
    auto FuncBlock = new Block();
    auto Func = new ctree::Function(Type, GPrint + Name, FuncBlock);
    Func->Args.push_back({ Type, "s" });
    Func->Args.push_back({ GBoolType, "is_top" });
    CProg->Functions.push_back(Func);

    // "(Type_1 -> Type_2 -> ... -> Type_n)"
    string PrintString = "(";

    for (size_t i = 0; i < SubTypes.size(); ++i) {
        PrintString += SubTypes[i].str();

        if (i < SubTypes.size() - 1)
            PrintString += " -> ";
    }
    PrintString += ")";

    // print("(Type_1 -> Type_2 -> ... -> Type_n)");
    auto Print = new Call(new Ident(PrintFunc));
    Print->Args.push_back(new String(PrintString));
    FuncBlock->Stmts.push_back(new ExprStmt(Print));

    // return s;
    FuncBlock->Stmts.push_back(new Return(new Ident("s")));

    return Name;
}

void codegen::CParCodeGen::outputParallel() {
    while (ParallelCalls.size() > 0) {
        for (auto Stmt: ParallelCalls.back()) {
            CurrBlock->Stmts.push_back(Stmt);
        }

        if (SequentialCall.back() != nullptr) {
            CurrBlock->Stmts.push_back(SequentialCall.back());
        }

        if (ParallelCalls.back().size() != 0) {
            auto Yield = new Call(new Ident("taskyield"));
            Yield->Args.push_back(new Ident(GTask));
            CurrBlock->Stmts.push_back(new ExprStmt(Yield));
        }

        ParallelCalls.pop_back();
        SequentialCall.pop_back();
    }
}

void codegen::CParCodeGen::generateStd() {
    CProg->Directives.push_back(new Include("task.h", false));
    CCodeGen::generateStd();
}

std::string codegen::CParCodeGen::getArgType(common::Type &Ty) {
    return getArgName(Ty) + "*";
}

std::string codegen::CParCodeGen::getArgName(common::Type &Ty) {
    return getName(Ty) + GArg;
}





















