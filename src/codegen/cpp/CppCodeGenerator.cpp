#include "CppCodeGenerator.h"
#include <string>
#include <iostream>
#include <sstream>
#include <Scope.h>

using namespace common;
using namespace std;

namespace codegen
{
    CCodeGenerator::CCodeGenerator(Driver &driver) :
            CodeGenerator(driver),
            Output(driver.Out),
            Header(driver.HOut),
            ListOffsets(vector<int>({ 0 })),
            StringList(Type(TypeId::LIST, vector<Type>({ Type(TypeId::STRING) }))),
            RealString(Type(TypeId::LIST, vector<Type>({ Type(TypeId::CHAR) })))
    { }

    void CCodeGenerator::visit(Program &node)
    {
        Prog = &node;
        Function* main = nullptr;

        // generate the standard functionality for every program
        generateStd();

        // find the main function
        for (auto &i: node.Funcs)
            if (i->Id == "main")
                main = i.get();

        if (!main)
            throw "No main, help!";

        // get the type of main, so that return type of main is generated.
        getType(main->Ty);

        // generate real main function, that calls the users main
        string strlistname = getList(StringList);

        *Output << "int main(int argc, char** argv) { \n"
                   "    " << strlistname << " *args = " << GGenerated << GCreate << strlistname << "(0); \n"
                   "    int i; \n"
                   " \n"
                   "    for(i = argc - 1; i >= 0; i--) { \n"
                   "        args = " << GGenerated << GAdd << strlistname << "(args, " << GGenerated << GCreate << GString << "(argv[i])); \n"
                   "    } \n"
                   " \n"
                   "    " << GGenerated << GPrint << GString << "("  << ToStrings[main->Ty] << "(" << GGlobal << GUser << GMain << ".call(&" << GGlobal << GUser << GMain << ", args))); \n"
                   "    return 0; \n"
                   "} \n"
                   " \n";

        for (auto &f: node.Funcs) {
            f->accept(*this);
            *Output << endl;
            outputBuffer();
        }
    }

    void CCodeGenerator::visit(Function &node)
    {
        stringstream function;
        stringstream arg_name;
        string return_type = getType(node.Ty);
        string argument_type;
        string signature = getEnvironment(node.Signature);

        CurFunc = &node;

        // generate function name and return type
        function << return_type << " " << GUser << node.Id << "(" << signature << " *" << GGenerated << GSignature;

        // generate function arguments
        for (size_t i = 0; i < node.Signature.Subtypes.size() - 1; i++) {
            function << ", ";

            argument_type = getType(node.Signature.Subtypes[i]);
            arg_name << GGenerated << GArg << i;
            ArgNames.push_back(arg_name.str());
            function << argument_type << " " << arg_name.str();

            arg_name.str("");
        }

        function << ")";

        // generate function decleration in header
        *Header << function.str() << "; \n \n";

        *Header << signature << " " << GGlobal << GUser << node.Id << " = { " << GUser << node.Id << " }; \n\n";

        // generate function in *output
        *Output << function.str() << " { \n"
                   "Start: \n"
                   "\n";


        // generate cases
        for (auto &c: node.Cases) {
            c->accept(*this);
            *Output << endl;

            // clear assigments specific for current case
            Assignments.clear();
        }

        // generate error, for when program doesn't realize a case
        *Output << "    printf(\"No cases realized!\\n\"); \n"
                   "    exit(1); \n"
                   "} \n"
                   " \n";

        // clear arg_names for current function
        ArgNames.clear();
    }

    void CCodeGenerator::visit(Case &node)
    {
        stringstream pattern;
        bool empty = true;

        // generate if-statement for matching
        pattern << "    if (";

        IdCtx = IdContext::PATTERN;
        for (size_t i = 0; i < node.Patterns.size(); i++) {
            // push arg_name on get_value_builder. get_value_builder is used for generate
            // assignments in a case
            GetValueBuilder.push_back(ArgNames[i]);

            // generate pattern
            node.Patterns[i]->accept(*this);

            // cleanup
            GetValueBuilder.pop_back();

            // only add pattern, if pattern is not "1"
            if (LastPattern != "1") {

                if (!empty)
                    pattern << " && ";

                empty = false;
                pattern << LastPattern;
            }
        }

        pattern << ")";

        // only generate if statement, if the pattern wasn't empty
        if (!empty) {
            *Output << pattern.str() << endl;
        }

        *Output << "    { \n";

        // generate all nessesary assigments
        for (auto &assign: Assignments) {
            *Output << "        " << assign << endl;

            if (assign == Assignments.back())
                *Output << endl;
        }

        // generate return expression
        IdCtx = IdContext::EXPR;
        ExprStack.push(stringstream());

        if (node.TailRec) {
            auto call = (Call*)node.Expr.get();
            for (size_t i = 0; i < CurFunc->Signature.Subtypes.size() - 1; i++) {
                ExprStack.push(stringstream());
                call->Args[i]->accept(*this);
                *Output << "        " << GGenerated << GArg << i << " = " << ExprStack.top().str() << "; \n";
                ExprStack.pop();
            }

            *Output << "\n"
                       "        goto Start; \n";
        } else {
            string result_type = getType(CurFunc->Signature.Subtypes.back());
            ExprStack.push(stringstream());

            node.Expr->accept(*this);
            *Output << "        " << result_type << " res = " << ExprStack.top().str() << "; \n";
            *Output << "        return res; \n";
            ExprStack.pop();
        }

        *Output << "    } \n"
                   " \n";
    }

