#include <assert.h>
#include "CParCodeGen.h"

using namespace common;
using namespace std;
using namespace codegen;

CParCodeGen::CParCodeGen(parser::Driver &Drv)
    : CCodeGenOld(Drv) {}


void CParCodeGen::visit(Program &Node) {
    Prog = &Node;
    Function *Main = nullptr;

#ifdef TESTS
    *Header << "#include \"task.h\" " << endl;
#else
    *Header << "#include \"src/runtime/task.h\" " << endl;
#endif
    // Generate the standard functionality for every program
    generateStd();

    // Find the main function
    for (auto &Decl : Node.Decls) {
        if (typeid(*Decl.get()) == typeid(Function) &&
            ((Function *)Decl.get())->Id == "main") {
            Main = (Function *)Decl.get();
            break;
        }
    }

    if (!Main) {
        addError(Error("No main function"));
        return;
    }

    // Get the type of main, so that return type of main is generated
    string MainSig = getType(Main->Signature);

    // Generate real main function, that calls the users main
    string StrListName = getList(StringList);

    *Output << "int main(int argc, char** argv) { " << endl
            << "    " << StrListName << " *args = " << GGenerated << GCreate << StrListName << "(0); " << endl
            << "    int i; " << endl
            << " " << endl
            << "    for(i = argc - 1; i >= 0; i--) { " << endl
            << "        args = " << GGenerated << GAdd << StrListName << "(args, " << GGenerated
                                            << GCreate << GString << "(argv[i])); " << endl
            << "    } " << endl
            << " " << endl
            << "    " << MainSig << GArg << "* main_args = malloc(sizeof(" << MainSig << GArg << ")); " << endl
            << "    main_args->" << GGenerated << GArg << "0 = args; " << endl
            << "    task_t *main_task = taskcreate((void (*)(void*))" << GUser << GMain << "."
                                    << GParallel << ", (void *)main_args); " << endl
            << "    rmain(4, main_task); " << endl
            << endl
            << "    taskdealloc(main_task); " << endl
            << "    return 0; " << endl
            << "} " << endl
            << " " << endl;


    for (auto &Func : Node.Decls) {
        Func->accept(*this);
        *Output << endl;
        outputBuffer();
    }
}

void CParCodeGen::visit(Function &Node) {
    string ParFunc;
    string SeqFunc;
    string ArgType;
    string Signature = getEnvironment(Node.Signature);

    CurrentArg = Signature + GArg;

    CurFunc = &Node;

    ParFunc = "void " + GGenerated + GParallel + Node.Id + "(task_t *t)";
    SeqFunc = getType(Node.Signature.Subtypes.back()) + " " + GGenerated + GSequential + Node.Id + "(";

    for (size_t i = 0; i < Node.Signature.Subtypes.size() - 1; ++i) {
        SeqFunc += getType(Node.Signature.Subtypes[i]) + " " + GGenerated + GArg + to_string(i);

        if (i < Node.Signature.Subtypes.size() - 2)
            SeqFunc += ", ";
    }

    SeqFunc += ")";

    *Header << ParFunc << ";" << endl;
    *Header << SeqFunc << ";" << endl;
    *Header << Signature << " " << GUser << Node.Id  << " = { " << GGenerated << GParallel << Node.Id << ", "
                                                                << GGenerated << GSequential << Node.Id << " };" << endl;

    GenerateParallel = true;
    *Output << ParFunc << " { " << endl;

    for (size_t i = 0; i < Node.Signature.Subtypes.size() - 1; ++i) {
        string ArgName = GGenerated + GArg + to_string(i);

        ArgType = getType(Node.Signature.Subtypes[i]);

        *Output << "    " << ArgType << " " << ArgName << " = "
                    << "(" << ArgType << ")" << "(" << "(" << CurrentArg << "*)" << "t->startarg)->"
                    << ArgName << ";" << endl;
    }

    *Output << "Start: " << endl
            << endl;

    // Generate cases
    for (auto &Case: Node.Cases) {
        Case->accept(*this);
        *Output << endl;

        // Clear assigments specific for current case
        Assignments.clear();
    }

    // Generate error, for when program doesn't realize a case
    *Output << "    print(\"No cases realized in " << CurFunc->Id << " \\n\"); " << endl
            << "    exit(1); " << endl
            << "} " << endl
            << endl;




    GenerateParallel = false;
    *Output << SeqFunc << " { " << endl;
    *Output << "Start: " << endl
            << endl;

    // Generate cases
    for (auto &Case: Node.Cases) {
        Case->accept(*this);
        *Output << endl;

        // Clear assigments specific for current case
        Assignments.clear();
    }

    // Generate error, for when program doesn't realize a case
    *Output << "    print(\"No cases realized in " << CurFunc->Id << " \\n\"); " << endl
            << "    exit(1); " << endl
            << "} " << endl
            << endl;
}

