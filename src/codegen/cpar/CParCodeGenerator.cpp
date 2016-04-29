#include "CParCodeGenerator.h"

using namespace common;
using namespace std;
using namespace codegen;

CParCodeGenerator::CParCodeGenerator(parser::Driver &Drv)
    : CCodeGenerator(Drv) {}


void CParCodeGenerator::visit(Program &Node) {
    Prog = &Node;
    Function *Main = nullptr;

    *Header << "#include \"task.h\" " << endl;

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

    if (!Main)
        throw "No main, help!";

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
            << "    task_t *main_task = taskcreate((void *)&" << GUser << GMain << ", (void *)main_args); " << endl
            << "    rmain(4, main_task); " << endl
            << "" << endl
            << "    " << Prints[Main->Signature.Subtypes.back()] << "(main_args->" << GGenerated << GRes << "); " << endl
            << "    printf(\"\\n\");" << endl
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

void CParCodeGenerator::visit(Function &Node) {
    string ParFunc;
    string SeqFunc;
    string ArgType;
    string Signature = getEnvironment(Node.Signature);

    CurrentArg = Signature + GArg;

    CurFunc = &Node;

    ParFunc = "void " + GUser + GParallel + Node.Id + "(task_t *t)";
    SeqFunc = getType(Node.Signature.Subtypes.back()) + " " + GUser + GSequential + Node.Id + "(task_t *t";
    for (size_t i = 0; i < Node.Signature.Subtypes.size() - 1; ++i) {
        string ArgName = GGenerated + GArg + to_string(i);

        ArgType = getType(Node.Signature.Subtypes[i]);
        ArgNames.push_back(ArgName);

        SeqFunc += ", " + ArgType + " " + ArgName;
    }

    SeqFunc += ")";

    *Header << ParFunc << ";" << endl;
    *Header << SeqFunc << ";" << endl;
    *Header << Signature << " " << GUser << Node.Id  << " = { " << GUser << GParallel << Node.Id << ", "
                                                                << GUser << GSequential << Node.Id << " };" << endl;

    GenerateParallel = true;
    *Output << ParFunc << " { " << endl;

    for (size_t i = 0; i < Node.Signature.Subtypes.size() - 1; ++i) {
        string ArgName = GGenerated + GArg + to_string(i);

        ArgType = getType(Node.Signature.Subtypes[i]);
        ArgNames.push_back(ArgName);

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
    *Output << "    printf(\"No cases realized!\\n\"); " << endl
            << "    exit(1); " << endl
            << "} " << endl
            << endl;

    // Clear ArgNames for current function
    ArgNames.clear();




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
    *Output << "    printf(\"No cases realized!\\n\"); " << endl
            << "    exit(1); " << endl
            << "} " << endl
            << endl;

    // Clear ArgNames for current function
    ArgNames.clear();
}

void CParCodeGenerator::visit(Case &Node) {
    stringstream Pattern;
    bool Empty = true;

    // Generate if-statement for matching
    Pattern << "    if (";

    for (size_t i = 0; i < Node.Patterns.size(); ++i) {
        // Push arg_name on get_value_builder. get_value_builder is used for
        // generate assignments in a case
        GetValueBuilder.push_back(ArgNames[i]);

        // Generate pattern
        Node.Patterns[i]->accept(*this);

        // Cleanup
        GetValueBuilder.pop_back();

        // Only add pattern, if pattern is not "1"
        if (LastPattern != "1") {

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

        Node.When->accept(*this);

        if (GenerateParallel) {
            for (size_t i = CallStack.size(); i > 0; i--) {
                *Output << CallStack.back();

                if (i == 1)
                    *Output << SequentialCall;

                if (CallStackCount.back() != 0)
                    *Output << "        taskyield(t);" << endl;

                CallStack.pop_back();
                CallStackCount.pop_back();
            }

            CallDepth = 0;
        }

        *Output << "        if (" << ExprStack.top().str() << ") " << endl
                << "        { " << endl;
        ExprStack.pop();
    }

    // Generate return expression
    ExprStack.push(stringstream());

    if (Node.TailRec) {
        auto C = (CallExpr *)Node.Expr.get();
        for (size_t i = 0; i < CurFunc->Signature.Subtypes.size() - 1; ++i) {
            ExprStack.push(stringstream());
            C->Args[i]->accept(*this);

            if (GenerateParallel) {
                for (size_t i = CallStack.size(); i > 0; i--) {
                    *Output << CallStack.back();

                    if (i == 1)
                        *Output << SequentialCall;

                    if (CallStackCount.back() != 0)
                        *Output << "        taskyield(t);" << endl;

                    CallStack.pop_back();
                    CallStackCount.pop_back();
                }

                CallDepth = 0;
            }

            *Output << "        " << GGenerated << GArg << i << " = "
                                  << ExprStack.top().str() << "; " << endl;
            ExprStack.pop();
        }

        *Output << endl
                << "        goto Start; " << endl;
    } else {
        ExprStack.push(stringstream());

        Node.Expr->accept(*this);

        if (GenerateParallel) {
            for (size_t i = CallStack.size(); i > 0; i--) {
                *Output << CallStack.back();

                if (i == 1)
                    *Output << SequentialCall;

                if (CallStackCount.back() != 0)
                    *Output << "        taskyield(t);" << endl;

                CallStack.pop_back();
                CallStackCount.pop_back();
            }

            CallDepth = 0;

            *Output << "        ((" << CurrentArg << "*)t->startarg)->" << GGenerated << GRes << " = "
                                    << ExprStack.top().str() << "; " << endl
                                    << "        taskexit(t); " << endl;
        } else {
            *Output << "        return " << ExprStack.top().str() << ";" << endl;
        }

        ExprStack.pop();
    }

    if (Node.When) {
        *Output << "        } " << endl;
    }

    *Output << "    } " << endl
            << endl;
}

void CParCodeGenerator::visit(common::CallExpr &Node) {
    if (GenerateParallel) {
        stringstream GeneratedCall;
        string Name = GTask + to_string(TaskCount++);
        string Signature = getEnvironment(Node.Callee->RetTy);

        CurrentTasks.push_back(Name);

        if (CallStack.size() == CallDepth) {
            CallStack.push_back(string());
            CallStackCount.push_back(0);
        }

        CallDepth++;

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
            << "        subtaskadd(t, " << Name << "); " << endl;

            ExprStack.pop();
            ExprStack.top() << "((" << Signature << GArg << "*)" << Name << "->startarg" << ")->" << GGenerated << GRes;

            CallDepth--;
            CallStack[CallDepth] += GeneratedCall.str();
            CallStackCount[CallDepth]++;
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

            GeneratedCall << ");" << endl;

            ExprStack.top() << Name << GRes;
            SequentialCall = GeneratedCall.str();
        }
    } else {
        Node.Callee->accept(*this);

        ExprStack.top() << ".seq(";
        for (auto &Expr: Node.Args) {
            Expr->accept(*this);

            if (Expr != Node.Args.back())
                ExprStack.top() << ", ";
        }
        ExprStack.top() << ")";
    }
}

std::string CParCodeGenerator::generateEnvironment(common::Type &Ty) {
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