    void CCodeGenerator::visit(Or &node)
    {
        ExprStack.top() << "(";
        node.Left->accept(*this);
        ExprStack.top() << "||";
        node.Right->accept(*this);
        ExprStack.top() << ")";
    }

    void CCodeGenerator::visit(And &node)
    {
        ExprStack.top() << "(";
        node.Left->accept(*this);
        ExprStack.top() << "&&";
        node.Right->accept(*this);
        ExprStack.top() << ")";
    }

    void CCodeGenerator::visit(Equal &node)
    {
        outputEqual(node.Ty, *node.Left.get(), *node.Right.get());
    }

    void CCodeGenerator::visit(NotEqual &node)
    {
        ExprStack.top() << "(!";
        outputEqual(node.Ty, *node.Left.get(), *node.Right.get());
        ExprStack.top() << ")";
    }

    void CCodeGenerator::outputEqual(Type &type, Expression &left, Expression &right) {
        string name;

        switch (type.Id) {
            case TypeId::TUPLE:
                name = getTuple(left.Ty);

                ExprStack.top() << GGenerated << GCompare << name << "(";
                left.accept(*this);
                ExprStack.top() << ", ";
                right.accept(*this);
                ExprStack.top() << ")";
                break;
            case TypeId::LIST:
            case TypeId::STRING:
                name = getList(left.Ty);

                ExprStack.top() << GGenerated << GCompare << name << "(";
                left.accept(*this);
                ExprStack.top() << ", ";
                right.accept(*this);
                ExprStack.top() << ")";
                break;
            default:
                ExprStack.top() << "(";
                left.accept(*this);
                ExprStack.top() << "==";
                right.accept(*this);
                ExprStack.top() << ")";
                break;
        }
    }

    void CCodeGenerator::visit(Lesser &node)
    {
        ExprStack.top() << "(";
        node.Left->accept(*this);
        ExprStack.top() << "<";
        node.Right->accept(*this);
        ExprStack.top() << ")";
    }

    void CCodeGenerator::visit(LesserEq &node)
    {
        ExprStack.top() << "(";
        node.Left->accept(*this);
        ExprStack.top() << "<=";
        node.Right->accept(*this);
        ExprStack.top() << ")";
    }

    void CCodeGenerator::visit(Greater &node)
    {
        ExprStack.top() << "(";
        node.Left->accept(*this);
        ExprStack.top() << ">";
        node.Right->accept(*this);
        ExprStack.top() << ")";
    }

    void CCodeGenerator::visit(GreaterEq &node)
    {
        ExprStack.top() << "(";
        node.Left->accept(*this);
        ExprStack.top() << ">=";
        node.Right->accept(*this);
        ExprStack.top() << ")";
    }

    void CCodeGenerator::visit(Add &node)
    {
        ExprStack.top() << "(";
        node.Left->accept(*this);
        ExprStack.top() << "+";
        node.Right->accept(*this);
        ExprStack.top() << ")";
    }

    void CCodeGenerator::visit(Sub &node)
    {
        ExprStack.top() << "(";
        node.Left->accept(*this);
        ExprStack.top() << "-";
        node.Right->accept(*this);
        ExprStack.top() << ")";
    }

    void CCodeGenerator::visit(Mul &node)
    {
        ExprStack.top() << "(";
        node.Left->accept(*this);
        ExprStack.top() << "*";
        node.Right->accept(*this);
        ExprStack.top() << ")";
    }

    void CCodeGenerator::visit(Div &node)
    {
        ExprStack.top() << "(";
        node.Left->accept(*this);
        ExprStack.top() << "/";
        node.Right->accept(*this);
        ExprStack.top() << ")";
    }

    void CCodeGenerator::visit(Mod &node)
    {
        ExprStack.top() << "(";
        node.Left->accept(*this);
        ExprStack.top() << "%";
        node.Right->accept(*this);
        ExprStack.top() << ")";
    }

    void CCodeGenerator::visit(ListAdd &node)
    {
        string name = getList(node.Ty);

        // use pregenerated push function to push thing onto list
        // look at generate_list(Type &type) for the generation of this function
        ExprStack.top() << GGenerated << GAdd << name << "(";
        node.Right->accept(*this);
        ExprStack.top() << ", ";
        node.Left->accept(*this);
        ExprStack.top() << ")";
    }

