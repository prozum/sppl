#pragma once

#include "CodeGenerator.h"
#include "Node.h"
#include "Scope.h"

#include <string>
#include <iostream>
#include <map>
#include <bits/stl_map.h>

using namespace common;

namespace codegen {

    class GasCodeGenerator: public parser::CodeGenerator
    {
    public:
        GasCodeGenerator(parser::Driver &Drv);

        void visit(Program &Node);
        void visit(Function &Node);
        void visit(Case &Node);
        void visit(Or &Node);
        void visit(And &Node);
        void visit(Equal &Node);
        void visit(NotEqual &Node);
        void visit(Lesser &Node);
        void visit(Greater &Node);
        void visit(LesserEq &Node);
        void visit(GreaterEq &Node);
        void visit(Add &Node);
        void visit(Sub &Node);
        void visit(Mul &Node);
        void visit(Div &Node);
        void visit(Mod &Node);
        void visit(ListAdd &Node);
        void visit(ParExpr &Node);
        void visit(Not &Node);
        void visit(IntExpr &Node);
        void visit(FloatExpr &Node);
        void visit(BoolExpr &Node);
        void visit(CharExpr &Node);
        void visit(StringExpr &Node);
        void visit(ListExpr &Node);
        void visit(TupleExpr &Node);
        void visit(ListSplit &Node);
        void visit(IdExpr &Node);
        void visit(CallExpr &Node);
        void visit(Type &Node);

    private:
        string getType(Type * Ty);
        string buildSource();

        string Func;    // Contains the current function
        string FuncName;    // Contains the name of the current function

        vector<string> FuncVector;      // Contains all functions that have been read
        vector<string> FuncGlobl;       // Contains a globl for all functions

        size_t CaseCount = 0;           // Used to count the current case
        size_t Cases = 0;               // Number of cases
                                        // These may be moved into function later

        std::map<string,string> VarMap;

        typedef struct {
            string TypeName;
            string TypeValue;
        } Helper;

        Helper Hpr;

    };
}
