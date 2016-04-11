#include "CppCodeGenerator.h"
#include <string>
#include <iostream>
#include <sstream>
#include <Scope.h>

using namespace common;
using namespace std;

namespace codegen
{
    CCodeGenerator::CCodeGenerator(shared_ptr<std::ostream> &out, shared_ptr<std::ostream> &head)
            : CodeGenerator::CodeGenerator(out), header(head)
    {
        _char = make_shared<Type>(Types::CHAR);
        real_string = make_shared<Type>(Types::LIST);
        fake_string = make_shared<Type>(Types::STRING);
        string_list = make_shared<Type>(Types::LIST);
        real_string->types.push_back(_char);
        string_list->types.push_back(fake_string);
        list_offsets.push_back(0);
    }

    void CCodeGenerator::visit(Program &node)
    {
        generate_std();

        *output << "#include \"test.h\" \n"
                  " \n"
                  "int main(int argc, char** argv) { \n"
                  "    " << lists[*string_list] << " *args = " << g_generated << g_create << lists[*string_list] << "(0); \n"
                  "    int i; \n"
                  " \n"
                  "    for(i = argc - 1; i >= 0; i--) { \n"
                  "        " << g_generated << g_push << lists[*string_list] << "(args, " << g_generated << g_create << g_string << "(argv[i])); \n"
                  "    } \n"
                  " \n"
                  "    printf(\"%d\\n\", " << g_user << g_main << "(args)); \n"
                  "    return 0; \n"
                  "} \n"
                  " \n";

        for (auto f : node.funcs) {
            f->accept(*this);
            *output << endl;
        }
    }