    void CCodeGenerator::visit(Par &node)
    {
        ExprStack.top() << "(";
        node.Child->accept(*this);
        ExprStack.top() << ")";
    }

    void CCodeGenerator::visit(Not &node)
    {
        ExprStack.top() << "(";
        ExprStack.top() << "!";
        node.Child->accept(*this);
        ExprStack.top() << ")";
    }

    void CCodeGenerator::visit(ListPattern &node)
    {
        // result is needed, so we don't generate something else, while the ListPattern is being generated
        stringstream result;
        string value_gotten;
        string type_name = getList(node.Ty);

        // generate the gotten value from get_value_builder
        for (auto str : GetValueBuilder) {
            value_gotten += str;
        }

        result << "(" << value_gotten << "->" << GSize << " - " << ListOffsets.back() << " == " << node.Patterns.size() << ")";

        for (size_t i = 0; i < node.Patterns.size(); i++) {
            // add "gat_"name"(" and ", i + offset)" to get_value_builder
            GetValueBuilder.insert(GetValueBuilder.begin(),  GGenerated + GValueAt + type_name + "(");
            GetValueBuilder.push_back(", " + to_string(i + ListOffsets.back()) + ")");

            // push new offset.
            // lists accesed later down in pattern should not be offseted by the same as current list.
            ListOffsets.push_back(0);

            // generate pattern
            node.Patterns[i]->accept(*this);

            // cleanup
            ListOffsets.pop_back();
            GetValueBuilder.pop_back();
            GetValueBuilder.erase(GetValueBuilder.begin());

            // don't add pattern, if pattern is "1"
            if (LastPattern != "1") {
                result << " && " << LastPattern;
            }
        }

        LastPattern = "(" + result.str() + ")";
    }

    void CCodeGenerator::visit(TuplePattern &node)
    {
        // result is needed, so we don't start generating something in a signature in the header file
        stringstream result;
        string type_name = getTuple(node.Ty);
        bool empty = true;

        // iterate through all items in tuple
        for (size_t i = 0; i < node.Patterns.size(); i++) {
            // add ".gi"i"" to get_value_builder
            GetValueBuilder.push_back("." + GGenerated + GItem + to_string(i));

            // generate pattern
            node.Patterns[i]->accept(*this);

            // cleanup
            GetValueBuilder.pop_back();

            // don't add pattern, if pattern is "1"
            if (LastPattern != "1") {
                if (!empty)
                    result << " && ";

                empty = false;
                result << LastPattern;
            }

        }

        // if empty, then let last_pattern be "1"
        if (empty) {
            LastPattern = "1";
        } else {
            LastPattern = "(" + result.str() + ")";
        }
    }

    void CCodeGenerator::visit(ListSplit &node)
    {
        stringstream result;
        string type_name = getList(node.Ty);
        bool empty = true;

        // add "gat_"name"(" and ", offset)" to get_value_builder
        // this is done, so that patterns on the left of node, will use
        // the first + offset item in the list
        GetValueBuilder.insert(GetValueBuilder.begin(), GGenerated + GValueAt + type_name + "(");
        GetValueBuilder.push_back(", " + to_string(ListOffsets.back()) + ")");

        // push new offset.
        // lists accesed later down in pattern should not be offseted by the same as current list.
        ListOffsets.push_back(0);

        // generate pattern
        node.Left->accept(*this);

        // cleanup
        ListOffsets.pop_back();
        GetValueBuilder.pop_back();
        GetValueBuilder.erase(GetValueBuilder.begin());

        // don't add pattern, if pattern is "1"
        if (LastPattern != "1") {
            empty = false;
            result << LastPattern;
        }

        // right side of a list split, will be the list, but with the first + offset elements missing.
        // this is why we track an offset, so that we don't clone a list, unless we have to.
        ListOffsets[ListOffsets.size() - 1]++;

        // generate pattern
        node.Right->accept(*this);

        // reverse offset back to what it was
        ListOffsets[ListOffsets.size() - 1]--;

        // don't add pattern, if pattern is "1"
        if (LastPattern != "1") {
            if (!empty)
                result << " && ";

            empty = false;
            result << LastPattern;
        }

        // if empty, then let last_pattern be "1"
        if (empty) {
            LastPattern = "1";
        } else {
            LastPattern = "(" + result.str() + ")";
        }
    }

    void CCodeGenerator::visit(Int &node)
    {
        // if pattern, then generate an expression for matching on this pattern
        if (IdCtx == IdContext::PATTERN) {
            string value = "";

            for (auto str : GetValueBuilder) {
                value += str;
            }

            LastPattern = value + " == " + node.str();

        // else, just output value
        } else {
            ExprStack.top() << "(" << node.Val << ")";
        }
    }

