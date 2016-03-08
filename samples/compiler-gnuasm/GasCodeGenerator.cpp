#include "GasCodeGenerator.h"
#include <iostream>
#include <vector>

using namespace common;
using namespace std;

std::string function;

vector<string> funcVector;
vector<string> funcGlobl;

int caseCount = 0;
int cases = 0;

bool hasMain = false;

GasCodeGenerator::GasCodeGenerator(std::ostream &os) : visitor::CodeGenerator::CodeGenerator(os), os(os) {
}

void GasCodeGenerator::visit(Program *node) {
    // Visit all functions

    for (auto func : node->funcs) {
        func->accept(this);
    }

    if (!hasMain) {
        cout << "Error: no main\n";
    }

    string source = buildSource();
    cout << source << endl;
}

void GasCodeGenerator::visit(Function *node) {
    string funcName = node->id;

    if (funcName.compare("main") == 0) {
        hasMain = true;
    }

    string globl = ".globl ";
    globl.append(funcName);
    funcGlobl.push_back(globl);
    function.append(funcName);
    function.append(":\n");
    function.append("pushl %ebp\n");
    function.append("movl %esp, %ebp\n");
    function.append(".");
    function.append(funcName);
    function.append("funcstart:\n");

    caseCount = 0;
    cases = node->cases.size();

    for (auto funcCase : node->cases) {
        funcCase->accept(this);
    }

    function.append(".");
    function.append(funcName);
    function.append("funcend:\n");
    function.append("movl %ebp, %esp\n");
    function.append("popl %ebp\n");
    function.append("leave\n");
    function.append("ret\n");

    funcVector.push_back(function);

    function.clear();
}

void GasCodeGenerator::visit(Case *node) {
    caseCount++;

    if (node->patterns.size() > 1) {
        if (caseCount == cases) {
            function.append(".casedefault:\n");
            node->expr->accept(this);
        } else {
            function.append(".case");
            function.append(to_string(caseCount));
            function.append(":\n");
            node->expr->accept(this);
        }
    } else {
        // Function is given no input, thus it is only concerned about the return value.
        function += ".default:\n";
        node->expr->accept(this);

    }
}

void GasCodeGenerator::visit(Or *node) {

}

void GasCodeGenerator::visit(And *node) {

}

void GasCodeGenerator::visit(Equal *node) {

}

void GasCodeGenerator::visit(NotEqual *node) {

}

void GasCodeGenerator::visit(Lesser *node) {

}

void GasCodeGenerator::visit(Greater *node) {

}

void GasCodeGenerator::visit(LesserEq *node) {

}

void GasCodeGenerator::visit(GreaterEq *node) {

}

void GasCodeGenerator::visit(Add *node) {

}

void GasCodeGenerator::visit(Sub *node) {

}

void GasCodeGenerator::visit(Mul *node) {

}

void GasCodeGenerator::visit(Div *node) {

}

void GasCodeGenerator::visit(Mod *node) {

}

void GasCodeGenerator::visit(ListAdd *node) {

}

void GasCodeGenerator::visit(Par *node) {

}

void GasCodeGenerator::visit(Not *node) {

}

void GasCodeGenerator::visit(Int *node) {

}

void GasCodeGenerator::visit(Float *node) {

}

void GasCodeGenerator::visit(Bool *node) {

}

void GasCodeGenerator::visit(Char *node) {

}

void GasCodeGenerator::visit(String *node) {

}

void GasCodeGenerator::visit(ListPattern *node) {

}

void GasCodeGenerator::visit(TuplePattern *node) {

}

void GasCodeGenerator::visit(ListSplit *node) {

}

void GasCodeGenerator::visit(List *node) {

}

void GasCodeGenerator::visit(Tuple *node) {

}

void GasCodeGenerator::visit(Id *node) {

}

void GasCodeGenerator::visit(Call *node) {

}

void GasCodeGenerator::visit(Type *node) {

}

string GasCodeGenerator::get_type(Type *) {

}

string GasCodeGenerator::buildSource() {
    string source = "";
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