    void CCodeGenerator::visit(Function &node)
    {
        stringstream function;
        stringstream arg_name;

        current_func = &node;

        node.types.back()->accept(*this);

        // generate function name and return type
        function << last_type << " " << g_user << node.id << "(";

        // generate function arguments
        for (size_t i = 0; i < node.types.size() - 1; i++) {
            node.types[i]->accept(*this);
            arg_name << g_generated << g_arg << i;
            arg_names.push_back(arg_name.str());
            function << last_type << " " << arg_name.str();

            if (i < node.types.size() - 2) // don't generate ", " after last argument
                function << ", ";

            arg_name.str("");
        }

        function << ")";

        // generate function decleration in header
        *header << function.str() << ";" << endl;
        *header << endl;

        // generate function in *output
        *output << function.str() << " { \n";

        // generate cases
        for (auto c : node.cases) {
            c->accept(*this);
            *output << endl;

            // clear assigments specific for current case
            listnames_offset.clear();
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
        for (size_t i = 0; i < node.patterns.size(); i++) {
            // push arg_name on get_value_builder. get_value_builder is used for generate
            // assignments in a case
            get_value_builder.push_back(arg_names[i]);

            // generate pattern
            node.patterns[i]->accept(*this);

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
        for (auto assign : assignments) {
            *output << "        " << assign << endl;

            if (assign == assignments.back())
                *output << endl;
        }

        // generate return expression
        id_context = IdContext::EXPR;
        current_func->types.back()->accept(*this);
        *output << "        " << last_type << " res = ";
        node.expr->accept(*this);
        *output << ";" << endl;

        *output <<  "        return res; \n"
                   "    } \n"
                   " \n";
    }

    void CCodeGenerator::visit(Or &node)
    {
        *output << "(";
        node.left->accept(*this);
        *output << "||";
        node.right->accept(*this);
        *output << ")";
    }

    void CCodeGenerator::visit(And &node)
    {
        *output << "(";
        node.left->accept(*this);
        *output << "&&";
        node.right->accept(*this);
        *output << ")";
    }

    void CCodeGenerator::visit(Equal &node)
    {
        unordered_map<Type, string>::iterator got;
        string name;

        switch (node.left->node_type->type) {
            case Types::TUPLE:
                got = tuples.find(*node.left->node_type);

                if (got == tuples.end()) {
                    name = generate_tuple(*node.left->node_type);
                } else {
                    name = got->second;
                }

                *output << g_generated << g_compare << name << "(";
                node.left->accept(*this);
                *output << ", ";
                node.right->accept(*this);
                *output << ")";
                break;
            case Types::LIST:
            case Types::STRING:
                got = lists.find(*node.left->node_type);

                if (got == tuples.end()) {
                    name = generate_list(*node.left->node_type);
                } else {
                    name = got->second;
                }

                *output << g_generated << g_compare << name << "(";
                node.left->accept(*this);
                *output << ", ";
                node.right->accept(*this);
                *output << ")";
                break;
            default:
                *output << "(";
                node.left->accept(*this);
                *output << "==";
                node.right->accept(*this);
                *output << ")";
                break;
        }
    }

    void CCodeGenerator::visit(NotEqual &node)
    {
        Equal eq;
        eq.left = node.left;
        eq.right = node.right;

        *output << "(!";
        eq.accept(*this);
        *output << ")";
    }

    void CCodeGenerator::visit(Lesser &node)
    {
        *output << "(";
        node.left->accept(*this);
        *output << "<";
        node.right->accept(*this);
        *output << ")";
    }

    void CCodeGenerator::visit(LesserEq &node)
    {
        *output << "(";
        node.left->accept(*this);
        *output << "<=";
        node.right->accept(*this);
        *output << ")";
    }

    void CCodeGenerator::visit(Greater &node)
    {
        *output << "(";
        node.left->accept(*this);
        *output << ">";
        node.right->accept(*this);
        *output << ")";
    }

    void CCodeGenerator::visit(GreaterEq &node)
    {
        *output << "(";
        node.left->accept(*this);
        *output << ">=";
        node.right->accept(*this);
        *output << ")";
    }

    void CCodeGenerator::visit(Add &node)
    {
        *output << "(";
        node.left->accept(*this);
        *output << "+";
        node.right->accept(*this);
        *output << ")";
    }

    void CCodeGenerator::visit(Sub &node)
    {
        *output << "(";
        node.left->accept(*this);
        *output << "-";
        node.right->accept(*this);
        *output << ")";
    }

    void CCodeGenerator::visit(Mul &node)
    {
        *output << "(";
        node.left->accept(*this);
        *output << "*";
        node.right->accept(*this);
        *output << ")";
    }

    void CCodeGenerator::visit(Div &node)
    {
        *output << "(";
        node.left->accept(*this);
        *output << "/";
        node.right->accept(*this);
        *output << ")";
    }

    void CCodeGenerator::visit(Mod &node)
    {
        *output << "(";
        node.left->accept(*this);
        *output << "%";
        node.right->accept(*this);
        *output << ")";
    }

    void CCodeGenerator::visit(ListAdd &node)
    {
        string name;
        auto got = lists.find(*node.right->node_type);

        // If current list doesn't exists. Create it
        if (got == lists.end()) {
            name = generate_list(*node.node_type);
        } else {
            name = got->second;
        }

        // use pregenerated push function to push thing onto list
        // look at generate_list(Type &type) for the generation of this function
        *output << g_generated << g_push << name << "(";
        node.right->accept(*this);
        *output << ", ";
        node.left->accept(*this);
        *output << ")";
    }

    void CCodeGenerator::visit(Par &node)
    {
        *output << "(";
        node.child->accept(*this);
        *output << ")";
    }

    void CCodeGenerator::visit(Not &node)
    {
        *output << "(";
        *output << "!";
        node.child->accept(*this);
        *output << ")";
    }

    void CCodeGenerator::visit(ListPattern &node)
    {
        // result is needed, so we don't generate something else, while the ListPattern is being generated
        stringstream result;
        string value_gotten = "";

        // visit nodes type, to ensure that a list of this type has be generated
        node.node_type->accept(*this);

        // generate the gotten value from get_value_builder
        for (auto str : get_value_builder) {
            value_gotten += str;
        }

        result << "(" << value_gotten << "->" << g_head << " - " << list_offsets.back() << " == " << node.patterns.size() << ")";

        for (size_t i = 0; i < node.patterns.size(); i++) {
            // add "gat_"name"(" and ", i + offset)" to get_value_builder
            get_value_builder.insert(get_value_builder.begin(),  g_generated + g_at + lists[*node.node_type] + "(");
            get_value_builder.push_back(", " + to_string(i + list_offsets.back()) + ")");

            // push new offset.
            // lists accesed later down in pattern should not be offseted by the same as current list.
            list_offsets.push_back(0);

            // generate pattern
            node.patterns[i]->accept(*this);

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
        bool empty = true;

        // visit nodes type, to ensure that a tuple of this type has be generated
        node.node_type->accept(*this);

        // iterate through all items in tuple
        for (size_t i = 0; i < node.patterns.size(); i++) {
            // add ".gi"i"" to get_value_builder
            get_value_builder.push_back("." + g_generated + g_item + to_string(i));

            // generate pattern
            node.patterns[i]->accept(*this);

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
        bool empty = true;

        // add "gat_"name"(" and ", offset)" to get_value_builder
        // this is done, so that patterns on the left of node, will use
        // the first + offset item in the list
        get_value_builder.insert(get_value_builder.begin(), g_generated + g_at + lists[*node.node_type] + "(");
        get_value_builder.push_back(", " + to_string(list_offsets.back()) + ")");

        // push new offset.
        // lists accesed later down in pattern should not be offseted by the same as current list.
        list_offsets.push_back(0);

        // generate pattern
        node.left->accept(*this);

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
        node.right->accept(*this);

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

            last_pattern = value + " == " + to_string(node.value);

        // else, just output value
        } else {
            *output << node.value;
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

            last_pattern = value + " == " + to_string(node.value);

        // else, just output value
        } else {
            *output << node.value;
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

            last_pattern = value + " == " + to_string(node.value);

        // else, just output value
        } else {
            *output << node.value;
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

            last_pattern = value + " == '" + node.value + "'";

        // else, just output value
        } else {
            *output << "'" << node.value << "'";
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
            last_pattern = g_generated + g_compare + g_string + "(" + value + ", \"" + node.value + "\", " + to_string(list_offsets.back()) + ")";

        // else, just output value
        } else {
            // gcreate_string is generate by generate_std. It creates string base on a char*
            *output << g_generated << g_create << g_string << "(\"" << node.value << "\")";
        }
    }

    void CCodeGenerator::visit(List &node)
    {
        string name;

        // find generated name for the list
        auto got = lists.find(*node.node_type);

        // If current list doesn't exists. Create it
        if (got == lists.end()) {
            name = generate_list(*node.node_type);
        } else {
            name = got->second;
        }

        // create list
        *output << g_generated << g_create << name << "(" << node.exprs.size();
        for (int i = node.exprs.size() - 1; i >= 0; i--) {
            *output << ", ";

            node.exprs[i]->accept(*this);
        }

        *output << ")";
    }

    void CCodeGenerator::visit(Tuple &node)
    {
        string name;

        // find generated name for the tuple
        auto got = tuples.find(*(node.node_type));

        // If current tuple doesn't exists. Create it
        if (got == tuples.end()) {
            name = generate_tuple(*node.node_type);
        } else {
            name = got->second;
        }

        // create tuple
        *output << g_generated << g_create << name << "(";
        for (auto expr : node.exprs){
            expr->accept(*this);

            if (expr != node.exprs.back())
                *output << ", ";
        }
        *output << ")";
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

            // get id's type
            node.node_type->accept(*this);

            // generate an assignment for the id, which should occur after the if-statement
            assign << last_type << " " << g_user << node.id << " = ";

            // if id is of a type that is allocated on the heap (aka strings and lists)
            // then make a copy of it, since the language should have no sideeffects
            // there could also be an offset, the lists and string should be copied with
            //if (node.node_type->type == Types::LIST || node.node_type->type == Types::STRING) {
            //    assign << g_generated << g_clone << lists[*node.node_type] << "(" << name << ", " << list_offsets.back() << ");";
            //} else {
                assign << name << ";";
                listnames_offset.insert({node.id, list_offsets.back()});
            //}

            // save the assigment untill after the pattern has been generated
            assignments.push_back(assign.str());

            // since and id, in a pattern is allways true, then last_pattern is just set to "1"
            last_pattern = "1";
        } else {
            switch (node.node_type->type){
                // We assume, that if the programmer uses an id, that is of type list or string,
                // then they indent of making changes to it. A clone is therefor made, so no
                // sideeffects occur in the program.
                case Types::LIST:
                case Types::STRING:
                    *output << g_generated << g_clone << lists[*node.node_type] << "(" << g_user << node.id << ", " << listnames_offset[node.id] << ")";
                    break;
                default:
                    *output << g_user << node.id;
                    break;
            }
        }
    }

    void CCodeGenerator::visit(Call &node)
    {
        *output << "(";
        // generate the callee (aka, the function being called)
        node.callee->accept(*this);

        // generate the arguments the function is being called with
        *output << "(";
        for (auto expr : node.exprs){
            expr->accept(*this);

            if (expr != node.exprs.back())
                *output << ", ";
        }
        *output << ")";

        *output << ")";
    }

    void CCodeGenerator::visit(Type &node) {
        unordered_map<Type, string>::iterator got;

        // generate the currect c type, based on sppl's types
        switch (node.type) {
            case Types::FLOAT:
                last_type = g_float;
                break;
            case Types::CHAR:
                last_type = g_char;
                break;
            case Types::INT:
                last_type = g_int;
                break;
            case Types::BOOL:
                last_type = g_bool;
                break;
            // for tuples, lists, signatures and strings, custom types will be generated
            case Types::TUPLE:
                // find custom type
                got = tuples.find(node);

                if (got != tuples.end()) {
                    last_type = got->second;
                } else {
                    // if type didn't exist, then generate it
                    last_type = generate_tuple(node);
                }
                break;
            case Types::SIGNATURE:
                // find custom type
                got = signatures.find(node);

                if (got != signatures.end()) {
                    last_type = got->second;
                } else {
                    // if type didn't exist, then generate it
                    last_type = generate_signature(node);
                }
                break;
            case Types::STRING:
                // the string type is generate at the start of the generation.
                // we therefor just use the name that this type was give.
                last_type = string_type_name;

                // strings are allocated on the heap, and we therefor need a pointer to it.
                last_type += "*";
                break;
            case Types::LIST:
                // find custom type
                got = lists.find(node);

                if (got != lists.end()) {
                    last_type = got->second;
                } else {
                    // if type didn't exist, then generate it
                    last_type = generate_list(node);
                }

                // lists are allocated on the heap, and we therefor need a pointer to it.
                last_type += "*";
                break;
            default:
                break;
        }
    }

    string CCodeGenerator::generate_list(Type &type) {
        // while generating a type, other types could be generated at the same time.
        // we therefor need to save our generated code in a temporary variable, so
        // that these generations doesn't write on top of eachother
        stringstream result;
        string name = g_generated + g_list;
        name += to_string(list_count);

        // increase list count, so next list doesn't have the same name
        list_count++;

        // get name of the items types
        type.types.front()->accept(*this);

        /* generation of list starts here */
        result << "typedef struct " << name << " { \n"
                  "    " << last_type << "* items; \n"
                  "    int " << g_head << "; \n"
                  "    int " << g_size << "; \n"
                  "} " << name << "; \n"
                  " \n";
        /* generation of list ends here */

        /* generation of list push starts here */
        result << name << "* " << g_generated << g_push << name << "(" << name << "* this, " << last_type << " item) { \n"
                  "    if (this->" << g_head << " >= this->" << g_size << " - 1) { \n"
                  "        this->" << g_size << " *= 2; \n"
                  "        this->" << g_items << " = realloc(this->" << g_items << ", this->" << g_size << " * sizeof(" << last_type << ")); \n"
                  "    } \n"
                  " \n"
                  "    this->" << g_items << "[this->" << g_head << "++] = item; \n"
                  "    return this; \n"
                  "} \n"
                  " \n";
        /* generation of list push ends here */

        /* generation of list constructer starts here */
        result << name << "* " << g_generated << g_create << name << "(int count, ...) { \n"
                  "    int i, size = " << g_generated << g_nearpow2 << "(count); \n"
                  "    " << name << "* res = malloc(sizeof(" << name << ")); \n"
                  " \n"
                  "    res->" << g_head << " = 0; \n"
                  "    res->" << g_size << " = size; \n"
                  "    res->" << g_items << " = malloc(sizeof(" << name << ")); \n"
                  " \n"
                  "    va_list args; \n"
                  "    va_start(args, count); \n"
                  " \n"
                  "    for(i = 0; i < count; i++) { \n"
                  "        " << g_generated << g_push << name << "(res, va_arg(args, " << last_type << ")); \n"
                  "    } \n"
                  " \n"
                  "    va_end(args); \n"
                  "    return res; \n"
                  "} \n"
                  " \n";
        /* generation of list constructer ends here */

        /* generation of at function starts here */
        result << last_type << " " << g_generated << g_at << name << "(" << name << "* this, int index) { \n"
                  "    if (index >= this->" << g_head << ") { \n"
                  "        printf(\"Out of bound!\"); \n"
                  "        exit(1); \n"
                  "     } \n"
                  " \n"
                  "     return this->" << g_items << "[this->" << g_head << " - (1 + index)]; \n"
                  "} \n"
                  " \n";
        /* generation of at function ends here */

        /* generation of list constructer starts here */
        result << name << "* " << g_generated << g_clone << name << "(" << name << "* list, int offset) { \n"
                  "    " << name << "* res = malloc(sizeof(" << name << ")); \n"
                  " \n"
                  "    res->" << g_head << " = list->" << g_head << " - offset; \n"
                  "    res->" << g_size << " = list->" << g_size << "; \n"
                  "    res->" << g_items << " = malloc(list->" << g_size << " * sizeof(" << last_type << ")); \n"
                  " \n"
                  "    memcpy(res->" << g_items << ", list->" << g_items << ", res->" << g_head << " * sizeof(" << last_type << ")); \n"
                  " \n"
                  "    return res; \n"
                  "} \n"
                  " \n";
        /* generation of list constructer ends here */

        /* generation of compare function starts here */
        result << "int " << g_generated << g_compare << name << "(" << name << "* list1, " << name << "* list2) { \n"
                  "    int i; \n"
                  "    if (list1->" << g_head << " != list2->" << g_head << ") { \n"
                  "         return 0; \n"
                  "    } \n"
                  " \n"
                  "    for (i = 0; i < list1->" << g_head << "; i++) { \n"
                  "        if(";

        switch (type.types.front()->type) {
            case Types::LIST:
                result << "!" << g_generated << g_compare << lists[*type.types.front()] << "(list1->" << g_items << "[i], list2->" << g_items << "[i])";
                break;
            case Types::TUPLE:
                result << "!" << g_generated << g_compare << tuples[*type.types.front()] << "(list1->" << g_items << "[i], list2->" << g_items << "[i])";
                break;
            default:
                result << "list1->" << g_items << "[i] != list2->" << g_items << "[i]";
                break;
        }

        result << ") \n"
                  "            return 0; \n"
                  "    } \n"
                  " \n"
                  "    return 1; \n"
                  "} \n"
                  " \n";
        /* generation of compare function ends here */

        // writing list to headerfile
        *header << result.str();

        // save list in signature hash map
        lists[type] = name;

        // return name of list generated
        return name;
    }

    string CCodeGenerator::generate_signature(Type &type) {
        // result is needed, so we don't start generating something in a signature in the headerfile
        stringstream result;
        string name = g_generated + g_signature;
        name += to_string(sig_count);

        // increase signature count, so next signature doesn't have the same name
        sig_count++;

        type.types.back()->accept(*this);
        result << "typedef " << last_type << " (* " << name << ")(";

        for (size_t i = 0; i < type.types.size() - 1; ++i) {
            type.types[i]->accept(*this); // generate the actual type of the argument
            result << last_type;

            if (i < type.types.size() - 2) // don't print ", " after the last argument
                result << ", ";
        }

        result << ");" << endl;
        result << endl;

        // writing signature to headerfile
        *header << result.str();

        // save signature in signature hash map
        signatures[type] = name;

        // return name of signature generated
        return name;
    }

    string CCodeGenerator::generate_tuple(Type &type) {
        // result is needed, so we don't start generating something in a tuple in the headerfile
        stringstream result;
        string name = g_generated + g_tuple;
        name += to_string(tuple_count);

        // increase tuple count, so next tuple doesn't have the same name
        tuple_count++;

        /* generation of tuple starts here */
        result << "typedef struct " << name << " {" << endl;
        // generate an item for each type in the tuple
        for (size_t i = 0; i < type.types.size(); ++i) {
            type.types[i]->accept(*this); // generate the actual type of the item

            result << "    " << last_type << " " << g_generated << g_item << i << ";" << endl; // give this item a unique name

        }
        result << "} " << name << ";" << endl;
        result << endl;
        /* generation of tuple ends here */

        /* generation of tuple contructor starts here */
        // give contructor a unique name
        result << name << g_generated << g_create << name << "(";

        // generate an argument for each item in the struct
        for (size_t i = 0; i < type.types.size(); ++i) {
            type.types[i]->accept(*this); // generate the actual type of the argument
            result << last_type << " " << g_generated << g_item << i;

            if (i < type.types.size() - 1) // don't print ", " after the last argument
                result << ", ";
        }

        result << ") {" << endl;

        // generate a result variable
        result << "    " << name << " " << "res;";

        // for each item in res, assign values
        for (size_t i = 0; i < type.types.size(); ++i) {
            result << "    res." << g_generated << g_item << i << " = " << g_generated << g_item << i << ";" << endl;
        }
        result << " \n"
                  "    return res; \n"
                  "} \n"
                  " \n";
        /* generation of tuple contructor ends here */

        /* generation of compare function starts here */
        result << "int " << g_generated << g_compare << name << "(" << name << " tuple1, " << name << " tuple2) { \n";

        for (size_t i = 0; i < type.types.size(); ++i) {
            result << "    if (";

            switch (type.types[i]->type) {
                case Types::STRING:
                case Types::LIST:
                    result << "!" << g_generated << g_compare << lists[*type.types[i]] << "(tuple1." << g_generated << g_item << i << ", tuple2." << g_generated << g_item << i << ")";
                    break;
                case Types::TUPLE:
                    result << "!" << g_generated << g_compare << tuples[*type.types[i]] << "(tuple1." << g_generated << g_item << i << ", tuple2." << g_generated << g_item << i << ")";
                    break;
                default:
                    result << "(tuple1." << g_generated << g_item << i << " != tuple2." << g_generated << g_item << i << ")";
                    break;
            }

            result << ") \n"
                      "        return 0; \n";
        }

        result << " \n"
                  "    return 1; \n"
                  "} \n"
                  "\n";
        /* generation of compare function ends here */

        // writing tuple to headerfile
        *header << result.str();

        // save tuple in tuple hash map
        tuples[type] = name;

        // return name of tuple generated
        return name;
    }

    void CCodeGenerator::generate_std() {

        *header << "#include <stdarg.h> \n"
                  "#include <stdio.h> \n"
                  "#include <stdlib.h> \n"
                  "#include <string.h> \n"
                  " \n";

        /* generating to nearest power of 2 function starts here */
        *header << "int " << g_generated << g_nearpow2 << "(int num) { \n"
                  "    num--; \n"
                  "    num |= num >> 1; \n"
                  "    num |= num >> 2; \n"
                  "    num |= num >> 4; \n"
                  "    num |= num >> 8; \n"
                  "    num |= num >> 16; \n"
                  "    num ++; \n"
                  " \n"
                  "    return num + (num == 0); \n"
                  "} \n"
                  " \n";
        /* generating to nearest power of 2 function ends here */

        string_type_name = generate_list(*real_string);
        lists[*fake_string] = string_type_name;
        generate_list(*string_list);

        /* generation of string constructer starts here */
        *header << string_type_name << "* " << g_generated << g_create << g_string << "(char* values) { \n"
                  "    int i, str_length = strlen(values), size = " << g_generated << g_nearpow2 << "(str_length); \n"
                  "    " << string_type_name << "* res = malloc(sizeof(" << string_type_name << ")); \n"
                  " \n"
                  "    res->" << g_head << " = 0; \n"
                  "    res->" << g_size << " = size; \n"
                  "    res->" << g_items << " = malloc(size * sizeof(" << g_char << ")); \n"
                  " \n"
                  "    for (i = str_length - 1; i >= 0; i--) { \n"
                  "        " << g_generated << g_push << string_type_name << "(res, values[i]); \n"
                  "    } \n"
                  " \n"
                  "    return res; \n"
                  "} \n"
                  " \n";
        /* generation of string constructer ends here */

        /* generation of string compare here */
        *header << "int " << g_generated << g_compare << g_string << "(" << string_type_name << "* string, char* values, int offset) { \n"
                  "    int i, j, size = strlen(values); \n"
                  " \n"
                  "    if (size == string->" << g_head << " - offset) { \n"
                  "        for (i = 0, j = size - 1; i < size; i++, j--) { \n"
                  "            if (string->" << g_items << "[j] != values[i]) \n"
                  "                return 0; \n"
                  "        } \n"
                  "    } else { \n"
                  "        return 0; \n"
                  "    } \n"
                  " \n"
                  "    return 1; \n"
                  "} \n"
                  " \n";
        /* generation of string compare ends here */

    }
}