    void CCodeGenerator::visit(Float &node)
    {
        // if pattern, then generate an expression for matching on this pattern
        if (IdCtx == IdContext::PATTERN) {
            string value = "";

            for (auto str : GetValueBuilder) {
                value += str;
            }

            LastPattern = value + " == " + node.str();

        // else, just output value
        } else {
            ExprStack.top() << "(" << node.str() << ")";
        }

    }

    void CCodeGenerator::visit(Bool &node)
    {
        // if pattern, then generate an expression for matching on this pattern
        if (IdCtx == IdContext::PATTERN) {
            string value = "";

            for (auto str : GetValueBuilder) {
                value += str;
            }

            LastPattern = value + " == " + to_string(node.Val);

        // else, just output value
        } else {
            ExprStack.top() << node.Val;
        }
    }

    void CCodeGenerator::visit(Char &node)
    {
        // if pattern, then generate an expression for matching on this pattern
        if (IdCtx == IdContext::PATTERN) {
            string value = "";

            for (auto str : GetValueBuilder) {
                value += str;
            }

            LastPattern = value + " == " + node.str();

        // else, just output value
        } else {
            ExprStack.top() << node.str();
        }
    }

    void CCodeGenerator::visit(String &node)
    {
        // if pattern, then generate an expression for matching on this pattern
        if (IdCtx == IdContext::PATTERN) {
            string value = "";

            for (auto str : GetValueBuilder) {
                value += str;
            }

            // gcompare_string is generated by generate_std. it compares the custome string type, to a char*
            // it also takes an offset, for when list splits occur on strings
            LastPattern = GGenerated + GCompare + GString + "(" + value + ", " + node.str() + ", " + to_string(ListOffsets.back()) + ")";

        // else, just output value
        } else {
            // gcreate_string is generate by generate_std. It creates string base on a char*
            ExprStack.top() << GGenerated << GCreate << GString << "(" << node.str() << ")";
        }
    }

    void CCodeGenerator::visit(List &node)
    {
        string name = getList(node.Ty);

        // create list
        ExprStack.top() << GGenerated << GCreate << name << "(" << node.Elements.size();
        for (int i = node.Elements.size() - 1; i >= 0; i--) {
            ExprStack.top() << ", ";

            node.Elements[i]->accept(*this);
        }

        ExprStack.top() << ")";
    }

    void CCodeGenerator::visit(Tuple &node)
    {
        string name = getTuple(node.Ty);

        // create tuple
        ExprStack.top() << GGenerated << GCreate << name << "(";
        for (auto &expr: node.Elements){
            expr->accept(*this);

            if (expr != node.Elements.back())
                ExprStack.top() << ", ";
        }
        ExprStack.top() << ")";
    }

    void CCodeGenerator::visit(Id &node)
    {
        stringstream assign;
        string name = "";

        if (IdCtx == IdContext::PATTERN) {
            // created name from get_value_builder
            for (auto str : GetValueBuilder) {
                name += str;
            }

            // generate an assignment for the id, which should occur after the if-statement
            assign << getType(node.Ty) << " " << GUser << node.Val << " = ";

            if ((node.Ty.Id == TypeId::LIST || node.Ty.Id == TypeId::STRING) &&
                 ListOffsets.back() > 0) {
                assign << GGenerated << GAt << getList(node.Ty) << "(" << name << ", " << ListOffsets.back() << ");";
            } else {
                assign << name << ";";
            }

            // save the assigment untill after the pattern has been generated
            Assignments.push_back(assign.str());

            // since and id, in a pattern is allways true, then last_pattern is just set to "1"
            LastPattern = "1";
        } else {
            bool is_declared = false;

            for (auto &func: Prog->Funcs) {
                if (func->Id == node.Val) {
                    is_declared = true;
                    break;
                }
            }

            if (is_declared) {
                ExprStack.top() << "&" << GGlobal;
            }

            ExprStack.top() << GUser << node.Val;
        }
    }

    void CCodeGenerator::visit(Call &node)
    {
        string name = GGenerated + GClosure + to_string(EnvCount++);
        string assignment = getEnvironment(node.Callee->Ty) + "* " + name + " = ";

        ExprStack.push(stringstream());
        // generate the callee (aka, the function being called)
        node.Callee->accept(*this);
        *Output << "        " << assignment << ExprStack.top().str() << "; \n";
        ExprStack.pop();

        // generate the arguments the function is being called with
        ExprStack.top() << name << "->call(" << name;
        for (auto &expr: node.Args){
            ExprStack.top() << ", ";

            expr->accept(*this);
        }
        ExprStack.top() << ")";
    }

    string CCodeGenerator::getType(Type &type) {
        unordered_map<Type, string>::iterator got;

        // generate the currect c type, based on sppl's types
        switch (type.Id) {
            case TypeId::FLOAT:
                return GFloat;
            case TypeId::CHAR:
                return GChar;
            case TypeId::INT:
                return GInt;
            case TypeId::BOOL:
                return GBool;
                // for tuples, lists, closures and strings, custom types will be generated
            case TypeId::TUPLE:
                return getTuple(type);
            case TypeId::SIGNATURE:
                return getEnvironment(type) + "*";
            case TypeId::STRING:
                return StringTypeName + "*";
            case TypeId::LIST:
                return getList(type) + "*";
            default:
                // This should never happen. If it does, the type checker made a mistake.
                return "This really shouldn't happen.";
        }
    }

