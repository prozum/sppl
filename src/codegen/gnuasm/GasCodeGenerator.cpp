#include "GasCodeGenerator.h"

// TODO: Add compile messages and final error messages.
// TODO: move all debug output lines to debug only.

using namespace common;
using namespace std;

namespace codegen {

    GasCodeGenerator::GasCodeGenerator(parser::Driver &Drv) : parser::CodeGenerator(Drv) {
    }

    void GasCodeGenerator::visit(Program &node) {
        // Visit all functions

        for (auto &func : node.funcs) {
            func->accept(*this);
        }

        string source = buildSource();  // Build source.S file
        *driver.out << source << endl;
    }

    void GasCodeGenerator::visit(Function &node) {
        FuncName = node.id;                     // Function name used for anything function related including
                                                // names and labels.

        string globl = ".globl ";               // Build the globl
        globl += FuncName;
        FuncGlobl.push_back(globl);
        Func += FuncName;                   // Function entry
        Func += ":\n";
        Func += "pushl %ebp\n";             // Save base pointer
        Func += "movl %esp, %ebp\n";        // and move stack pointer
        Func += ".";
        Func += FuncName;
        Func += "funcstart:\n";

        CaseCount = 0;
        Cases = node.cases.size();             // Get number of cases

        for (auto &funcCase : node.cases) {     // Build cases
            funcCase->accept(*this);
        }

        Func += ".";
        Func += FuncName;
        Func += "funcend:\n";

        if (FuncName.compare("main") == 0) {    // If the current function is main, we want to terminate the program when done
            Func += "movl $0, %ebx\n";
            Func += "movl $1, %eax\n";
            Func += "int $0x80\n";
        } else {                                // Otherwise return to calling function
            Func += "movl %ebp, %esp\n";
            Func += "popl %ebp\n";
            Func += "leave\n";
            Func += "ret\n";
        }

        FuncVector.push_back(Func);         // adds function to vector with completed functions

        Func.clear();                       // Prepare string variable for next function
    }

    void GasCodeGenerator::visit(Case &node) {
        CaseCount++;

        int argc = 0;
        for (auto &c : node.patterns) {
            c->accept(*this);
            cout << "PATTERN IN THIS SCOPE => " << Helper.TypeName << "    " << Helper.TypeValue << endl;

            if (Helper.TypeName.compare("Id") == 0) {
                int mempos = argc*4+8;
                string var = "";
                var += "movl ";
                var += to_string(mempos);
                var += ", %eax\n";
                VarMap[Helper.TypeValue] = var;
            }
            Helper = {};
            argc++;
        }

        if (Cases == CaseCount) {   // Default case
            Func += ".";
            Func += FuncName;
            Func += "casedefault:\n";
            node.expr->accept(*this);
        } else {                    // Other cases
            Func += ".";
            Func += FuncName;
            Func += "case";
            Func += to_string(CaseCount);
            Func += ":\n";

            int argNum = 0;                         // first argument have index 0
            for (auto &c : node.patterns) {
                c->accept(*this);                    // Gets the pattern, and puts it in "helper"

                cout << "Working on pattern" << endl;

                if (Helper.TypeName.compare("Int") == 0) {     // Case where pattern is an Int
                    // Compare input argument with pattern
                    Func += "cmpl $";
                    Func += Helper.TypeValue;
                    Func += ", ";
                    int mempos = argNum*4+8;            // Stack starts at 8, each arg with 4 space
                    Func += to_string(mempos);
                    Func += "(%ebp)\n";

                    argNum++;                           // Prepare for next argument

                    // If not different move on
                    Func += "jne .";
                    Func += FuncName;
                    Func += "case";
                    if (CaseCount + 1 == Cases) {       // Last case is called default
                        Func += "default";
                    } else {
                        Func += to_string(CaseCount + 1);
                    }
                    Func += "\n";
                    continue;
                }
                // repeat for all possebilities
            }
            node.expr->accept(*this);

            Helper = {};
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

        Func += "pushl %eax\n";
        node.right->accept(*this);

        Func += "popl %ebx\n";
        Func += "addl %ebx, %eax\n";
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
        Func += "movl $";
        Func += to_string(node.value);
        Func += ", %eax\n";

        Helper.TypeName = "Int";
        Helper.TypeValue = to_string(node.value);


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
        Helper.TypeName = "Id";
        Helper.TypeValue = FuncName + node.id;
        cout << "Got ID => " << node.id << endl;
    }

    void GasCodeGenerator::visit(Call &node) {

        // TODO: Find way pu push arguments to stack.
        vector<string> params;
        for (auto &arg : node.exprs) {
            arg->accept(*this);

            if(Helper.TypeName.compare("Int") == 0) {
                Func += "pushl %eax\n";
            } else if (Helper.TypeName.compare("Id") == 0) {

            }
        }

        node.callee->accept(*this); // function to call;
        Func += "call ";
        Func += Helper.TypeValue;
        Func += "\n";

        Helper = {};

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
        for (auto &f : FuncGlobl) {
            source += f;
            source += "\n";
        }

        for (auto &f : FuncVector) {
            source += f;
        }
        return source;
    }
}
