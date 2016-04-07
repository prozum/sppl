#pragma once
#include "CodeGenerator.h"
#include "Node.h"
#include "../../semantics/TypeChecker.h"
#include "../../semantics/ScopeGenerator.h"
#include <string>
#include <unordered_map>
#include <vector>
#include <sstream>

using namespace common;
using namespace std;

namespace codegen {

    enum class IdContext {
        PATTERN,
        EXPR
    };

    class CCodeGenerator: public CodeGenerator
    {
        public:
            CCodeGenerator(std::ostream&, std::ostream&);

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
            const string g_generated = "generated_";
            const string g_user = "user_";
            const string g_push = "push_";
            const string g_create = "create_";
            const string g_compare = "compare_";
            const string g_at = "at_";
            const string g_clone = "clone_";
            const string g_float = "double";
            const string g_int = "int";
            const string g_char = "int";
            const string g_bool = "int";
            const string g_string = "string";
            const string g_list = "list";
            const string g_signature = "signature";
            const string g_tuple = "tuple";
            const string g_head = "head";
            const string g_size = "size";
            const string g_items = "items";
            const string g_item = "item";
            const string g_arg = "arg";
            const string g_main = "main";
            const string g_nearpow2 = "nearest_power_of_two";

            std::ostream &header;

            int tuple_count = 0;
            int list_count = 0;
            int sig_count = 0;
            unordered_map<Type, string> tuples;
            unordered_map<Type, string> lists;
            unordered_map<Type, string> signatures;
            unordered_map<string, int> listnames_offset;
            vector<string> arg_names;
            vector<string> get_value_builder;
            vector<string> assignments;
            vector<int> list_offsets;
            IdContext id_context;
            string last_pattern;
            string last_type;
            string string_type_name;

            Type _char;
            Type fake_string;
            Type real_string;
            Type string_list;

            Function* current_func;

            string generate_list(Type &);
            string generate_tuple(Type &);
            string generate_signature(Type &);
            void generate_std();
    };
}