    string CCodeGenerator::generateList(Type &type) {
        // save the generated list in a result stream
        stringstream result;
        string name = GGenerated + GList + to_string(ListCount);
        string children_type = getType(type.Subtypes.front());

        // increase list count, so next list doesn't have the same name
        ListCount++;

        // generate the list struct
        *Header << " \n"
                   "typedef struct " << name << " { \n"
                   "    struct " << name << "* " << GNext << "; \n"
                   "    " << children_type << " " << GValue << "; \n"
                   "    int " << GEmpty << "; \n"
                   "    int " << GSize << "; \n"
                   "} " << name << "; \n"
                   " \n";


        // generate add function for list
        *Header << name << "* " << GGenerated << GAdd << name << "(" << name << "* current, " << children_type << " item); \n";
        Buffer << name << "* " << GGenerated << GAdd << name << "(" << name << "* current, " << children_type << " item) { \n"
                   "    " << name << "* res = malloc(sizeof(" << name << "));\n"
                   "    res->" << GValue << " = item; \n"
                   "    res->" << GNext << " = current; \n"
                   "    res->" << GEmpty << " = 0; \n"
                   "    res->" << GSize << " = current->" << GSize << " + 1; \n"
                   "    return res; \n"
                   "} \n"
                   " \n";


        // generate create function for list
        *Header << name << "* " << GGenerated << GCreate << name << "(int count, ...); \n";
        Buffer << name << "* " << GGenerated << GCreate << name << "(int count, ...) { \n"
                  "    int i; \n"
                  "    va_list args; \n"
                  "    " << name << "* res = malloc(sizeof(" << name << ")); \n"
                  "    res->" << GEmpty << " = 1; \n"
                  "    res->" << GSize << " = 0; \n"
                  "\n"
                  "    va_start(args, count); \n"
                  "\n"
                  "    for (i = 0; i < count; i++) { \n"
                  "        res = " << GGenerated << GAdd << name << "(res, va_arg(args, " << children_type << ")); \n"
                  "    } \n"
                  "\n"
                  "    va_end(args); \n"
                  "    return res; \n"
                  "} \n"
                  " \n";


        // generate at function for list
        *Header << name << "* " << GGenerated << GAt << name << "(" << name << "* current, int index); \n";
        Buffer << name << "* " << GGenerated << GAt << name << "(" << name << "* current, int index) { \n"
                   "    while (index-- > 0) { \n"
                   "        if (current->" << GEmpty << ") { \n"
                   "            return current; \n"
                   "        } \n"
                   "\n"
                   "        current = current->" << GNext << "; \n"
                   "    } \n"
                   "\n"
                   "    return current; \n"
                   "} \n"
                   " \n";

        // generate valueat function for list
        *Header << children_type << " " << GGenerated << GValueAt << name << "(" << name << "* current, int index); \n";
        Buffer << children_type << " " << GGenerated << GValueAt << name << "(" << name << "* current, int index) { \n"
                   "    " << name << "* res = " << GGenerated << GAt << name << "(current, index); \n"
                   "\n"
                   "    if (res->" << GEmpty << ") { \n"
                   "        printf(\"Out of bound! " << OobCount++ << "\\n\"); \n"
                   "        exit(1); \n"
                   "    }"
                   "\n"
                   "    return res->" << GValue << "; \n"
                   "} \n"
                   " \n";

        // generate compare function for list
        *Header << "int " << GGenerated << GCompare << name << "(" << name << "* list1, " << name << "* list2); \n";
        Buffer << "int " << GGenerated << GCompare << name << "(" << name << "* list1, " << name << "* list2) { \n"
                   "    int i; \n"
                   "    if (list1->" << GSize << " != list2->" << GSize << ") { \n"
                   "         return 0; \n"
                   "    } \n"
                   " \n"
                   "    for (i = 0; i < list1->" << GSize << "; i++) { \n"
                   "        if(";

        switch (type.Subtypes.front().Id) {
            case TypeId::LIST:
            case TypeId::TUPLE:
                Buffer << "!" << GGenerated << GCompare << children_type <<
                        "(" << GGenerated << GValueAt << name << "(list1, i), " << GGenerated << GValueAt << name << "(list2, i))";
                break;
            default:
                Buffer << GGenerated << GValueAt << name << "(list1, i) != " << GGenerated << GValueAt << name << "(list2, i)";
                break;
        }

        Buffer << ") \n"
                  "            return 0; \n"
                  "    } \n"
                  " \n"
                  "    return 1; \n"
                  "} \n"
                  " \n";

        // generation of concat
        *Header << name << "* " << GGenerated << GConcat << name << "(" << name << "* list1, " << name << "* list2); \n";
        Buffer << name << "* " << GGenerated << GConcat << name << "(" << name << "* list1, " << name << "* list2) { \n"
                   "    int i; \n"
                   "    " << name << "** elements = malloc(sizeof(" << name << "*) * list1->" << GSize <<"); \n"
                   " \n"
                   "    for (i = 0; !list1->" << GEmpty << "; ++i) { \n"
                   "        elements[i] = list1; \n"
                   "        list1 = list1->" << GNext << "; \n"
                   "    } \n"
                   " \n"
                   "    for (--i; i >= 0; --i) { \n"
                   "        list2 = " << GGenerated << GAdd << name << "(list2, elements[i]->" << GValue << "); \n"
                   "    } \n"
                   " \n"
                   "    free(elements); \n"
                   "    return list2; \n"
                   "} \n"
                   "\n";

        // generation of tostring
        ToStrings[type] = GGenerated + GToString + name;

        *Header << StringTypeName << "* " << ToStrings[type] << "(" << name << "* value); \n";
        Buffer << StringTypeName << "* " << ToStrings[type] << "(" << name << "* value) { \n"
                "    " << StringTypeName << "* comma = " << GGenerated << GCreate << GString << "(\", \"); \n"
                "    " << StringTypeName << "* res = " << GGenerated << GCreate << GString << "(\"]\"); \n"
                "    int i; \n"
                "\n"
                "    for (i = value->" << GSize << " - 1; i >= 0; i--) { \n"
                "        res = " << GGenerated << GConcat << StringTypeName << "(" << ToStrings[type.Subtypes.front()] << "(" << GGenerated << GValueAt << name << "(value, i)), res); \n"
                " \n"
                "        if (i != 0) \n"
                "            res = " << GGenerated << GConcat << StringTypeName << "(comma, res); \n"
                "    } \n"
                " \n"
                "    res = " << GGenerated << GAdd << StringTypeName << "(res, '['); \n"
                "    return res; \n"
                "} \n"
                " \n";


        Lists[type] = name;

        // return name of list generated
        return name;
    }

