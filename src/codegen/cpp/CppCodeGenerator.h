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

namespace std {

    template <>
    struct hash<Type>
    {
        std::size_t operator()(const Type& k) const
        {
            size_t res = std::hash<int>()(static_cast<int>(k.type));

            for (auto type : k.types) {
                res ^= (std::hash<Type>()(*type) << 1);
            }

            return res;
        }
    };

}

namespace codegen {

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
            int output_tap_count = 0;
            int header_tap_count = 0;
            unordered_map<Type, string> tuples;
            unordered_map<Type, string> lists;
            unordered_map<Type, string> signatures;
            vector<string> arg_names;
            vector<string> arg_name_stack;
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

            string generate_list(Type &);
            string generate_tuple(Type &);
            string generate_signature(Type &);
            void generate_std();

            ostream &outputt();
            ostream &headert();
            ostream &t(ostream &stream, int i);
            string tap(int i);
    };
}
