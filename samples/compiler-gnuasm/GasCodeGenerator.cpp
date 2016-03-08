#include "GasCodeGenerator.h"
#include <iostream>
#include <vector>

using namespace common;
using namespace std;

string function;    // Contains the current function
string funcName;    // Contains the name of the current function

vector<string> funcVector;  // Contains all functions that have been read
vector<string> funcGlobl;   // Contains a globl for all functions

int caseCount = 0;          // Used to count the current case
int cases = 0;              // Number of cases
                            // These may be moved into function later

bool hasMain = false;       // If no main is found, good luck assembler

GasCodeGenerator::GasCodeGenerator(std::ostream &os) : visitor::CodeGenerator::CodeGenerator(os), os(os) {
}

void GasCodeGenerator::visit(Program *node) {
    // Visit all functions

    for (auto func : node->funcs) {
        func->accept(this);
    }

    if (!hasMain) {
        cout << "Error: no main\n";     // Feedback
    } else {
        string source = buildSource();  // Build source.S file
        cout << source << endl;         // print result (for debugging purpose)
    }
}

void GasCodeGenerator::visit(Function *node) {
    funcName = node->id;

    if (funcName.compare("main") == 0) {    // Checks if we have a main
        hasMain = true;
    }

    string globl = ".globl ";               // Build the globl
    globl += funcName;
    funcGlobl.push_back(globl);
    function += funcName;                   // Function entry
    function += ":\n";
    function += "pushl %ebp\n";
    function += "movl %esp, %ebp\n";
    function += ".";
    function += funcName;
    function += "funcstart:\n";

    caseCount = 0;
    cases = node->cases.size();             // Get number of cases

    for (auto funcCase : node->cases) {
        funcCase->accept(this);
    }

    function += ".";
    function += funcName;
    function += "funcend:\n";

    if (funcName.compare("main") == 0) {    // If the current function is main, we want to terminate the program when done
        function += "popl %ebx\n";
        function += "movl $1, %eax\n";
        function += "int $0x80\n";
    } else {                                // Otherwise return to calling function
        function += "movl %ebp, %esp\n";
        function += "popl %ebp\n";
        function += "leave\n";
        function += "ret\n";
    }

    funcVector.push_back(function);

    function.clear();
}

void GasCodeGenerator::visit(Case *node) {
    caseCount++;
    cout << "CaseNotImplemented" << endl;
}

void GasCodeGenerator::visit(Or *node) {
    cout << "OrNotImplemented" << endl;
}

void GasCodeGenerator::visit(And *node) {
    cout << "AndNotImplemented" << endl;
}

void GasCodeGenerator::visit(Equal *node) {
    cout << "EqualNotImplemented" << endl;
}

void GasCodeGenerator::visit(NotEqual *node) {
    cout << "NotEqualNotImplemented" << endl;
}

void GasCodeGenerator::visit(Lesser *node) {
    cout << "LesserNotImplemented" << endl;
}

void GasCodeGenerator::visit(Greater *node) {
    cout << "GreaterNotImplemented" << endl;
}

void GasCodeGenerator::visit(LesserEq *node) {
    cout << "LesserEqNotImplemented" << endl;
}

void GasCodeGenerator::visit(GreaterEq *node) {
    cout << "GreaterEqNotImplemented" << endl;
}

void GasCodeGenerator::visit(Add *node) {
    cout << "AddNotImplemented" << endl;
}

void GasCodeGenerator::visit(Sub *node) {
    cout << "SubNotImplemented" << endl;
}

void GasCodeGenerator::visit(Mul *node) {
    cout << "MulNotImplemented" << endl;
}

void GasCodeGenerator::visit(Div *node) {
    cout << "DivNotImplemented" << endl;
}

void GasCodeGenerator::visit(Mod *node) {
    cout << "ModNotImplemented" << endl;
}

void GasCodeGenerator::visit(ListAdd *node) {
    cout << "ListAddNotImplemented" << endl;
}

void GasCodeGenerator::visit(Par *node) {
    cout << "ParNotImplemented" << endl;
}

void GasCodeGenerator::visit(Not *node) {
    cout << "NotNotImplemented" << endl;
}

void GasCodeGenerator::visit(Int *node) {
    cout << "IntNotImplemented" << endl;
}

void GasCodeGenerator::visit(Float *node) {
    cout << "FloatNotImplemented" << endl;
}

void GasCodeGenerator::visit(Bool *node) {
    cout << "BoolNotImplemented" << endl;
}

void GasCodeGenerator::visit(Char *node) {
    cout << "CharNotImplemented" << endl;
}

void GasCodeGenerator::visit(String *node) {
    cout << "StringNotImplemented" << endl;
}

void GasCodeGenerator::visit(ListPattern *node) {
    cout << "ListPatternNotImplemented" << endl;
}

void GasCodeGenerator::visit(TuplePattern *node) {
    cout << "TuplePatternNotImplemented" << endl;
}

void GasCodeGenerator::visit(ListSplit *node) {
    cout << "ListSplitNotImplemented" << endl;
}

void GasCodeGenerator::visit(List *node) {
    cout << "ListNotImplemented" << endl;
}

void GasCodeGenerator::visit(Tuple *node) {
    cout << "TupleNotImplemented" << endl;
}

void GasCodeGenerator::visit(Id *node) {
    cout << "IdNotImplemented" << endl;
}

void GasCodeGenerator::visit(Call *node) {
    cout << "CallNotImplemented" << endl;
}

void GasCodeGenerator::visit(Type *node) {
    cout << "TypeNotImplemented" << endl;
}

string GasCodeGenerator::get_type(Type *) {
    cout << "get_typeNotImplemented" << endl;
}

string GasCodeGenerator::buildSource() {
    string source = "";

    source += ".data\n";
    source += "fmt:\n";
    source += ".string \"%d\\n\"\n";        // Allow printing of numbers in printf

    source += ".text\n";
    for (auto f : funcGlobl) {
        source += f;
        source += "\n";
    }

    for (auto f : funcVector) {
        source += f;
    }
    return source;
}