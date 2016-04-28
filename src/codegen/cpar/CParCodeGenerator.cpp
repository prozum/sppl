#include "CParCodeGenerator.h"

using namespace common;
using namespace std;
using namespace codegen;

CParCodeGenerator::CParCodeGenerator(parser::Driver &Drv)
    : CCodeGenerator(Drv) {}


void CParCodeGenerator::visit(Program &Node) {
    Prog = &Node;
    Function *Main = nullptr;

    *Header << "#include \"src/runtime/task.h\" \n";

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

    *Output << "int main(int argc, char** argv) { \n"
            "    " << StrListName << " *args = " << GGenerated << GCreate << StrListName << "(0); \n"
            "    int i; \n"
            " \n"
            "    for(i = argc - 1; i >= 0; i--) { \n"
            "        args = " << GGenerated << GAdd << StrListName << "(args, " << GGenerated
                                            << GCreate << GString << "(argv[i])); \n"
            "    } \n"
            " \n"
            "    " << MainSig << GArg << "* main_args = malloc(sizeof(" << MainSig << GArg << ")); \n"
            "    main_args->" << GGenerated << GArg << "0 = args; \n"
            "    task_t *main_task = taskcreate((void *)&" << GUser << GMain << ", (void *)main_args); \n"
            "    rmain(4, main_task); \n"
            "\n"
            "    " << Prints[Main->Signature.Subtypes.back()] << "(main_args->" << GGenerated << GRes << "); \n"
            "    taskdealloc(main_task); \n"
            "    return 0; \n"
            "} \n"
            " \n";


    for (auto &Func : Node.Decls) {
        Func->accept(*this);
        *Output << endl;
        outputBuffer();
    }
}

void CParCodeGenerator::visit(Function &Node) {
    stringstream Func;
    stringstream ArgName;
    string ArgType;
    string Signature = getEnvironment(Node.Signature);
    CurrentArg = Signature + GArg;

    CurFunc = &Node;

    // Generate function name and return type
    Func << "void " << GUser << Node.Id << "(task_t *t)";

    // Generate function decleration in header
    *Header << Func.str() << "; \n \n";

    // Generate function in *output
    *Output << Func.str() << " { \n";

    for (size_t i = 0; i < Node.Signature.subtypeCount() - 1; ++i) {
        ArgType = getType(Node.Signature.Subtypes[i]);
        ArgName << GGenerated << GArg << i;
        ArgNames.push_back(ArgName.str());
        *Output << "    " << ArgType << " " << ArgName.str() << " = "
                   "(" << ArgType << ")" << "(" << "(" << CurrentArg << "*)" << "t->startarg)->"
                << ArgName.str() << ";\n";

        ArgName.str("");
    }

    *Output << "Start: \n"
               "\n";

    // Generate cases
    for (auto &Case : Node.Cases) {
        SupTaskCount = 0;
        Case->accept(*this);
        *Output << endl;

        // Clear assigments specific for current case
        Assignments.clear();
    }

    // Generate error, for when program doesn't realize a case
    *Output << "    printf(\"No cases realized!\\n\"); \n"
            "    exit(1); \n"
            "} \n"
            " \n";

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
        // generate
        // Assignments in a case
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

    *Output << "    { \n";

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

        for (size_t i = CallStack.size(); i > 0; i--) {
            *Output << CallStack.back();
            *Output << "        taskyield(t);\n";
            CallStack.pop_back();
        }

        *Output << "        if (" << ExprStack.top().str() << ") \n"
                   "        { \n";
        ExprStack.pop();
    }

    // Generate return expression
    ExprStack.push(stringstream());

    if (Node.TailRec) {
        auto C = (CallExpr *)Node.Expr.get();
        for (size_t i = 0; i < CurFunc->Signature.Subtypes.size() - 1; ++i) {
            ExprStack.push(stringstream());
            C->Args[i]->accept(*this);

            for (size_t i = CallStack.size(); i > 0; i--) {
                *Output << CallStack.back();
                *Output << "        taskyield(t);\n";
                CallStack.pop_back();
            }

            *Output << "        " << GGenerated << GArg << i << " = "
                                  << ExprStack.top().str() << "; \n";
            ExprStack.pop();
        }

        *Output << "\n"
                "        goto Start; \n";
    } else {
        string ResType = getType(CurFunc->Signature.Subtypes.back());
        ExprStack.push(stringstream());

        Node.Expr->accept(*this);

        for (size_t i = CallStack.size(); i > 0; i--) {
            *Output << CallStack.back();
            *Output << "        taskyield(t);\n";
            CallStack.pop_back();
        }

        *Output << "        ((" << CurrentArg << "*)t->startarg)->" << GGenerated << GRes << " = " << ExprStack.top().str() << "; \n";
        *Output << "        taskexit(t); \n";
        ExprStack.pop();
    }

    if (Node.When) {
        *Output << "        } \n";
    }

    *Output << "    } \n"
            " \n";
}

void CParCodeGenerator::visit(common::CallExpr &Node) {
    stringstream GeneratedCall;
    string Name = GTask + to_string(TaskCount++);
    string Signature = getEnvironment(Node.Callee->RetTy);

    CurrentTasks.push_back(Name);

    if (CallStack.size() == CallDepth) {
        CallStack.push_back(string());
    }

    CallDepth++;


    GeneratedCall << "        " << Signature << GArg << "* " << Name << GArg << " = malloc(sizeof(" << Signature << GArg << "));\n";
    for (size_t i = 0; i < Node.Args.size(); ++i) {
        ExprStack.push(stringstream());
        Node.Args[i]->accept(*this);

        GeneratedCall << "        " << Name << GArg << "->" << GGenerated << GArg << i << " = " << ExprStack.top().str() << "; \n";

        ExprStack.pop();
    }

    ExprStack.push(stringstream());
    Node.Callee->accept(*this);

    GeneratedCall << "        task_t *" << Name << " = taskcreate((void *)&" << ExprStack.top().str()
                                              << ", (void *)" << Name << GArg << ");\n";
    GeneratedCall << "        subtaskadd(t, " << Name << "); \n";

    ExprStack.pop();
    ExprStack.top() << "((" << Signature << GArg << "*)" << Name << "->startarg" << ")->" << GGenerated << GRes;
    CallDepth--;
    CallStack[CallDepth] += GeneratedCall.str();
}

std::string CParCodeGenerator::generateEnvironment(common::Type &Ty) {
    string Name = GGenerated + GSignature + to_string(++SigCount);
    stringstream Res;

    Res << "typedef struct " << Name << GArg << " {\n";

    for (size_t i = 0; i < Ty.subtypeCount() - 1; ++i) {
        Res << "    " << getType(Ty.Subtypes[i]) << " " << GGenerated << GArg << i << ";\n";
    }

    Res << "    " << getType(Ty.Subtypes.back()) << " " << GGenerated << GRes << "; \n";
    Res << "} " << Name << GArg << "; \n";

    /*
    Res << "typedef " << getType(Ty.Subtypes.back()) << "(*" << Name << ")(";

    for (size_t i = 0; i < Ty.subtypeCount() - 1; ++i) {
        Res << getType(Ty.Subtypes[i]);

        if (i != Ty.subtypeCount() - 2)
            Res << ", ";
    }

    Res << "); \n";
     */

    *Header << Res.str();

    *Header << "typedef void (* " << Name << ")(task_t*);\n";

    Closures[Ty] = Name;
    return Name;
}