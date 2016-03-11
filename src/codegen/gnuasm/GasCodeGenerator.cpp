#include "GasCodeGenerator.h"
#include <iostream>
#include <map>
#include <bits/stl_map.h>
#include <Scope.h>

// TODO: Add compile messages and final error messages.
// TODO: move all debug output lines to debug only.

using namespace common;
using namespace std;

namespace codegen {

    string function;    // Contains the current function
    string funcName;    // Contains the name of the current function

    vector<string> funcVector;  // Contains all functions that have been read
    vector<string> funcGlobl;   // Contains a globl for all functions

    int caseCount = 0;          // Used to count the current case
    int cases = 0;              // Number of cases
                                // These may be moved into function later

    map<string,string> varmap;

    typedef struct {
        string typeName;
        string typeValue;
    } helper_t;

    helper_t helper;

    GasCodeGenerator::GasCodeGenerator(std::ostream &out) : CodeGenerator::CodeGenerator(out) {
    }

    void GasCodeGenerator::visit(Program &node) {
        // Visit all functions

        for (auto func : node.funcs) {
            func->accept(*this);
        }

        string source = buildSource();  // Build source.S file
        output << source << endl;

    }

    void GasCodeGenerator::visit(Function &node) {
        funcName = node.id;                    // Function name used for anything function related including
                                                // names and labels.

        string globl = ".globl ";               // Build the globl
        globl += funcName;
        funcGlobl.push_back(globl);
        function += funcName;                   // Function entry
        function += ":\n";
        function += "pushl %ebp\n";             // Save base pointer
        function += "movl %esp, %ebp\n";        // and move stack pointer
        function += ".";
        function += funcName;
        function += "funcstart:\n";

        caseCount = 0;
        cases = node.cases.size();             // Get number of cases

        for (auto funcCase : node.cases) {     // Build cases
            funcCase->accept(*this);
        }

        function += ".";
        function += funcName;
        function += "funcend:\n";

        if (funcName.compare("main") == 0) {    // If the current function is main, we want to terminate the program when done
            function += "movl $0, %ebx\n";
            function += "movl $1, %eax\n";
            function += "int $0x80\n";
        } else {                                // Otherwise return to calling function
            function += "movl %ebp, %esp\n";
            function += "popl %ebp\n";
            function += "leave\n";
            function += "ret\n";
        }

        funcVector.push_back(function);         // adds function to vector with completed functions

        function.clear();                       // Prepare string variable for next function
    }

    void GasCodeGenerator::visit(Case &node) {
        caseCount++;

        int argc = 0;
        for (auto c : node.patterns) {
            c->accept(*this);
            cout << "PATTERN IN THIS SCOPE => " << helper.typeName << "    " << helper.typeValue << endl;

            if (helper.typeName.compare("Id") == 0) {
                int mempos = argc*4+8;
                string var = "";
                var += "movl ";
                var += to_string(mempos);
                var += ", %eax\n";
                varmap[helper.typeValue] = var;
            }
            helper = {};
            argc++;
        }

        if (cases == caseCount) {   // Default case
            function += ".";
            function += funcName;
            function += "casedefault:\n";
            node.expr->accept(*this);
        } else {                    // Other cases
            function += ".";
            function += funcName;
            function += "case";
            function += to_string(caseCount);
            function += ":\n";

            int argNum = 0;                         // first argument have index 0
            for (auto c : node.patterns) {
                c->accept(*this);                    // Gets the pattern, and puts it in "helper"

                cout << "Working on pattern" << endl;

                if (helper.typeName.compare("Int") == 0) {     // Case where pattern is an Int
                    // Compare input argument with pattern
                    function += "cmpl $";
                    function += helper.typeValue;
                    function += ", ";
                    int mempos = argNum*4+8;            // Stack starts at 8, each arg with 4 space
                    function += to_string(mempos);
                    function += "(%ebp)\n";

                    argNum++;                           // Prepare for next argument

                    // If not different move on
                    function += "jne .";
                    function += funcName;
                    function += "case";
                    if (caseCount + 1 == cases) {       // Last case is called default
                        function += "default";
                    } else {
                        function += to_string(caseCount + 1);
                    }
                    function += "\n";
                    continue;
                }
                // repeat for all possebilities
            }
            node.expr->accept(*this);

            helper = {};
        }

        cout << "CaseNotImplemented" << endl;
    }

