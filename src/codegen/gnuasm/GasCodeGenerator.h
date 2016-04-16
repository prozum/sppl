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
        GasCodeGenerator(parser::Driver &);

        void visit(Program &node);
        void visit(Function &node);
        void visit(Case &node);
        void visit(Or &node);
        void visit(And &node);
        void visit(Equal &node);
        void visit(NotEqual &node);
        void visit(Lesser &node);
        void visit(Greater &node);
        void visit(LesserEq &node);
        void visit(GreaterEq &node);
        void visit(Add &node);
        void visit(Sub &node);
        void visit(Mul &node);
        void visit(Div &node);
        void visit(Mod &node);
        void visit(ListAdd &node);
        void visit(Par &node);
        void visit(Not &node);
        void visit(Int &node);
        void visit(Float &node);
        void visit(Bool &node);
        void visit(Char &node);
        void visit(String &node);
        void visit(ListPattern &node);
        void visit(TuplePattern &node);
        void visit(ListSplit &node);
        void visit(List &node);
        void visit(Tuple &node);
        void visit(Id &node);
        void visit(Call &node);
        void visit(Type &node);

    private:
        string get_type(Type *);
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
        } helper_t;

        helper_t Helper;
    };
}
