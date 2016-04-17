#include "GasCodeGenerator.h"

// TODO: Add compile messages and final error messages.
// TODO: move all debug output lines to debug only.

using namespace common;
using namespace std;

namespace codegen {

    GasCodeGenerator::GasCodeGenerator(parser::Driver &Drv) : parser::CodeGenerator(Drv) {
    }

    void GasCodeGenerator::visit(Program &Node) {
        // Visit all functions

        for (auto &func : Node.Funcs) {
            func->accept(*this);
        }

        string source = buildSource();  // Build source.S file
        *Drv.Out << source << endl;
    }

    void GasCodeGenerator::visit(Function &Node) {
        FuncName = Node.Id;                     // Function name used for anything function related including
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
        Cases = Node.Cases.size();             // Get number of cases

        for (auto &funcCase : Node.Cases) {     // Build cases
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

    void GasCodeGenerator::visit(Case &Node) {
        CaseCount++;

        int argc = 0;
        for (auto &c : Node.Patterns) {
            c->accept(*this);
            cout << "PATTERN IN THIS SCOPE => " << Hpr.TypeName << "    " << Hpr.TypeValue << endl;

            if (Hpr.TypeName.compare("Id") == 0) {
                int mempos = argc*4+8;
                string var = "";
                var += "movl ";
                var += to_string(mempos);
                var += ", %eax\n";
                VarMap[Hpr.TypeValue] = var;
            }
            Hpr = {};
            argc++;
        }

        if (Cases == CaseCount) {   // Default case
            Func += ".";
            Func += FuncName;
            Func += "casedefault:\n";
            Node.Expr->accept(*this);
        } else {                    // Other cases
            Func += ".";
            Func += FuncName;
            Func += "case";
            Func += to_string(CaseCount);
            Func += ":\n";

            int argNum = 0;                         // first argument have index 0
            for (auto &c : Node.Patterns) {
                c->accept(*this);                    // Gets the pattern, and puts it in "helper"

                cout << "Working on pattern" << endl;

                if (Hpr.TypeName.compare("Int") == 0) {     // Case where pattern is an Int
                    // Compare input argument with pattern
                    Func += "cmpl $";
                    Func += Hpr.TypeValue;
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
            Node.Expr->accept(*this);

            Hpr = {};
        }

        cout << "CaseNotImplemented" << endl;
    }

    void GasCodeGenerator::visit(Or &Node) {
        cout << "OrNotImplemented" << endl;
    }

    void GasCodeGenerator::visit(And &Node) {
        cout << "AndNotImplemented" << endl;
    }

    void GasCodeGenerator::visit(Equal &Node) {
        cout << "EqualNotImplemented" << endl;
    }

    void GasCodeGenerator::visit(NotEqual &Node) {
        cout << "NotEqualNotImplemented" << endl;
    }

    void GasCodeGenerator::visit(Lesser &Node) {
        cout << "LesserNotImplemented" << endl;
    }

    void GasCodeGenerator::visit(Greater &Node) {
        cout << "GreaterNotImplemented" << endl;
    }

    void GasCodeGenerator::visit(LesserEq &Node) {
        cout << "LesserEqNotImplemented" << endl;
    }

    void GasCodeGenerator::visit(GreaterEq &Node) {
        cout << "GreaterEqNotImplemented" << endl;
    }

    void GasCodeGenerator::visit(Add &Node) {
        cout << "ADD" << endl;

        Node.Left->accept(*this);

        Func += "pushl %eax\n";
        Node.Right->accept(*this);

        Func += "popl %ebx\n";
        Func += "addl %ebx, %eax\n";
        cout << "AddNotImplemented" << endl;
    }

    void GasCodeGenerator::visit(Sub &Node) {
        cout << "SubNotImplemented" << endl;
    }

    void GasCodeGenerator::visit(Mul &Node) {
        cout << "MulNotImplemented" << endl;
    }

    void GasCodeGenerator::visit(Div &Node) {
        cout << "DivNotImplemented" << endl;
    }

    void GasCodeGenerator::visit(Mod &Node) {
        cout << "ModNotImplemented" << endl;
    }

    void GasCodeGenerator::visit(ListAdd &Node) {
        cout << "ListAddNotImplemented" << endl;
    }

    void GasCodeGenerator::visit(Par &Node) {
        cout << "ParNotImplemented" << endl;
    }

    void GasCodeGenerator::visit(Not &Node) {
        cout << "NotNotImplemented" << endl;
    }

    void GasCodeGenerator::visit(Int &Node) {
        Func += "movl $";
        Func += to_string(Node.Val);
        Func += ", %eax\n";

        Hpr.TypeName = "Int";
        Hpr.TypeValue = to_string(Node.Val);


        cout << "Got integer => " << to_string(Node.Val) << endl;
    }

    void GasCodeGenerator::visit(Float &Node) {
        cout << "FloatNotImplemented" << endl;
    }

    void GasCodeGenerator::visit(Bool &Node) {
        cout << "BoolNotImplemented" << endl;
    }

    void GasCodeGenerator::visit(Char &Node) {
        cout << "CharNotImplemented" << endl;
    }

    void GasCodeGenerator::visit(String &Node) {
        cout << "StringNotImplemented" << endl;
    }

    void GasCodeGenerator::visit(ListPattern &Node) {
        cout << "ListPatternNotImplemented" << endl;
    }

    void GasCodeGenerator::visit(TuplePattern &Node) {
        cout << "TuplePatternNotImplemented" << endl;
    }

    void GasCodeGenerator::visit(ListSplit &Node) {
        cout << "ListSplitNotImplemented" << endl;
    }

    void GasCodeGenerator::visit(List &Node) {
        cout << "ListNotImplemented" << endl;
    }

    void GasCodeGenerator::visit(Tuple &Node) {
        cout << "TupleNotImplemented" << endl;
    }

    void GasCodeGenerator::visit(Id &Node) {
        Hpr.TypeName = "Id";
        Hpr.TypeValue = FuncName + Node.Val;
        cout << "Got ID => " << Node.Val << endl;
    }

    void GasCodeGenerator::visit(Call &Node) {

        // TODO: Find way pu push arguments to stack.
        vector<string> params;
        for (auto &arg : Node.Args) {
            arg->accept(*this);

            if(Hpr.TypeName.compare("Int") == 0) {
                Func += "pushl %eax\n";
            } else if (Hpr.TypeName.compare("Id") == 0) {

            }
        }

        Node.Callee->accept(*this); // function to call;
        Func += "call ";
        Func += Hpr.TypeValue;
        Func += "\n";

        Hpr = {};

        cout << "CallNotImplemented" << endl;
    }

    void GasCodeGenerator::visit(Type &Node) {
        cout << "TypeNotImplemented" << endl;
    }

    string GasCodeGenerator::getType(Type * Ty) {
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
