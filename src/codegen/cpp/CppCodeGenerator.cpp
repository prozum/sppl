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
            output(driver.Out),
            header(driver.HOut),
            list_offsets(vector<int>({ 0 })),
            string_list(Type(TypeId::LIST, vector<Type>({ Type(TypeId::STRING) }))),
            real_string(Type(TypeId::LIST, vector<Type>({ Type(TypeId::CHAR) })))
    { }

    void CCodeGenerator::visit(Program &node)
    {
        program = &node;
        Function* main = nullptr;

        // generate the standard functionality for every program
        generate_std();

        // find the main function
        for (auto &i: node.Funcs)
            if (i->Id == "main")
                main = i.get();

        if (!main)
            throw "No main, help!";

        // get the type of main, so that return type of main is generated.
        get_type(main->Ty);

        // generate real main function, that calls the users main
        string strlistname = get_list(string_list);

        *output << "int main(int argc, char** argv) { \n"
                   "    " << strlistname << " *args = " << g_generated << g_create << strlistname << "(0); \n"
                   "    int i; \n"
                   " \n"
                   "    for(i = argc - 1; i >= 0; i--) { \n"
                   "        args = " << g_generated << g_add << strlistname << "(args, " << g_generated << g_create << g_string << "(argv[i])); \n"
                   "    } \n"
                   " \n"
                   "    " << g_generated << g_print << g_string << "("  << to_strings[main->Ty] << "(" << g_global << g_user << g_main << ".call(&" << g_global << g_user << g_main << ", args))); \n"
                   "    return 0; \n"
                   "} \n"
                   " \n";

        for (auto &f: node.Funcs) {
            f->accept(*this);
            *output << endl;
            output_buffer();
        }
    }

    void CCodeGenerator::visit(Function &node)
    {
        stringstream function;
        stringstream arg_name;
        string return_type = get_type(node.Ty);
        string argument_type;
        string signature = get_environment(node.Signature);

        current_func = &node;

        // generate function name and return type
        function << return_type << " " << g_user << node.Id << "(" << signature << " *" << g_generated << g_signature;

        // generate function arguments
        for (size_t i = 0; i < node.Signature.Subtypes.size() - 1; i++) {
            function << ", ";

            argument_type = get_type(node.Signature.Subtypes[i]);
            arg_name << g_generated << g_arg << i;
            arg_names.push_back(arg_name.str());
            function << argument_type << " " << arg_name.str();

            arg_name.str("");
        }

        function << ")";

        // generate function decleration in header
        *header << function.str() << "; \n \n";

        *header << signature << " " << g_global << g_user << node.Id << " = { " << g_user << node.Id << " }; \n\n";

        // generate function in *output
        *output << function.str() << " { \n"
                   "Start: \n"
                   "\n";


        // generate cases
        for (auto &c: node.Cases) {
            c->accept(*this);
            *output << endl;

            // clear assigments specific for current case
            assignments.clear();
        }

        // generate error, for when program doesn't realize a case
        *output << "    printf(\"No cases realized!\\n\"); \n"
                   "    exit(1); \n"
                   "} \n"
                   " \n";

        // clear arg_names for current function
        arg_names.clear();
    }

    void CCodeGenerator::visit(Case &node)
    {
        stringstream pattern;
        bool empty = true;

        // generate if-statement for matching
        pattern << "    if (";

        id_context = IdContext::PATTERN;
        for (size_t i = 0; i < node.Patterns.size(); i++) {
            // push arg_name on get_value_builder. get_value_builder is used for generate
            // assignments in a case
            get_value_builder.push_back(arg_names[i]);

            // generate pattern
            node.Patterns[i]->accept(*this);

            // cleanup
            get_value_builder.pop_back();

            // only add pattern, if pattern is not "1"
            if (last_pattern != "1") {

                if (!empty)
                    pattern << " && ";

                empty = false;
                pattern << last_pattern;
            }
        }

        pattern << ")";

        // only generate if statement, if the pattern wasn't empty
        if (!empty) {
            *output << pattern.str() << endl;
        }

        *output << "    { \n";

        // generate all nessesary assigments
        for (auto &assign: assignments) {
            *output << "        " << assign << endl;

            if (assign == assignments.back())
                *output << endl;
        }

        // generate return expression
        id_context = IdContext::EXPR;
        expr_stack.push(stringstream());

        if (node.TailRec) {
            auto call = (Call*)node.Expr.get();
            for (size_t i = 0; i < current_func->Signature.Subtypes.size() - 1; i++) {
                expr_stack.push(stringstream());
                call->Args[i]->accept(*this);
                *output << "        " << g_generated << g_arg << i << " = " << expr_stack.top().str() << "; \n";
                expr_stack.pop();
            }

            *output << "\n"
                       "        goto Start; \n";
        } else {
            string result_type = get_type(current_func->Signature.Subtypes.back());
            expr_stack.push(stringstream());

            node.Expr->accept(*this);
            *output << "        " << result_type << " res = " << expr_stack.top().str() << "; \n";
            *output << "        return res; \n";
            expr_stack.pop();
        }

        *output << "    } \n"
                   " \n";
    }

    void CCodeGenerator::visit(Or &node)
    {
        expr_stack.top() << "(";
        node.Left->accept(*this);
        expr_stack.top() << "||";
        node.Right->accept(*this);
        expr_stack.top() << ")";
    }

    void CCodeGenerator::visit(And &node)
    {
        expr_stack.top() << "(";
        node.Left->accept(*this);
        expr_stack.top() << "&&";
        node.Right->accept(*this);
        expr_stack.top() << ")";
    }

    void CCodeGenerator::visit(Equal &node)
    {
        output_equal(node.Ty, *node.Left.get(), *node.Right.get());
    }

    void CCodeGenerator::visit(NotEqual &node)
    {
        expr_stack.top() << "(!";
        output_equal(node.Ty, *node.Left.get(), *node.Right.get());
        expr_stack.top() << ")";
    }

    void CCodeGenerator::output_equal(Type &type, Expression &left, Expression &right) {
        string name;

        switch (type.Id) {
            case TypeId::TUPLE:
                name = get_tuple(left.Ty);

                expr_stack.top() << g_generated << g_compare << name << "(";
                left.accept(*this);
                expr_stack.top() << ", ";
                right.accept(*this);
                expr_stack.top() << ")";
                break;
            case TypeId::LIST:
            case TypeId::STRING:
                name = get_list(left.Ty);

                expr_stack.top() << g_generated << g_compare << name << "(";
                left.accept(*this);
                expr_stack.top() << ", ";
                right.accept(*this);
                expr_stack.top() << ")";
                break;
            default:
                expr_stack.top() << "(";
                left.accept(*this);
                expr_stack.top() << "==";
                right.accept(*this);
                expr_stack.top() << ")";
                break;
        }
    }

    void CCodeGenerator::visit(Lesser &node)
    {
        expr_stack.top() << "(";
        node.Left->accept(*this);
        expr_stack.top() << "<";
        node.Right->accept(*this);
        expr_stack.top() << ")";
    }

    void CCodeGenerator::visit(LesserEq &node)
    {
        expr_stack.top() << "(";
        node.Left->accept(*this);
        expr_stack.top() << "<=";
        node.Right->accept(*this);
        expr_stack.top() << ")";
    }

    void CCodeGenerator::visit(Greater &node)
    {
        expr_stack.top() << "(";
        node.Left->accept(*this);
        expr_stack.top() << ">";
        node.Right->accept(*this);
        expr_stack.top() << ")";
    }

    void CCodeGenerator::visit(GreaterEq &node)
    {
        expr_stack.top() << "(";
        node.Left->accept(*this);
        expr_stack.top() << ">=";
        node.Right->accept(*this);
        expr_stack.top() << ")";
    }

    void CCodeGenerator::visit(Add &node)
    {
        expr_stack.top() << "(";
        node.Left->accept(*this);
        expr_stack.top() << "+";
        node.Right->accept(*this);
        expr_stack.top() << ")";
    }

    void CCodeGenerator::visit(Sub &node)
    {
        expr_stack.top() << "(";
        node.Left->accept(*this);
        expr_stack.top() << "-";
        node.Right->accept(*this);
        expr_stack.top() << ")";
    }

    void CCodeGenerator::visit(Mul &node)
    {
        expr_stack.top() << "(";
        node.Left->accept(*this);
        expr_stack.top() << "*";
        node.Right->accept(*this);
        expr_stack.top() << ")";
    }

    void CCodeGenerator::visit(Div &node)
    {
        expr_stack.top() << "(";
        node.Left->accept(*this);
        expr_stack.top() << "/";
        node.Right->accept(*this);
        expr_stack.top() << ")";
    }

    void CCodeGenerator::visit(Mod &node)
    {
        expr_stack.top() << "(";
        node.Left->accept(*this);
        expr_stack.top() << "%";
        node.Right->accept(*this);
        expr_stack.top() << ")";
    }

    void CCodeGenerator::visit(ListAdd &node)
    {
        string name = get_list(node.Ty);

        // use pregenerated push function to push thing onto list
        // look at generate_list(Type &type) for the generation of this function
        expr_stack.top() << g_generated << g_add << name << "(";
        node.Right->accept(*this);
        expr_stack.top() << ", ";
        node.Left->accept(*this);
        expr_stack.top() << ")";
    }

    void CCodeGenerator::visit(Par &node)
    {
        expr_stack.top() << "(";
        node.Child->accept(*this);
        expr_stack.top() << ")";
    }

    void CCodeGenerator::visit(Not &node)
    {
        expr_stack.top() << "(";
        expr_stack.top() << "!";
        node.Child->accept(*this);
        expr_stack.top() << ")";
    }

    void CCodeGenerator::visit(ListPattern &node)
    {
        // result is needed, so we don't generate something else, while the ListPattern is being generated
        stringstream result;
        string value_gotten;
        string type_name = get_list(node.Ty);

        // generate the gotten value from get_value_builder
        for (auto str : get_value_builder) {
            value_gotten += str;
        }

        result << "(" << value_gotten << "->" << g_size << " - " << list_offsets.back() << " == " << node.Patterns.size() << ")";

        for (size_t i = 0; i < node.Patterns.size(); i++) {
            // add "gat_"name"(" and ", i + offset)" to get_value_builder
            get_value_builder.insert(get_value_builder.begin(),  g_generated + g_valueat + type_name + "(");
            get_value_builder.push_back(", " + to_string(i + list_offsets.back()) + ")");

            // push new offset.
            // lists accesed later down in pattern should not be offseted by the same as current list.
            list_offsets.push_back(0);

            // generate pattern
            node.Patterns[i]->accept(*this);

            // cleanup
            list_offsets.pop_back();
            get_value_builder.pop_back();
            get_value_builder.erase(get_value_builder.begin());

            // don't add pattern, if pattern is "1"
            if (last_pattern != "1") {
                result << " && " << last_pattern;
            }
        }

        last_pattern = "(" + result.str() + ")";
    }

    void CCodeGenerator::visit(TuplePattern &node)
    {
        // result is needed, so we don't start generating something in a signature in the header file
        stringstream result;
        string type_name = get_tuple(node.Ty);
        bool empty = true;

        // iterate through all items in tuple
        for (size_t i = 0; i < node.Patterns.size(); i++) {
            // add ".gi"i"" to get_value_builder
            get_value_builder.push_back("." + g_generated + g_item + to_string(i));

            // generate pattern
            node.Patterns[i]->accept(*this);

            // cleanup
            get_value_builder.pop_back();

            // don't add pattern, if pattern is "1"
            if (last_pattern != "1") {
                if (!empty)
                    result << " && ";

                empty = false;
                result << last_pattern;
            }

        }

        // if empty, then let last_pattern be "1"
        if (empty) {
            last_pattern = "1";
        } else {
            last_pattern = "(" + result.str() + ")";
        }
    }

    void CCodeGenerator::visit(ListSplit &node)
    {
        stringstream result;
        string type_name = get_list(node.Ty);
        bool empty = true;

        // add "gat_"name"(" and ", offset)" to get_value_builder
        // this is done, so that patterns on the left of node, will use
        // the first + offset item in the list
        get_value_builder.insert(get_value_builder.begin(), g_generated + g_valueat + type_name + "(");
        get_value_builder.push_back(", " + to_string(list_offsets.back()) + ")");

        // push new offset.
        // lists accesed later down in pattern should not be offseted by the same as current list.
        list_offsets.push_back(0);

        // generate pattern
        node.Left->accept(*this);

        // cleanup
        list_offsets.pop_back();
        get_value_builder.pop_back();
        get_value_builder.erase(get_value_builder.begin());

        // don't add pattern, if pattern is "1"
        if (last_pattern != "1") {
            empty = false;
            result << last_pattern;
        }

        // right side of a list split, will be the list, but with the first + offset elements missing.
        // this is why we track an offset, so that we don't clone a list, unless we have to.
        list_offsets[list_offsets.size() - 1]++;

        // generate pattern
        node.Right->accept(*this);

        // reverse offset back to what it was
        list_offsets[list_offsets.size() - 1]--;

        // don't add pattern, if pattern is "1"
        if (last_pattern != "1") {
            if (!empty)
                result << " && ";

            empty = false;
            result << last_pattern;
        }

        // if empty, then let last_pattern be "1"
        if (empty) {
            last_pattern = "1";
        } else {
            last_pattern = "(" + result.str() + ")";
        }
    }

    void CCodeGenerator::visit(Int &node)
    {
        // if pattern, then generate an expression for matching on this pattern
        if (id_context == IdContext::PATTERN) {
            string value = "";

            for (auto str : get_value_builder) {
                value += str;
            }

            last_pattern = value + " == " + node.str();

        // else, just output value
        } else {
            expr_stack.top() << "(" << node.Val << ")";
        }
    }

    void CCodeGenerator::visit(Float &node)
    {
        // if pattern, then generate an expression for matching on this pattern
        if (id_context == IdContext::PATTERN) {
            string value = "";

            for (auto str : get_value_builder) {
                value += str;
            }

            last_pattern = value + " == " + node.str();

        // else, just output value
        } else {
            expr_stack.top() << "(" << node.str() << ")";
        }

    }

    void CCodeGenerator::visit(Bool &node)
    {
        // if pattern, then generate an expression for matching on this pattern
        if (id_context == IdContext::PATTERN) {
            string value = "";

            for (auto str : get_value_builder) {
                value += str;
            }

            last_pattern = value + " == " + to_string(node.Val);

        // else, just output value
        } else {
            expr_stack.top() << node.Val;
        }
    }

    void CCodeGenerator::visit(Char &node)
    {
        // if pattern, then generate an expression for matching on this pattern
        if (id_context == IdContext::PATTERN) {
            string value = "";

            for (auto str : get_value_builder) {
                value += str;
            }

            last_pattern = value + " == " + node.str();

        // else, just output value
        } else {
            expr_stack.top() << node.str();
        }
    }

    void CCodeGenerator::visit(String &node)
    {
        // if pattern, then generate an expression for matching on this pattern
        if (id_context == IdContext::PATTERN) {
            string value = "";

            for (auto str : get_value_builder) {
                value += str;
            }

            // gcompare_string is generated by generate_std. it compares the custome string type, to a char*
            // it also takes an offset, for when list splits occur on strings
            last_pattern = g_generated + g_compare + g_string + "(" + value + ", " + node.str() + ", " + to_string(list_offsets.back()) + ")";

        // else, just output value
        } else {
            // gcreate_string is generate by generate_std. It creates string base on a char*
            expr_stack.top() << g_generated << g_create << g_string << "(" << node.str() << ")";
        }
    }

    void CCodeGenerator::visit(List &node)
    {
        string name = get_list(node.Ty);

        // create list
        expr_stack.top() << g_generated << g_create << name << "(" << node.Elements.size();
        for (int i = node.Elements.size() - 1; i >= 0; i--) {
            expr_stack.top() << ", ";

            node.Elements[i]->accept(*this);
        }

        expr_stack.top() << ")";
    }

    void CCodeGenerator::visit(Tuple &node)
    {
        string name = get_tuple(node.Ty);

        // create tuple
        expr_stack.top() << g_generated << g_create << name << "(";
        for (auto &expr: node.Elements){
            expr->accept(*this);

            if (expr != node.Elements.back())
                expr_stack.top() << ", ";
        }
        expr_stack.top() << ")";
    }

    void CCodeGenerator::visit(Id &node)
    {
        stringstream assign;
        string name = "";

        if (id_context == IdContext::PATTERN) {
            // created name from get_value_builder
            for (auto str : get_value_builder) {
                name += str;
            }

            // generate an assignment for the id, which should occur after the if-statement
            assign << get_type(node.Ty) << " " << g_user << node.Val << " = ";

            if ((node.Ty.Id == TypeId::LIST || node.Ty.Id == TypeId::STRING) &&
                 list_offsets.back() > 0) {
                assign << g_generated << g_at << get_list(node.Ty) << "(" << name << ", " << list_offsets.back() << ");";
            } else {
                assign << name << ";";
            }

            // save the assigment untill after the pattern has been generated
            assignments.push_back(assign.str());

            // since and id, in a pattern is allways true, then last_pattern is just set to "1"
            last_pattern = "1";
        } else {
            bool is_declared = false;

            for (auto &func: program->Funcs) {
                if (func->Id == node.Val) {
                    is_declared = true;
                    break;
                }
            }

            if (is_declared) {
                expr_stack.top() << "&" << g_global;
            }

            expr_stack.top() << g_user << node.Val;
        }
    }

    void CCodeGenerator::visit(Call &node)
    {
        string name = g_generated + g_closure + to_string(env_count++);
        string assignment = get_environment(node.Callee->Ty) + "* " + name + " = ";

        expr_stack.push(stringstream());
        // generate the callee (aka, the function being called)
        node.Callee->accept(*this);
        *output << "        " << assignment << expr_stack.top().str() << "; \n";
        expr_stack.pop();

        // generate the arguments the function is being called with
        expr_stack.top() << name << "->call(" << name;
        for (auto &expr: node.Args){
            expr_stack.top() << ", ";

            expr->accept(*this);
        }
        expr_stack.top() << ")";
    }

    string CCodeGenerator::get_type(Type &type) {
        unordered_map<Type, string>::iterator got;

        // generate the currect c type, based on sppl's types
        switch (type.Id) {
            case TypeId::FLOAT:
                return g_float;
            case TypeId::CHAR:
                return g_char;
            case TypeId::INT:
                return g_int;
            case TypeId::BOOL:
                return g_bool;
                // for tuples, lists, closures and strings, custom types will be generated
            case TypeId::TUPLE:
                return get_tuple(type);
            case TypeId::SIGNATURE:
                return get_environment(type) + "*";
            case TypeId::STRING:
                return string_type_name + "*";
            case TypeId::LIST:
                return get_list(type) + "*";
            default:
                // This should never happen. If it does, the type checker made a mistake.
                return "This really shouldn't happen.";
        }
    }

    string CCodeGenerator::generate_list(Type &type) {
        // save the generated list in a result stream
        stringstream result;
        string name = g_generated + g_list + to_string(list_count);
        string children_type = get_type(type.Subtypes.front());

        // increase list count, so next list doesn't have the same name
        list_count++;

        // generate the list struct
        *header << " \n"
                   "typedef struct " << name << " { \n"
                   "    struct " << name << "* " << g_next << "; \n"
                   "    " << children_type << " " << g_value << "; \n"
                   "    int " << g_empty << "; \n"
                   "    int " << g_size << "; \n"
                   "} " << name << "; \n"
                   " \n";


        // generate add function for list
        *header << name << "* " << g_generated << g_add << name << "(" << name << "* current, " << children_type << " item); \n";
        buffer << name << "* " << g_generated << g_add << name << "(" << name << "* current, " << children_type << " item) { \n"
                   "    " << name << "* res = malloc(sizeof(" << name << "));\n"
                   "    res->" << g_value << " = item; \n"
                   "    res->" << g_next << " = current; \n"
                   "    res->" << g_empty << " = 0; \n"
                   "    res->" << g_size << " = current->" << g_size << " + 1; \n"
                   "    return res; \n"
                   "} \n"
                   " \n";


        // generate create function for list
        *header << name << "* " << g_generated << g_create << name << "(int count, ...); \n";
        buffer << name << "* " << g_generated << g_create << name << "(int count, ...) { \n"
                  "    int i; \n"
                  "    va_list args; \n"
                  "    " << name << "* res = malloc(sizeof(" << name << ")); \n"
                  "    res->" << g_empty << " = 1; \n"
                  "    res->" << g_size << " = 0; \n"
                  "\n"
                  "    va_start(args, count); \n"
                  "\n"
                  "    for (i = 0; i < count; i++) { \n"
                  "        res = " << g_generated << g_add << name << "(res, va_arg(args, " << children_type << ")); \n"
                  "    } \n"
                  "\n"
                  "    va_end(args); \n"
                  "    return res; \n"
                  "} \n"
                  " \n";


        // generate at function for list
        *header << name << "* " << g_generated << g_at << name << "(" << name << "* current, int index); \n";
        buffer << name << "* " << g_generated << g_at << name << "(" << name << "* current, int index) { \n"
                   "    while (index-- > 0) { \n"
                   "        if (current->" << g_empty << ") { \n"
                   "            return current; \n"
                   "        } \n"
                   "\n"
                   "        current = current->" << g_next << "; \n"
                   "    } \n"
                   "\n"
                   "    return current; \n"
                   "} \n"
                   " \n";

        // generate valueat function for list
        *header << children_type << " " << g_generated << g_valueat << name << "(" << name << "* current, int index); \n";
        buffer << children_type << " " << g_generated << g_valueat << name << "(" << name << "* current, int index) { \n"
                   "    " << name << "* res = " << g_generated << g_at << name << "(current, index); \n"
                   "\n"
                   "    if (res->" << g_empty << ") { \n"
                   "        printf(\"Out of bound! " << oob_count++ << "\\n\"); \n"
                   "        exit(1); \n"
                   "    }"
                   "\n"
                   "    return res->" << g_value << "; \n"
                   "} \n"
                   " \n";

        // generate compare function for list
        *header << "int " << g_generated << g_compare << name << "(" << name << "* list1, " << name << "* list2); \n";
        buffer << "int " << g_generated << g_compare << name << "(" << name << "* list1, " << name << "* list2) { \n"
                   "    int i; \n"
                   "    if (list1->" << g_size << " != list2->" << g_size << ") { \n"
                   "         return 0; \n"
                   "    } \n"
                   " \n"
                   "    for (i = 0; i < list1->" << g_size << "; i++) { \n"
                   "        if(";

        switch (type.Subtypes.front().Id) {
            case TypeId::LIST:
            case TypeId::TUPLE:
                buffer << "!" << g_generated << g_compare << children_type <<
                        "(" << g_generated << g_valueat << name << "(list1, i), " << g_generated << g_valueat << name << "(list2, i))";
                break;
            default:
                buffer << g_generated << g_valueat << name << "(list1, i) != " << g_generated << g_valueat << name << "(list2, i)";
                break;
        }

        buffer << ") \n"
                  "            return 0; \n"
                  "    } \n"
                  " \n"
                  "    return 1; \n"
                  "} \n"
                  " \n";

        // generation of concat
        *header << name << "* " << g_generated << g_concat << name << "(" << name << "* list1, " << name << "* list2); \n";
        buffer << name << "* " << g_generated << g_concat << name << "(" << name << "* list1, " << name << "* list2) { \n"
                   "    int i; \n"
                   "    " << name << "** elements = malloc(sizeof(" << name << "*) * list1->" << g_size <<"); \n"
                   " \n"
                   "    for (i = 0; !list1->" << g_empty << "; ++i) { \n"
                   "        elements[i] = list1; \n"
                   "        list1 = list1->" << g_next << "; \n"
                   "    } \n"
                   " \n"
                   "    for (--i; i >= 0; --i) { \n"
                   "        list2 = " << g_generated << g_add << name << "(list2, elements[i]->" << g_value << "); \n"
                   "    } \n"
                   " \n"
                   "    free(elements); \n"
                   "    return list2; \n"
                   "} \n"
                   "\n";

        // generation of tostring
        to_strings[type] = g_generated + g_tostring + name;

        *header << string_type_name << "* " << to_strings[type] << "(" << name << "* value); \n";
        buffer << string_type_name << "* " << to_strings[type] << "(" << name << "* value) { \n"
                "    " << string_type_name << "* comma = " << g_generated << g_create << g_string << "(\", \"); \n"
                "    " << string_type_name << "* res = " << g_generated << g_create << g_string << "(\"]\"); \n"
                "    int i; \n"
                "\n"
                "    for (i = value->" << g_size << " - 1; i >= 0; i--) { \n"
                "        res = " << g_generated << g_concat << string_type_name << "(" << to_strings[type.Subtypes.front()] << "(" << g_generated << g_valueat << name << "(value, i)), res); \n"
                " \n"
                "        if (i != 0) \n"
                "            res = " << g_generated << g_concat << string_type_name << "(comma, res); \n"
                "    } \n"
                " \n"
                "    res = " << g_generated << g_add << string_type_name << "(res, '['); \n"
                "    return res; \n"
                "} \n"
                " \n";


        lists[type] = name;

        // return name of list generated
        return name;
    }

    string CCodeGenerator::generate_environment(Type &type) {
        // result is needed, so we don't generate something inside the signature, while generating other types
        stringstream result;
        string name = g_generated + g_signature + to_string(sig_count);

        // increase signature count, so next signature doesn't have the same name
        sig_count++;

        result << "typedef struct " << name << " {\n"
               << "    " << get_type(type.Subtypes.back()) << " (* call)(struct " << name << "*";

        for (size_t i = 0; i < type.Subtypes.size() - 1; ++i) {
            result << ", ";

            // get the actual type of the argument
            result << get_type(type.Subtypes[i]);
        }

        result << "); \n"
                  "} " << name << ";\n\n";

        *header << "\n" << result.str();

        // generation of signature to string
        to_strings[type] = g_generated + g_tostring + name;

        *header << string_type_name << "* " << to_strings[type] << "(" << name << " value); \n";
        buffer << string_type_name << "* " << to_strings[type] << "(" << name << " value) { \n"
                   "    return " << g_generated << g_create << g_string << "(\"" << type.str() << "\"); \n"
                   "} \n"
                   " \n";

        // save signature in signature hash map
        closures[type] = name;

        // return name of signature generated
        return name;
    }

    string CCodeGenerator::generate_tuple(Type &type) {
        // result is needed, so we don't start generating something while generating the tuple
        stringstream result;
        string name = g_generated + g_tuple + to_string(tuple_count);

        // increase tuple count, so next tuple doesn't have the same name
        tuple_count++;

        // generate the tuple struct
        result << " \n"
                  "typedef struct " << name << " {" << endl;
        // generate an item for each type in the tuple
        for (size_t i = 0; i < type.Subtypes.size(); ++i) {
            // get the actual type of the item
            result << "    " << get_type(type.Subtypes[i]) << " " << g_generated << g_item << i << ";" << endl; // give this item a unique name

        }
        result << "} " << name << "; \n";

        *header << result.str();
        result.str("");

        // generate a create function for the tuple
        // give contructor a unique name
        result << name << " " << g_generated << g_create << name << "(";

        // generate an argument for each item in the struct
        for (size_t i = 0; i < type.Subtypes.size(); ++i) {
            // get the actual type of the argument
            result << get_type(type.Subtypes[i]) << " " << g_generated << g_item << i;

            if (i < type.Subtypes.size() - 1) // don't print ", " after the last argument
                result << ", ";
        }

        result << ")";

        *header << result.str() << "; \n";
        buffer << result.str() << " { \n";

        // generate a result variable
        buffer << "    " << name << " " << "res; \n";

        // for each item in res, assign values
        for (size_t i = 0; i < type.Subtypes.size(); ++i) {
            buffer << "    res." << g_generated << g_item << i << " = " << g_generated << g_item << i << "; \n";
        }
        buffer << " \n"
                   "    return res; \n"
                   "} \n"
                   " \n";

        // generate a compare function for this tuple
        *header << "int " << g_generated << g_compare << name << "(" << name << " tuple1, " << name << " tuple2); \n";
        buffer << "int " << g_generated << g_compare << name << "(" << name << " tuple1, " << name << " tuple2) { \n";

        for (size_t i = 0; i < type.Subtypes.size(); ++i) {
            buffer << "    if (";

            switch (type.Subtypes[i].Id) {
                case TypeId::STRING:
                case TypeId::LIST:
                    buffer << "!" << g_generated << g_compare << lists[type.Subtypes[i]] <<
                               "(tuple1." << g_generated << g_item << i << ", tuple2." << g_generated << g_item << i << ")";
                    break;
                case TypeId::TUPLE:
                    buffer << "!" << g_generated << g_compare << tuples[type.Subtypes[i]] <<
                               "(tuple1." << g_generated << g_item << i << ", tuple2." << g_generated << g_item << i << ")";
                    break;
                default:
                    buffer << "(tuple1." << g_generated << g_item << i << " != tuple2." << g_generated << g_item << i << ")";
                    break;
            }

            buffer << ") \n"
                      "        return 0; \n";
        }

        buffer << " \n"
                  "    return 1; \n"
                  "} \n"
                  "\n";


        // generate a to_string for the tuple
        to_strings[type] = g_generated + g_tostring + name;

        *header << string_type_name << "* " << to_strings[type] << "(" << name << " value); \n";
        buffer << string_type_name << "* " << to_strings[type] << "(" << name << " value) { \n"
                   "    " << string_type_name << "* comma = " << g_generated << g_create << g_string << "(\", \"); \n"
                   "    " << string_type_name << "* res = " << g_generated << g_create << g_string << "(\")\"); \n"
                   "\n";

        for (size_t i = type.Subtypes.size() - 1; i != 0; --i) {
            buffer << "    res = " << g_generated << g_concat << string_type_name << "(" << to_strings[type.Subtypes[i]] << "(value." << g_generated << g_item << i << ")" << ", res); \n";
            buffer << "    res = " << g_generated << g_concat << string_type_name << "(comma, res); \n";
        }


        buffer << "    res = " << g_generated << g_concat << string_type_name << "(" << to_strings[type.Subtypes[0]] << "(value." << g_generated << g_item << "0)" << ", res); \n"
                   "    res = " << g_generated << g_add << string_type_name << "(res, '('); \n"
                   " \n"
                   "    return res; \n"
                   "} \n"
                   "\n";

        // save tuple in tuple hash map
        tuples[type] = name;

        // return name of tuple generated
        return name;
    }

    void CCodeGenerator::generate_std() {
        *output << "#include \"test.h\" \n \n";

        *header << "#include <stdarg.h> \n"
                   "#include <stdio.h> \n"
                   "#include <stdlib.h> \n"
                   "#include <string.h> \n"
                   "#include <stdint.h> \n"
                   "#include <inttypes.h> \n";

        to_strings[Type(TypeId::INT)] = g_generated + g_tostring + "int";
        to_strings[Type(TypeId::BOOL)] = g_generated + g_tostring + "bool";
        to_strings[Type(TypeId::FLOAT)] = g_generated + g_tostring + "float";
        to_strings[Type(TypeId::CHAR)] = g_generated + g_tostring + "char";
        to_strings[Type(TypeId::STRING)] = g_generated + g_tostring + "string";

        string_type_name = g_generated + g_list + to_string(list_count);
        lists[Type(TypeId::STRING)] = string_type_name;

        generate_list(real_string);
        generate_list(string_list);

        output_buffer();

        // generation of string constructer starts here
        *header << string_type_name << "* " << g_generated << g_create << g_string << "(char* values);\n";
        *output << string_type_name << "* " << g_generated << g_create << g_string << "(char* values) { \n"
                  "    int i, str_length = strlen(values); \n"
                  "    " << string_type_name << "* res = " << g_generated << g_create << string_type_name << "(0); \n"
                  " \n"
                  "    for (i = str_length - 1; i >= 0; i--) { \n"
                  "        res = " << g_generated << g_add << string_type_name << "(res, values[i]); \n"
                  "    } \n"
                  " \n"
                  "    return res; \n"
                  "} \n"
                  " \n";

        // generation of string compare
        *header << "int " << g_generated << g_compare << g_string << "(" << string_type_name << "* string, char* values, int offset); \n";
        *output << "int " << g_generated << g_compare << g_string << "(" << string_type_name << "* string, char* values, int offset) { \n"
                   "    int i, size = strlen(values); \n"
                   " \n"
                   "    if (size == string->" << g_size << ") { \n"
                   "        for (i = 0; i < size; i++) { \n"
                   "            if (" << g_generated << g_valueat << string_type_name << "(string, i) != values[i]) \n"
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
        *header << string_type_name << "* " << g_generated << g_print << g_string << "(" << string_type_name << "* string); \n";
        *output << string_type_name << "* " << g_generated << g_print << g_string << "(" << string_type_name << "* string) { \n"
                   "    char* buffer = malloc(sizeof(char) * (string->" << g_size << " + 1)); \n"
                   "    int i; \n"
                   " \n"
                   "    for (i = 0; i < string->" << g_size << "; i++) { \n"
                   "        buffer[i] = (char)" << g_generated << g_valueat << string_type_name << "(string, i); \n"
                   "    } \n"
                   " \n"
                   "    buffer[i] = '\\0'; \n"
                   "    printf(\"%s\\n\", buffer); \n"
                   "    free(buffer); \n"
                   "    return string; \n"
                   "} \n"
                   " \n";


        // generation of default to_string methods
        *header << string_type_name << "* " << to_strings[Type(TypeId::INT)] << "(" << g_int << " value); \n";
        *output << string_type_name << "* " << to_strings[Type(TypeId::INT)] << "(" << g_int << " value) { \n"
                   "    char buffer[100]; \n"
                   "    sprintf(buffer, \"%\" PRId64 \"\", value); \n"
                   "    return " << g_generated << g_create << g_string << "(buffer); \n"
                   "} \n"
                   " \n";

        *header << string_type_name << "* " << to_strings[Type(TypeId::BOOL)] << "(" << g_bool << " value); \n";
        *output << string_type_name << "* " << to_strings[Type(TypeId::BOOL)] << "(" << g_bool << " value) { \n"
                   "    if (value) \n"
                   "        return " << g_generated << g_create << g_string << "(\"True\"); \n"
                   "    else \n"
                   "        return " << g_generated << g_create << g_string << "(\"False\"); \n"
                   "} \n"
                   " \n";

        *header << string_type_name << "* " << to_strings[Type(TypeId::FLOAT)] << "(" << g_float << " value); \n";
        *output << string_type_name << "* " << to_strings[Type(TypeId::FLOAT)] << "(" << g_float << " value) { \n"
                   "    char buffer[100]; \n"
                   "    sprintf(buffer, \"%lf\", value); \n"
                   "    return " << g_generated << g_create << g_string << "(buffer); \n"
                   "} \n"
                   " \n";

        *header << string_type_name << "* " << to_strings[Type(TypeId::CHAR)] << "(" << g_char << " value); \n";
        *output << string_type_name << "* " << to_strings[Type(TypeId::CHAR)] << "(" << g_char << " value) { \n"
                   "    " << string_type_name << "* res = " << g_generated << g_create << g_string << "(\"\\'\"); \n"
                   "    res = " << g_generated << g_add << string_type_name << "(res, value); \n"
                   "    res = " << g_generated << g_add << string_type_name << "(res, '\\''); \n"
                   "    return res; \n"
                   "} \n"
                   " \n";

        *header << string_type_name << "* " << to_strings[Type(TypeId::STRING)] << "(" << string_type_name << "* value); \n";
        *output << string_type_name << "* " << to_strings[Type(TypeId::STRING)] << "(" << string_type_name << "* value) { \n"
                   "    " << string_type_name << "* res = " << g_generated << g_create << g_string << "(\"\\\"\"); \n"
                   "    res = " << g_generated << g_concat << string_type_name << "(value, res); \n"
                   "    res = " << g_generated << g_add << string_type_name << "(res, '\"'); \n"
                   "} \n"
                   " \n";
    }

    string CCodeGenerator::get_list(Type &type) {
        auto got = lists.find(type);

        if (got == lists.end()) {
            return  generate_list(type);
        } else {
            return got->second;
        }
    }

    string CCodeGenerator::get_tuple(Type &type) {
        auto got = tuples.find(type);

        if (got == tuples.end()) {
            return  generate_tuple(type);
        } else {
            return got->second;
        }
    }

    string CCodeGenerator::get_environment(Type &type) {
        auto got = closures.find(type);

        if (got == closures.end()) {
            return  generate_environment(type);
        } else {
            return got->second;
        }
    }

    void CCodeGenerator::output_buffer() {
        *output << buffer.str(),
        buffer.str(string());
    }
}
