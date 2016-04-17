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
            CCodeGenerator(Driver &driver);

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

        private:
            const string GGenerated = "generated_";
            const string GUser = "user_";
            const string GAdd = "add_";
            const string GCreate = "create_";
            const string GCompare = "compare_";
            const string GConcat = "concat_";
            const string GPrint = "print_";
            const string GAt = "at_";
            const string GValueAt = "valueat_";
            const string GToString = "tostring_";
            const string GFloat = "double";
            const string GInt = "int64_t";
            const string GChar = "int";
            const string GBool = "int";
            const string GString = "string";
            const string GList = "list";
            const string GSignature = "signature";
            const string GTuple = "tuple";
            const string GNext = "next";
            const string GValue = "value";
            const string GEmpty = "empty";
            const string GSize = "size";
            const string GItem = "item";
            const string GArg = "arg";
            const string GClosure = "closure";
            const string GGlobal = "global";
            const string GMain = "main";

            stringstream Buffer;
            ostream *Output;
            ostream *Header;

            int TupleCount = 0;
            int ListCount = 0;
            int SigCount = 0;
            int OobCount = 0;
            int EnvCount = 0;
            unordered_map<Type, string> Tuples;
            unordered_map<Type, string> Lists;
            unordered_map<Type, string> Closures;
            unordered_map<Type, string> ToStrings;
            vector<string> ArgNames;
            vector<string> GetValueBuilder;
            vector<string> Assignments;
            vector<int> ListOffsets;
            IdContext IdCtx;
            string LastPattern;
            string StringTypeName;

            std::stack<stringstream> ExprStack;

            Type StringList;
            Type RealString;

            Program* Prog;
            Function* CurFunc;

            string getType(Type &);
            string generateList(Type &);
            string generateTuple(Type &);
            string generateEnvironment(Type &);
            string getList(Type &);
            string getTuple(Type &);
            string getEnvironment(Type &);
            void generateStd();
            void outputBuffer();
            void outputEqual(Type &, Expression &, Expression &);

    };
}