    string CCodeGenerator::generateEnvironment(Type &type) {
        // result is needed, so we don't generate something inside the signature, while generating other types
        stringstream result;
        string name = GGenerated + GSignature + to_string(SigCount);

        // increase signature count, so next signature doesn't have the same name
        SigCount++;

        result << "typedef struct " << name << " {\n"
               << "    " << getType(type.Subtypes.back()) << " (* call)(struct " << name << "*";

        for (size_t i = 0; i < type.Subtypes.size() - 1; ++i) {
            result << ", ";

            // get the actual type of the argument
            result << getType(type.Subtypes[i]);
        }

        result << "); \n"
                  "} " << name << ";\n\n";

        *Header << "\n" << result.str();

        // generation of signature to string
        ToStrings[type] = GGenerated + GToString + name;

        *Header << StringTypeName << "* " << ToStrings[type] << "(" << name << " value); \n";
        Buffer << StringTypeName << "* " << ToStrings[type] << "(" << name << " value) { \n"
                   "    return " << GGenerated << GCreate << GString << "(\"" << type.str() << "\"); \n"
                   "} \n"
                   " \n";

        // save signature in signature hash map
        Closures[type] = name;

        // return name of signature generated
        return name;
    }

    string CCodeGenerator::generateTuple(Type &type) {
        // result is needed, so we don't start generating something while generating the tuple
        stringstream result;
        string name = GGenerated + GTuple + to_string(TupleCount);

        // increase tuple count, so next tuple doesn't have the same name
        TupleCount++;

        // generate the tuple struct
        result << " \n"
                  "typedef struct " << name << " {" << endl;
        // generate an item for each type in the tuple
        for (size_t i = 0; i < type.Subtypes.size(); ++i) {
            // get the actual type of the item
            result << "    " << getType(type.Subtypes[i]) << " " << GGenerated << GItem << i << ";" << endl; // give this item a unique name

        }
        result << "} " << name << "; \n";

        *Header << result.str();
        result.str("");

        // generate a create function for the tuple
        // give contructor a unique name
        result << name << " " << GGenerated << GCreate << name << "(";

        // generate an argument for each item in the struct
        for (size_t i = 0; i < type.Subtypes.size(); ++i) {
            // get the actual type of the argument
            result << getType(type.Subtypes[i]) << " " << GGenerated << GItem << i;

            if (i < type.Subtypes.size() - 1) // don't print ", " after the last argument
                result << ", ";
        }

        result << ")";

        *Header << result.str() << "; \n";
        Buffer << result.str() << " { \n";

        // generate a result variable
        Buffer << "    " << name << " " << "res; \n";

        // for each item in res, assign values
        for (size_t i = 0; i < type.Subtypes.size(); ++i) {
            Buffer << "    res." << GGenerated << GItem << i << " = " << GGenerated << GItem << i << "; \n";
        }
        Buffer << " \n"
                   "    return res; \n"
                   "} \n"
                   " \n";

        // generate a compare function for this tuple
        *Header << "int " << GGenerated << GCompare << name << "(" << name << " tuple1, " << name << " tuple2); \n";
        Buffer << "int " << GGenerated << GCompare << name << "(" << name << " tuple1, " << name << " tuple2) { \n";

        for (size_t i = 0; i < type.Subtypes.size(); ++i) {
            Buffer << "    if (";

            switch (type.Subtypes[i].Id) {
                case TypeId::STRING:
                case TypeId::LIST:
                    Buffer << "!" << GGenerated << GCompare << Lists[type.Subtypes[i]] <<
                               "(tuple1." << GGenerated << GItem << i << ", tuple2." << GGenerated << GItem << i << ")";
                    break;
                case TypeId::TUPLE:
                    Buffer << "!" << GGenerated << GCompare << Tuples[type.Subtypes[i]] <<
                               "(tuple1." << GGenerated << GItem << i << ", tuple2." << GGenerated << GItem << i << ")";
                    break;
                default:
                    Buffer << "(tuple1." << GGenerated << GItem << i << " != tuple2." << GGenerated << GItem << i << ")";
                    break;
            }

            Buffer << ") \n"
                      "        return 0; \n";
        }

        Buffer << " \n"
                  "    return 1; \n"
                  "} \n"
                  "\n";


        // generate a to_string for the tuple
        ToStrings[type] = GGenerated + GToString + name;

        *Header << StringTypeName << "* " << ToStrings[type] << "(" << name << " value); \n";
        Buffer << StringTypeName << "* " << ToStrings[type] << "(" << name << " value) { \n"
                   "    " << StringTypeName << "* comma = " << GGenerated << GCreate << GString << "(\", \"); \n"
                   "    " << StringTypeName << "* res = " << GGenerated << GCreate << GString << "(\")\"); \n"
                   "\n";

        for (size_t i = type.Subtypes.size() - 1; i != 0; --i) {
            Buffer << "    res = " << GGenerated << GConcat << StringTypeName << "(" << ToStrings[type.Subtypes[i]] << "(value." << GGenerated << GItem << i << ")" << ", res); \n";
            Buffer << "    res = " << GGenerated << GConcat << StringTypeName << "(comma, res); \n";
        }


        Buffer << "    res = " << GGenerated << GConcat << StringTypeName << "(" << ToStrings[type.Subtypes[0]] << "(value." << GGenerated << GItem << "0)" << ", res); \n"
                   "    res = " << GGenerated << GAdd << StringTypeName << "(res, '('); \n"
                   " \n"
                   "    return res; \n"
                   "} \n"
                   "\n";

        // save tuple in tuple hash map
        Tuples[type] = name;

        // return name of tuple generated
        return name;
    }