void CParCodeGen::visit(Case &Node) {
    stringstream Pattern;
    string WhenDealloc;
    bool Empty = true;

    // Generate if-statement for matching
    Pattern << "    if (";

    for (size_t i = 0; i < Node.Patterns.size(); ++i) {
        // Push arg_name on get_value_builder. get_value_builder is used for
        // generate assignments in a case
        PatternBuilder.push_back(GGenerated + GArg + to_string(i));

        // Generate pattern
        Node.Patterns[i]->accept(*this);

        // Cleanup
        PatternBuilder.pop_back();

        // Only add pattern, if pattern is not "1"
        if (!LastPattern.empty()) {

            if (!Empty)
                Pattern << " && ";

            Empty = false;
            Pattern << LastPattern;
        }
    }

    Pattern << ")";

    // Only generate if statement, if the pattern wasn't empty
    if (!Empty) {
        *Output << Pattern.str() << endl;
    }

    *Output << "    { " << endl;

    // Generate all nessesary assigments
    for (auto &Assign : Assignments) {
        *Output << "        " << Assign << endl;

        if (Assign == Assignments.back())
            *Output << endl;
    }

    if (Node.When) {
        // Generate return expression
        ExprStack.push(stringstream());
        SequentialCall.push_back(string());
        BeforeExpr.push_back(string());
        CallStackCount.push_back(0);

        Node.When->accept(*this);

        if (GenerateParallel) {
            outputBeforeExpr();

            for (auto &Dealloc: TaskDeallocs){
                WhenDealloc += "        " + Dealloc + "\n";
            }

            TaskDeallocs.clear();
        }

        *Output << "        if (" << ExprStack.top().str() << ") " << endl
                << "        { " << endl
                << WhenDealloc << endl
                ;
        ExprStack.pop();
    }

    // Generate return expression
    ExprStack.push(stringstream());
    SequentialCall.push_back(string());
    BeforeExpr.push_back(string());
    CallStackCount.push_back(0);

    if (Node.TailRec) {
        string Deallocs;
        auto C = (CallExpr *)Node.Expr.get();

        for (size_t i = 0; i < CurFunc->Signature.Subtypes.size() - 1; ++i) {
            ExprStack.push(stringstream());
            C->Args[i]->accept(*this);

            if (GenerateParallel) {
                outputBeforeExpr();

                for (auto &Dealloc: TaskDeallocs){
                    Deallocs += "        " + Dealloc + "\n";
                }

                TaskDeallocs.clear();
            }

            *Output << "        " << GGenerated << GArg << i << " = "
                                  << ExprStack.top().str() << "; " << endl;
            ExprStack.pop();
        }

        *Output << endl
                << Deallocs << endl
                << endl
                << "        goto Start; " << endl;
    } else {
        string Deallocs;
        ExprStack.push(stringstream());

        Node.Expr->accept(*this);

        if (GenerateParallel) {
            outputBeforeExpr();

            for (auto &Dealloc: TaskDeallocs){
                Deallocs += "        " + Dealloc + "\n";
            }

            TaskDeallocs.clear();

            *Output << "        ((" << CurrentArg << "*)t->startarg)->" << GGenerated << GRes << " = "
                                    << ExprStack.top().str() << "; " << endl
                                    << endl
                                    << Deallocs << endl
                                    << endl
                                    << "        taskexit(t); " << endl;
        } else {
            *Output << "        return " << ExprStack.top().str() << ";" << endl;
        }

        ExprStack.pop();
    }

    if (Node.When) {
        *Output << "        } " << endl;
        *Output << WhenDealloc;
    }

    *Output << "    } " << endl
            << endl;
}

