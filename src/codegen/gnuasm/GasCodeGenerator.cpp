#include "GasCodeGenerator.h"

// TODO: Add compile messages and final error messages.
// TODO: move all debug output lines to debug only.

using namespace common;
using namespace std;
using namespace codegen;

GasCodeGenerator::GasCodeGenerator(parser::Driver &Drv)
    : parser::CodeGenerator(Drv) {}

void GasCodeGenerator::visit(Program &Node) {
    // Visit all functions

    for (auto &Decl : Node.Decls) {
        Decl->accept(*this);
    }

    string Source = buildSource(); // Build source.S file
    *Drv.Out << Source << endl;
}

void GasCodeGenerator::visit(Function &Node) {
    FuncName =
        Node.Id; // Function name used for anything function related including
                 // names and labels.

    string Globl = ".globl "; // Build the globl
    Globl += FuncName;
    FuncGlobl.push_back(Globl);
    Func += FuncName; // Function entry
    Func += ":\n";
    Func += "pushl %ebp\n";      // Save base pointer
    Func += "movl %esp, %ebp\n"; // and move stack pointer
    Func += ".";
    Func += FuncName;
    Func += "funcstart:\n";

    CaseCount = 0;
    Cases = Node.Cases.size(); // Get number of cases

    for (auto &Case : Node.Cases) { // Build cases
        Case->accept(*this);
    }

    Func += ".";
    Func += FuncName;
    Func += "funcend:\n";

    if (FuncName.compare("main") == 0) { // If the current function is main, we
                                         // want to terminate the program when
                                         // done
        Func += "movl $0, %ebx\n";
        Func += "movl $1, %eax\n";
        Func += "int $0x80\n";
    } else { // Otherwise return to calling function
        Func += "movl %ebp, %esp\n";
        Func += "popl %ebp\n";
        Func += "leave\n";
        Func += "ret\n";
    }

    FuncVector.push_back(
        Func); // Adds function to vector with completed functions

    Func.clear(); // Prepare string variable for next function
}

void GasCodeGenerator::visit(Case &Node) {
    CaseCount++;

    int ArgCount = 0;
    for (auto &Pattern : Node.Patterns) {
        Pattern->accept(*this);
        cout << "PATTERN IN THIS SCOPE => " << Hpr.TypeName << "    "
             << Hpr.TypeValue << endl;

        if (Hpr.TypeName.compare("Id") == 0) {
            int MemPos = ArgCount * 4 + 8;
            string Var = "";
            Var += "movl ";
            Var += to_string(MemPos);
            Var += ", %eax\n";
            VarMap[Hpr.TypeValue] = Var;
        }
        Hpr = {};
        ArgCount++;
    }

    if (Cases == CaseCount) { // Default case
        Func += ".";
        Func += FuncName;
        Func += "casedefault:\n";
        Node.Expr->accept(*this);
    } else { // Other cases
        Func += ".";
        Func += FuncName;
        Func += "case";
        Func += to_string(CaseCount);
        Func += ":\n";

        int ArgNum = 0; // First argument have index 0
        for (auto &Pattern : Node.Patterns) {
            Pattern->accept(*this); // Gets the pattern, and puts it in "helper"

            cout << "Working on pattern" << endl;

            if (Hpr.TypeName.compare("Int") ==
                0) { // Case where pattern is an Int
                // Compare input argument with pattern
                Func += "cmpl $";
                Func += Hpr.TypeValue;
                Func += ", ";
                int MemPos =
                    ArgNum * 4 + 8; // Stack starts at 8, each arg with 4 space
                Func += to_string(MemPos);
                Func += "(%ebp)\n";

                ArgNum++; // Prepare for next argument

                // If not different move on
                Func += "jne .";
                Func += FuncName;
                Func += "case";
                if (CaseCount + 1 == Cases) { // Last case is called default
                    Func += "default";
                } else {
                    Func += to_string(CaseCount + 1);
                }
                Func += "\n";
                continue;
            }
            // Repeat for all possibilities
        }
        Node.Expr->accept(*this);

        Hpr = {};
    }

    cout << "CaseNotImplemented" << endl;
}

void GasCodeGenerator::visit(Or &Node) { cout << "OrNotImplemented" << endl; }

void GasCodeGenerator::visit(And &Node) { cout << "AndNotImplemented" << endl; }

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

void GasCodeGenerator::visit(Sub &Node) { cout << "SubNotImplemented" << endl; }

void GasCodeGenerator::visit(Mul &Node) { cout << "MulNotImplemented" << endl; }

void GasCodeGenerator::visit(Div &Node) { cout << "DivNotImplemented" << endl; }

void GasCodeGenerator::visit(Mod &Node) { cout << "ModNotImplemented" << endl; }

void GasCodeGenerator::visit(ListAdd &Node) {
    cout << "ListAddNotImplemented" << endl;
}

void GasCodeGenerator::visit(ParExpr &Node) {
    cout << "ParNotImplemented" << endl;
}

void GasCodeGenerator::visit(Not &Node) { cout << "NotNotImplemented" << endl; }

void GasCodeGenerator::visit(IntExpr &Node) {
    Func += "movl $";
    Func += Node.str();
    Func += ", %eax\n";

    Hpr.TypeName = "Int";
    Hpr.TypeValue = Node.str();

    cout << "Got integer => " << Node.str() << endl;
}

void GasCodeGenerator::visit(FloatExpr &Node) {
    cout << "FloatNotImplemented" << endl;
}

void GasCodeGenerator::visit(BoolExpr &Node) {
    cout << "BoolNotImplemented" << endl;
}

void GasCodeGenerator::visit(CharExpr &Node) {
    cout << "CharNotImplemented" << endl;
}

void GasCodeGenerator::visit(StringExpr &Node) {
    cout << "CharNotImplemented" << endl;
}

void GasCodeGenerator::visit(ListExpr &Node) {
    cout << "ListPatternNotImplemented" << endl;
}

void GasCodeGenerator::visit(TupleExpr &Node) {
    cout << "TuplePatternNotImplemented" << endl;
}

void GasCodeGenerator::visit(ListSplit &Node) {
    cout << "ListSplitNotImplemented" << endl;
}

void GasCodeGenerator::visit(IdExpr &Node) {
    Hpr.TypeName = "Id";
    Hpr.TypeValue = FuncName + Node.Val;
    cout << "Got ID => " << Node.Val << endl;
}

void GasCodeGenerator::visit(CallExpr &Node) {
    // TODO: Find way pu push arguments to stack.
    vector<string> Params;
    for (auto &Arg : Node.Args) {
        Arg->accept(*this);

        if (Hpr.TypeName.compare("Int") == 0) {
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

string GasCodeGenerator::getType(Type *Ty) { return "GasCodeGenerator"; }

string GasCodeGenerator::buildSource() {
    string Source = "";

    Source += ".data\n";
    Source += "fmt:\n";
    Source += ".string \"%d\\n\"\n"; // Allow printing of numbers in printf

    Source += ".text\n";
    for (auto &Func : FuncGlobl) {
        Source += Func;
        Source += "\n";
    }

    for (auto &Func : FuncVector) {
        Source += Func;
    }
    return Source;
}
