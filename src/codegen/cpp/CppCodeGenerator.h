#pragma once
#include "CodeGenerator.h"
#include "Node.h"
#include <string>
#include <unordered_map>
#include <vector>
#include <sstream>

using namespace common;
using namespace std;

enum class IdContext {
    PATTERN,
    EXPR
};

class CCodeGenerator: public CodeGenerator
{
    public:
        CCodeGenerator(std::ostream &, std::ostream &);

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
        std::ostream &header;

        int tuple_count = 0;
        int list_count = 0;
        int sig_count = 0;
        int arg_count = 0;
        unordered_map<Type &, string> tuples;
        unordered_map<Type &, string> lists;
        unordered_map<Type &, string> signatures;
        unordered_map<string, string> real_ids;
        vector<string> arg_names;
        IdContext id_context;

        string generate_tuple(Type &);
};