void CParCodeGen::visit(common::CallExpr &Node) {
    if (GenerateParallel) {
        stringstream GeneratedCall;
        string Name = GTask + to_string(TaskCount++);
        string Signature = getEnvironment(Node.Callee->RetTy);

        CurrentTasks.push_back(Name);

        if (BeforeExpr.size() == BeforeExprDepth) {
            SequentialCall.push_back(string());
            BeforeExpr.push_back(string());
            CallStackCount.push_back(0);
        }

        BeforeExprDepth++;

        if (Node.DoParallel) {
            GeneratedCall << "        " << Signature << GArg << "* " << Name << GArg << " = malloc(sizeof("
                                        << Signature << GArg << "));" << endl;

            for (size_t i = 0; i < Node.Args.size(); ++i) {
                ExprStack.push(stringstream());
                Node.Args[i]->accept(*this);

                GeneratedCall << "        " << Name << GArg << "->" << GGenerated << GArg << i << " = "
                                            << ExprStack.top().str() << "; " << endl;

                ExprStack.pop();
            }

            ExprStack.push(stringstream());
            Node.Callee->accept(*this);

            GeneratedCall << "        task_t *" << Name << " = taskcreate((void (*)(void*))" << ExprStack.top().str()
                                                << "." << GParallel << ", (void *)" << Name << GArg << ");" << endl
                                                << "        subtaskadd(t, " << Name << "); " << endl
                                                << endl;

            ExprStack.pop();
            ExprStack.top() << "((" << Signature << GArg << "*)" << Name << "->startarg" << ")->" << GGenerated << GRes;

            TaskDeallocs.push_back("taskdealloc(" + Name + ");");

            BeforeExprDepth--;
            BeforeExpr[BeforeExprDepth] += GeneratedCall.str();
            CallStackCount[BeforeExprDepth]++;
        } else {

            ExprStack.push(stringstream());
            Node.Callee->accept(*this);

            GeneratedCall << "        " << getType(Node.RetTy) << " " << Name << GRes << " = " << ExprStack.top().str()
                                        << "." << GSequential << "(";
            ExprStack.pop();

            for (size_t i = 0; i < Node.Args.size(); ++i) {
                ExprStack.push(stringstream());
                Node.Args[i]->accept(*this);

                GeneratedCall << ExprStack.top().str();

                if (i < Node.Args.size() - 1)
                    GeneratedCall << ", ";

                ExprStack.pop();
            }

            GeneratedCall << ");" << endl
                          << endl;

            ExprStack.top() << Name << GRes;
            SequentialCall[--BeforeExprDepth] = GeneratedCall.str();
        }
    } else {
        Node.Callee->accept(*this);

        ExprStack.top() << "." << GSequential << "(";
        for (auto &Expr: Node.Args) {
            Expr->accept(*this);

            if (Expr != Node.Args.back())
                ExprStack.top() << ", ";
        }
        ExprStack.top() << ")";
    }
}

std::string CParCodeGen::generateEnvironment(common::Type &Ty) {
    string Name = GGenerated + GSignature + to_string(++SigCount);
    stringstream Res;

    Res << endl
        << "typedef struct " << Name << GArg << " {" << endl;

    for (size_t i = 0; i < Ty.Subtypes.size()  - 1; ++i) {
        Res << "    " << getType(Ty.Subtypes[i]) << " " << GGenerated << GArg << i << ";" << endl;
    }

    Res << "    " << getType(Ty.Subtypes.back()) << " " << GGenerated << GRes << "; " << endl;
    Res << "} " << Name << GArg << "; " << endl
        << endl;

    Res << "typedef struct " << Name << "{" << endl
        << "    void (*" << GParallel << ")(task_t*);" << endl
        << "    " << getType(Ty.Subtypes.back()) << "(*" << GSequential << ")" << "(";

    for (size_t i = 0; i < Ty.Subtypes.size() - 1; ++i) {
        Res << getType(Ty.Subtypes[i]);

        if (i < Ty.Subtypes.size() - 2)
            Res << ", ";
    }

    Res << ");" << endl
        << "} " << Name << ";" << endl
        << endl;

    *Header << Res.str();

    Closures[Ty] = Name;
    return Name;
}

void CParCodeGen::outputBeforeExpr() {
    while (BeforeExpr.size()) {
        *Output << BeforeExpr.back();

        if (!SequentialCall.back().empty())
            *Output << SequentialCall.back();

        if (CallStackCount.back() != 0)
            *Output << "        taskyield(t);" << endl
                    << endl;

        SequentialCall.pop_back();
        BeforeExpr.pop_back();
        CallStackCount.pop_back();
    }

    BeforeExprDepth = 0;
}

