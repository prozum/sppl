#include "GasCodeGenerator.h"
#include <iostream>
#include <vector>

// TODO: Add compile messages and final error messages.
// TODO: move all debug output lines to debug only.

using namespace common;
using namespace std;

namespace codegen {

    typedef struct {    // Struct used for getting letterals from patterns and more in an easy and identical way.
        string n;       // Alternative is another visitor for each scenario where they appear
        string v;
    } argHelper_t;

    string function;    // Contains the current function
    string funcName;    // Contains the name of the current function

    vector<string> funcVector;  // Contains all functions that have been read
    vector<string> funcGlobl;   // Contains a globl for all functions

    int caseCount = 0;          // Used to count the current case
    int cases = 0;              // Number of cases
                                // These may be moved into function later

    bool hasMain = false;       // If no main is found, good luck assembler

    argHelper_t helper;         // Multipurpose helper struct

    GasCodeGenerator::GasCodeGenerator(std::ostream &out) : CodeGenerator::CodeGenerator(out) {
    }

    void GasCodeGenerator::visit(Program &node) {
        // Visit all functions

        for (auto func : node.funcs) {
            func->accept(*this);
        }

        if (!hasMain) {
            cout << "Error: no main\n";     // Feedback when no main is present
        } else {
            string source = buildSource();  // Build source.S file
            cout << source << endl;         // print result (for debugging purpose)
        }
    }

    void GasCodeGenerator::visit(Function &node) {
        funcName = node.id;                    // Function name used for anything function related including
                                                // names and labels.

        if (funcName.compare("main") == 0) {    // Checks if we have a main
            hasMain = true;                     // It is assumed a "multiple main" case have been caught if present
        }

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

        if (cases == caseCount) {   // Default case
            function += ".";
            function += funcName;
            function += "casedefault:\n";
            function += "FUNCTIONBODY\n";   // TODO: build function body
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

                if (helper.n.compare("Int") == 0) {     // Case where pattern is an Int
                    // Compare input argument with pattern
                    function += "cmpl $";
                    function += helper.v;
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
                    helper = {};    // Resets struct
                    continue;
                }
                // repeat for all possebilities
            }
            function += "FUNCTIONBODY\n";       // TODO: Build function body
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

        node.left->accept(*this);

        if (helper.n.compare("Int") == 0) {
            function += "movl $";
            function += helper.v;
            function += ", %eax\n";
        } else {
            throw "NonSupportedExpression";
        }

        function += "pushl %eax\n";
        node.right->accept(*this);

        if (helper.n.compare("Int") == 0) {
            function += "movl $";
            function += helper.v;
            function += ", %eax\n";
        } else {
            throw "NonSupportedExpression";
        }

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
        helper.n = "Int";
        helper.v = to_string(node.value);
        cout << "Got integer => " << helper.v << endl;
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
        cout << "IdNotImplemented" << endl;
    }

    void GasCodeGenerator::visit(Call &node) {
        cout << "CallNotImplemented" << endl;
    }

    void GasCodeGenerator::visit(Type &node) {
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
}