    void CCodeGenerator::generateStd() {
        *Output << "#include \"test.h\" \n \n";

        *Header << "#include <stdarg.h> \n"
                   "#include <stdio.h> \n"
                   "#include <stdlib.h> \n"
                   "#include <string.h> \n"
                   "#include <stdint.h> \n"
                   "#include <inttypes.h> \n";

        ToStrings[Type(TypeId::INT)] = GGenerated + GToString + "int";
        ToStrings[Type(TypeId::BOOL)] = GGenerated + GToString + "bool";
        ToStrings[Type(TypeId::FLOAT)] = GGenerated + GToString + "float";
        ToStrings[Type(TypeId::CHAR)] = GGenerated + GToString + "char";
        ToStrings[Type(TypeId::STRING)] = GGenerated + GToString + "string";

        StringTypeName = GGenerated + GList + to_string(ListCount);
        Lists[Type(TypeId::STRING)] = StringTypeName;

        generateList(RealString);
        generateList(StringList);

        outputBuffer();

        // generation of string constructer starts here
        *Header << StringTypeName << "* " << GGenerated << GCreate << GString << "(char* values);\n";
        *Output << StringTypeName << "* " << GGenerated << GCreate << GString << "(char* values) { \n"
                  "    int i, str_length = strlen(values); \n"
                  "    " << StringTypeName << "* res = " << GGenerated << GCreate << StringTypeName << "(0); \n"
                  " \n"
                  "    for (i = str_length - 1; i >= 0; i--) { \n"
                  "        res = " << GGenerated << GAdd << StringTypeName << "(res, values[i]); \n"
                  "    } \n"
                  " \n"
                  "    return res; \n"
                  "} \n"
                  " \n";

        // generation of string compare
        *Header << "int " << GGenerated << GCompare << GString << "(" << StringTypeName << "* string, char* values, int offset); \n";
        *Output << "int " << GGenerated << GCompare << GString << "(" << StringTypeName << "* string, char* values, int offset) { \n"
                   "    int i, size = strlen(values); \n"
                   " \n"
                   "    if (size == string->" << GSize << ") { \n"
                   "        for (i = 0; i < size; i++) { \n"
                   "            if (" << GGenerated << GValueAt << StringTypeName << "(string, i) != values[i]) \n"
                   "                return 0; \n"
                   "        } \n"
                   "    } else { \n"
                   "        return 0; \n"
                   "    } \n"
                   " \n"
                   "    return 1; \n"
                   "} \n"
                   " \n";



        // generation of print string
        *Header << StringTypeName << "* " << GGenerated << GPrint << GString << "(" << StringTypeName << "* string); \n";
        *Output << StringTypeName << "* " << GGenerated << GPrint << GString << "(" << StringTypeName << "* string) { \n"
                   "    char* buffer = malloc(sizeof(char) * (string->" << GSize << " + 1)); \n"
                   "    int i; \n"
                   " \n"
                   "    for (i = 0; i < string->" << GSize << "; i++) { \n"
                   "        buffer[i] = (char)" << GGenerated << GValueAt << StringTypeName << "(string, i); \n"
                   "    } \n"
                   " \n"
                   "    buffer[i] = '\\0'; \n"
                   "    printf(\"%s\\n\", buffer); \n"
                   "    free(buffer); \n"
                   "    return string; \n"
                   "} \n"
                   " \n";


        // generation of default to_string methods
        *Header << StringTypeName << "* " << ToStrings[Type(TypeId::INT)] << "(" << GInt << " value); \n";
        *Output << StringTypeName << "* " << ToStrings[Type(TypeId::INT)] << "(" << GInt << " value) { \n"
                   "    char buffer[100]; \n"
                   "    sprintf(buffer, \"%\" PRId64 \"\", value); \n"
                   "    return " << GGenerated << GCreate << GString << "(buffer); \n"
                   "} \n"
                   " \n";

        *Header << StringTypeName << "* " << ToStrings[Type(TypeId::BOOL)] << "(" << GBool << " value); \n";
        *Output << StringTypeName << "* " << ToStrings[Type(TypeId::BOOL)] << "(" << GBool << " value) { \n"
                   "    if (value) \n"
                   "        return " << GGenerated << GCreate << GString << "(\"True\"); \n"
                   "    else \n"
                   "        return " << GGenerated << GCreate << GString << "(\"False\"); \n"
                   "} \n"
                   " \n";

        *Header << StringTypeName << "* " << ToStrings[Type(TypeId::FLOAT)] << "(" << GFloat << " value); \n";
        *Output << StringTypeName << "* " << ToStrings[Type(TypeId::FLOAT)] << "(" << GFloat << " value) { \n"
                   "    char buffer[100]; \n"
                   "    sprintf(buffer, \"%lf\", value); \n"
                   "    return " << GGenerated << GCreate << GString << "(buffer); \n"
                   "} \n"
                   " \n";

        *Header << StringTypeName << "* " << ToStrings[Type(TypeId::CHAR)] << "(" << GChar << " value); \n";
        *Output << StringTypeName << "* " << ToStrings[Type(TypeId::CHAR)] << "(" << GChar << " value) { \n"
                   "    " << StringTypeName << "* res = " << GGenerated << GCreate << GString << "(\"\\'\"); \n"
                   "    res = " << GGenerated << GAdd << StringTypeName << "(res, value); \n"
                   "    res = " << GGenerated << GAdd << StringTypeName << "(res, '\\''); \n"
                   "    return res; \n"
                   "} \n"
                   " \n";

        *Header << StringTypeName << "* " << ToStrings[Type(TypeId::STRING)] << "(" << StringTypeName << "* value); \n";
        *Output << StringTypeName << "* " << ToStrings[Type(TypeId::STRING)] << "(" << StringTypeName << "* value) { \n"
                   "    " << StringTypeName << "* res = " << GGenerated << GCreate << GString << "(\"\\\"\"); \n"
                   "    res = " << GGenerated << GConcat << StringTypeName << "(value, res); \n"
                   "    res = " << GGenerated << GAdd << StringTypeName << "(res, '\"'); \n"
                   "} \n"
                   " \n";
    }

    string CCodeGenerator::getList(Type &type) {
        auto got = Lists.find(type);

        if (got == Lists.end()) {
            return generateList(type);
        } else {
            return got->second;
        }
    }

    string CCodeGenerator::getTuple(Type &type) {
        auto got = Tuples.find(type);

        if (got == Tuples.end()) {
            return generateTuple(type);
        } else {
            return got->second;
        }
    }

    string CCodeGenerator::getEnvironment(Type &type) {
        auto got = Closures.find(type);

        if (got == Closures.end()) {
            return generateEnvironment(type);
        } else {
            return got->second;
        }
    }

    void CCodeGenerator::outputBuffer() {
        *Output << Buffer.str(),
        Buffer.str(string());
    }
}