    void GasCodeGenerator::visit(Or &node) {
        cout << "OrNotImplemented" << endl;
    }

    void GasCodeGenerator::visit(And &node) {
        cout << "AndNotImplemented" << endl;
    }

    void GasCodeGenerator::visit(Equal &node) {
        cout << "EqualNotImplemented" << endl;
    }

    void GasCodeGenerator::visit(NotEqual &node) {
        cout << "NotEqualNotImplemented" << endl;
    }

    void GasCodeGenerator::visit(Lesser &node) {
        cout << "LesserNotImplemented" << endl;
    }

    void GasCodeGenerator::visit(Greater &node) {
        cout << "GreaterNotImplemented" << endl;
    }

    void GasCodeGenerator::visit(LesserEq &node) {
        cout << "LesserEqNotImplemented" << endl;
    }

    void GasCodeGenerator::visit(GreaterEq &node) {
        cout << "GreaterEqNotImplemented" << endl;
    }

    void GasCodeGenerator::visit(Add &node) {
        cout << "ADD" << endl;

        node.left->accept(*this);

        function += "pushl %eax\n";
        node.right->accept(*this);

        function += "popl %ebx\n";
        function += "addl %ebx, %eax\n";
        cout << "AddNotImplemented" << endl;
    }

    void GasCodeGenerator::visit(Sub &node) {
        cout << "SubNotImplemented" << endl;
    }

    void GasCodeGenerator::visit(Mul &node) {
        cout << "MulNotImplemented" << endl;
    }

    void GasCodeGenerator::visit(Div &node) {
        cout << "DivNotImplemented" << endl;
    }

    void GasCodeGenerator::visit(Mod &node) {
        cout << "ModNotImplemented" << endl;
    }

    void GasCodeGenerator::visit(ListAdd &node) {
        cout << "ListAddNotImplemented" << endl;
    }

    void GasCodeGenerator::visit(Par &node) {
        cout << "ParNotImplemented" << endl;
    }

    void GasCodeGenerator::visit(Not &node) {
        cout << "NotNotImplemented" << endl;
    }

    void GasCodeGenerator::visit(Int &node) {
        function += "movl $";
        function += to_string(node.value);
        function += ", %eax\n";

        helper.typeName = "Int";
        helper.typeValue = to_string(node.value);


        cout << "Got integer => " << to_string(node.value) << endl;
    }

    void GasCodeGenerator::visit(Float &node) {
        cout << "FloatNotImplemented" << endl;
    }

    void GasCodeGenerator::visit(Bool &node) {
        cout << "BoolNotImplemented" << endl;
    }

    void GasCodeGenerator::visit(Char &node) {
        cout << "CharNotImplemented" << endl;
    }

    void GasCodeGenerator::visit(String &node) {
        cout << "StringNotImplemented" << endl;
    }

    void GasCodeGenerator::visit(ListPattern &node) {
        cout << "ListPatternNotImplemented" << endl;
    }

    void GasCodeGenerator::visit(TuplePattern &node) {
        cout << "TuplePatternNotImplemented" << endl;
    }

    void GasCodeGenerator::visit(ListSplit &node) {
        cout << "ListSplitNotImplemented" << endl;
    }

    void GasCodeGenerator::visit(List &node) {
        cout << "ListNotImplemented" << endl;
    }

    void GasCodeGenerator::visit(Tuple &node) {
        cout << "TupleNotImplemented" << endl;
    }

    void GasCodeGenerator::visit(Id &node) {
        helper.typeName = "Id";
        helper.typeValue = funcName + node.id;
        cout << "Got ID => " << node.id << endl;
    }

    void GasCodeGenerator::visit(Call &node) {

        // TODO: Find way pu push arguments to stack.
        vector<string> params;
        for (auto arg : node.exprs) {
            arg->accept(*this);

            if(helper.typeName.compare("Int") == 0) {
                function += "pushl %eax\n";
            } else if (helper.typeName.compare("Id") == 0) {

            }
        }

        node.callee->accept(*this); // function to call;
        function += "call ";
        function += helper.typeValue;
        function += "\n";

        helper = {};

        cout << "CallNotImplemented" << endl;
    }

    void GasCodeGenerator::visit(Type &node) {
        cout << "TypeNotImplemented" << endl;
    }

    string GasCodeGenerator::get_type(Type *) {
        return "GasCodeGenerator";
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
}
