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
            CCodeGenerator(Driver &Drv);

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
            void visit(Par &Node);
            void visit(Not &Node);
            void visit(IntPattern &Node);
            void visit(FloatPattern &Node);
            void visit(Bool &Node);
            void visit(CharPattern &Node);
            void visit(String &Node);
            void visit(ListPattern &Node);
            void visit(TuplePattern &Node);
            void visit(ListSplit &Node);
            void visit(ListExpression &Node);
            void visit(TupleExpression &Node);
            void visit(IdPattern &Node);
            void visit(Call &Node);

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

            string getType(Type &Ty);
            string generateList(Type &Ty);
            string generateTuple(Type &Ty);
            string generateEnvironment(Type &Ty);
            string getList(Type &Ty);
            string getTuple(Type &Ty);
            string getEnvironment(Type &Ty);
            void generateStd();
            void outputBuffer();
            void outputEqual(Type &Ty, Expression &Left, Expression &Right);
    };
}
