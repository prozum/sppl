#include "CppCodeGenerator.h"
#include <string>
#include <iostream>
#include <sstream>
#include <Scope.h>

using namespace common;
using namespace std;

namespace codegen
{
#define res_endl tap_endl(tap_count)
#define out_endl tap_endl(output_tap_count)
#define head_endl tap_endl(header_tap_count)

    CCodeGenerator::CCodeGenerator(std::ostream &out, std::ostream &head)
            : CodeGenerator::CodeGenerator(out), header(head)
    {
        _char = Type(Types::CHAR);
        real_string = Type(Types::LIST);
        fake_string = Type(Types::STRING);
        string_list = Type(Types::LIST);
        real_string.types.push_back(&_char);
        string_list.types.push_back(&fake_string);
        list_offsets.push_back(0);
    }

    void CCodeGenerator::visit(Program &node)
    {
        generate_std();

        // TODO real header
        output << "#include \"test.h\""                                                                     << out_endl;
        output                                                                                              << out_endl;

        // generate c main, that calls umain
        output << "int main(int argc, char** argv)"                                                         << out_endl; output_tap_count++;
        output << "{"                                                                                       << out_endl;
        output <<      lists[string_list] << " *args = gcreate_" << lists[string_list] << "(0);"            << out_endl;
        output <<     "int i;"                                                                              << out_endl;
        output                                                                                              << out_endl; output_tap_count++;
        output <<     "for(i = argc - 1; i >= 0; i--) {"                                                    << out_endl;
        output <<         "gpush_" << lists[string_list] << "(args, gcreate_string(argv[i]));"              << out_endl; output_tap_count--;
        output <<     "}"                                                                                   << out_endl;
        output                                                                                              << out_endl;
        output <<     "printf(\"%d\\n\", umain(args));"                                                     << out_endl;
        output <<     "return 0;"                                                                           << out_endl; output_tap_count--;
        output << "}"                                                                                       << out_endl;
        output                                                                                              << out_endl;

        for (auto f : node.funcs) {
            f->accept(*this);
            output << out_endl;
        }
    }

    void CCodeGenerator::visit(Function &node)
    {
        stringstream function;
        stringstream arg_name;

        node.types.back()->accept(*this);

        // generate function name and return type
        function << last_type << " u" << node.id << "(";

        // generate function arguments
        for (size_t i = 0; i < node.types.size() - 1; i++) {
            node.types[i]->accept(*this);
            arg_name << "ga" << i;
            arg_names.push_back(arg_name.str());
            function << last_type << " " << arg_name.str();

            if (i < node.types.size() - 2) // don't generate ", " after last argument
                function << ", ";

            arg_name.str("");
        }

        function << ")";

        // generate function decleration in header
        header << function.str() << ";" << head_endl;
        header << head_endl;

        // generate function in output
        output << function.str()                         << out_endl; output_tap_count++;
        output << "{"                                    << out_endl;

        // generate cases
        for (auto c : node.cases) {
            c->accept(*this);
            output << out_endl;

            // clear assigments specific for current case
            assignments.clear();
        }

        // generate error, for when program doesn't realize a case
        output                                              << out_endl;
        output <<     "printf(\"No cases realized!\\n\");"  << out_endl;
        output <<     "exit(1);"                            << out_endl; output_tap_count--;
        output << "}"                                       << out_endl;
        output                                              << out_endl;

        // clear arg_names for current function
        arg_names.clear();
    }

    void CCodeGenerator::visit(Case &node)
    {
        stringstream pattern;
        bool empty = true;

        // generate if-statement for matching
        pattern << "if (";

        id_context = IdContext::PATTERN;
        for (size_t i = 0; i < node.patterns.size(); i++) {
            // push arg_name on arg_name_stack. arg_name_stack is used generate
            // assignments in a case
            arg_name_stack.push_back(arg_names[i]);

            // generate pattern
            node.patterns[i]->accept(*this);

            // cleanup
            arg_name_stack.pop_back();

            // only add pattern, if pattern is not "1"
            if (last_pattern != "1") {
                empty = false;
                pattern << last_pattern;

                if (i < node.patterns.size() - 1 && last_pattern != "1")
                    pattern << " && ";
            }
        }

        pattern << ")" << out_endl; output_tap_count++;

        // only generate if statement, if the pattern wasn't empty
        if (!empty) {
            output << pattern.str() << " {" << out_endl;
        }

        // generate all nessesary assigments
        for (auto assign : assignments) {
            output << assign << out_endl;
        }

        output << out_endl;

        // generate return expression
        id_context = IdContext::EXPR;
        output<< "return ";
        node.expr->accept(*this);
        output << ";" << out_endl; output_tap_count--;

        if (!empty) {
            output << "}" << out_endl;
            output << out_endl;
        }
    }

    void CCodeGenerator::visit(Or &node)
    {
        output << "(";
        node.left->accept(*this);
        output << "||";
        node.right->accept(*this);
        output << ")";
    }

    void CCodeGenerator::visit(And &node)
    {
        output << "(";
        node.left->accept(*this);
        output << "&&";
        node.right->accept(*this);
        output << ")";
    }

    void CCodeGenerator::visit(Equal &node)
    {
        // Todo Compare strings and lists and tuples
        output << "(";
        node.left->accept(*this);
        output << "==";
        node.right->accept(*this);
        output << ")";
    }

    void CCodeGenerator::visit(NotEqual &node)
    {
        // Todo Compare strings and lists and tuples
        output << "(";
        node.left->accept(*this);
        output << "!=";
        node.right->accept(*this);
        output << ")";
    }

    void CCodeGenerator::visit(Lesser &node)
    {
        output << "(";
        node.left->accept(*this);
        output << "<";
        node.right->accept(*this);
        output << ")";
    }

    void CCodeGenerator::visit(LesserEq &node)
    {
        output << "(";
        node.left->accept(*this);
        output << "<=";
        node.right->accept(*this);
        output << ")";
    }

    void CCodeGenerator::visit(Greater &node)
    {
        output << "(";
        node.left->accept(*this);
        output << ">";
        node.right->accept(*this);
        output << ")";
    }

    void CCodeGenerator::visit(GreaterEq &node)
    {
        output << "(";
        node.left->accept(*this);
        output << ">=";
        node.right->accept(*this);
        output << ")";
    }

    void CCodeGenerator::visit(Add &node)
    {
        output << "(";
        node.left->accept(*this);
        output << "+";
        node.right->accept(*this);
        output << ")";
    }

    void CCodeGenerator::visit(Sub &node)
    {
        output << "(";
        node.left->accept(*this);
        output << "-";
        node.right->accept(*this);
        output << ")";
    }

    void CCodeGenerator::visit(Mul &node)
    {
        output << "(";
        node.left->accept(*this);
        output << "*";
        node.right->accept(*this);
        output << ")";
    }

    void CCodeGenerator::visit(Div &node)
    {
        output << "(";
        node.left->accept(*this);
        output << "/";
        node.right->accept(*this);
        output << ")";
    }

    void CCodeGenerator::visit(Mod &node)
    {
        output << "(";
        node.left->accept(*this);
        output << "%";
        node.right->accept(*this);
        output << ")";
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

        // use pregenerated gpush_"name" to push thing onto list
        // look at generate_list(Type &type) for the generation of this function
        output << "gpush_" << name << "(";
        node.right->accept(*this);
        output << ", ";
        node.left->accept(*this);
        output << ")";
    }

    void CCodeGenerator::visit(Par &node)
    {
        output << "(";
        node.child->accept(*this);
        output << ")";
    }

    void CCodeGenerator::visit(Not &node)
    {
        output << "(";
        output << "!";
        node.child->accept(*this);
        output << ")";
    }

    void CCodeGenerator::visit(ListPattern &node)
    {
        // result is needed, so we don't generate something else, while the ListPattern is being generated
        stringstream result;
        string arg_name = "";
        bool empty = true;

        // visit nodes type, to ensure that a list of this type has be generated
        node.node_type->accept(*this);

        // generate full argument from arg_name_stack
        for (auto str : arg_name_stack) {
            arg_name += str;
        }

        result << arg_name << "->head - " << list_offsets.back() << " == " << node.patterns.size();

        for (size_t i = 0; i < node.patterns.size(); i++) {
            // add "gat_"name"(" and ", i + offset)" to arg_name_stack
            arg_name_stack.insert(arg_name_stack.begin(), "gat_" + lists[*node.node_type] + "(");
            arg_name_stack.push_back(", " + to_string(i + list_offsets.back()) + ")");

            // push new offset.
            // lists accesed later down in pattern should not be offseted by the same as current list.
            list_offsets.push_back(0);

            // generate pattern
            node.patterns[i]->accept(*this);

            // cleanup
            list_offsets.pop_back();
            arg_name_stack.pop_back();
            arg_name_stack.erase(arg_name_stack.begin());

            // don't add pattern, if pattern is "1"
            if (last_pattern != "1") {
                empty = false;
                result << " && " << last_pattern;
            }
        }

        // if empty, then let last_pattern be "1"
        if (empty) {
            last_pattern = "1";
        } else {
            last_pattern = "(" + result.str() + ")";
        }
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
            // add ".gi"i"" to arg_name_stack
            arg_name_stack.push_back(".gi" + to_string(i));

            // generate pattern
            node.patterns[i]->accept(*this);

            // cleanup
            arg_name_stack.pop_back();

            // don't add pattern, if pattern is "1"
            if (last_pattern != "1") {
                empty = false;
                result << last_pattern;

                if (i < node.patterns.size() - 1)
                    result << " && ";
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

        // add "gat_"name"(" and ", offset)" to arg_name_stack
        // this is done, so that patterns on the left of node, will use
        // the first + offset item in the list
        arg_name_stack.insert(arg_name_stack.begin(), "gat_" + lists[*node.node_type] + "(");
        arg_name_stack.push_back(", " + to_string(list_offsets.back()) + ")");

        // push new offset.
        // lists accesed later down in pattern should not be offseted by the same as current list.
        list_offsets.push_back(0);

        // generate pattern
        node.left->accept(*this);

        // cleanup
        list_offsets.pop_back();
        arg_name_stack.pop_back();
        arg_name_stack.erase(arg_name_stack.begin());

        // don't add pattern, if pattern is "1"
        if (last_pattern != "1") {
            empty = false;
            result << last_pattern << " && ";
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
            string arg = "";

            for (auto str : arg_name_stack) {
                arg += str;
            }

            last_pattern = arg + " == " + to_string(node.value);

        // else, just output value
        } else {
            output << node.value;
        }
    }

    void CCodeGenerator::visit(Float &node)
    {
        // if pattern, then generate an expression for matching on this pattern
        if (id_context == IdContext::PATTERN) {
            string arg = "";

            for (auto str : arg_name_stack) {
                arg += str;
            }

            last_pattern = arg + " == " + to_string(node.value);

        // else, just output value
        } else {
            output << node.value;
        }

    }

    void CCodeGenerator::visit(Bool &node)
    {
        // if pattern, then generate an expression for matching on this pattern
        if (id_context == IdContext::PATTERN) {
            string arg = "";

            for (auto str : arg_name_stack) {
                arg += str;
            }

            last_pattern = arg + " == " + to_string(node.value);

        // else, just output value
        } else {
            output << node.value;
        }
    }

    void CCodeGenerator::visit(Char &node)
    {
        // if pattern, then generate an expression for matching on this pattern
        if (id_context == IdContext::PATTERN) {
            string arg = "";

            for (auto str : arg_name_stack) {
                arg += str;
            }

            last_pattern = arg + " == '" + node.value + "'";

        // else, just output value
        } else {
            output << "'" << node.value << "'";
        }
    }

    void CCodeGenerator::visit(String &node)
    {
        // if pattern, then generate an expression for matching on this pattern
        if (id_context == IdContext::PATTERN) {
            string arg = "";

            for (auto str : arg_name_stack) {
                arg += str;
            }

            // gcompare_string is generated by generate_std. it compares the custome string type, to a char*
            // it also takes an offset, for when list splits occur on strings
            last_pattern = "gcompare_string(" + arg + ", \"" + node.value + "\", " + to_string(list_offsets.back()) + ")";

        // else, just output value
        } else {
            // gcreate_string is generate by generate_std. It creates string base on a char*
            output << "gcreate_string(\"" << node.value << "\")";
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
        output << "gcreate_" << name << "(" << node.exprs.size() << ", ";
        for (int i = node.exprs.size() - 1; i >= 0; i--) {
            node.exprs[i]->accept(*this);

            if (i != 0)
                output << ", ";
        }

        output << ")";
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
        output << "gcreate_" << name << "(";
        for (auto expr : node.exprs){
            expr->accept(*this);

            if (expr != node.exprs.back())
                output << ", ";
        }
        output << ")";
    }

    void CCodeGenerator::visit(Id &node)
    {
        stringstream assign;
        string name = "";

        if (id_context == IdContext::PATTERN) {
            // created name from arg_name_stack
            for (auto str : arg_name_stack) {
                name += str;
            }

            // get id's type
            node.node_type->accept(*this);

            // generate an assignment for the id, which sould occur after the if-statment
            assign << last_type << " u" << node.id << " = ";

            // if id is of a type that is allocated on the heap (aka strings and lists)
            // then make a copy of it, since the language should have no sideeffects
            // there could also be an offset, the lists and string should be copied with
            if (node.node_type->type == Types::LIST || node.node_type->type == Types::STRING) {
                assign << "gclone_" << lists[*node.node_type] << "(" << name << ", " << list_offsets.back() << ");";
            } else {
                assign << name << ";";
            }

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
                    assign << "gclone_" << lists[*node.node_type] << "(u" << node.id << ", 0);";
                    break;
                default:
                    output << "u" << node.id;
                    break;
            }
        }
    }

    void CCodeGenerator::visit(Call &node)
    {
        output << "(";
        // generate the callee (aka, the function being called)
        node.callee->accept(*this);

        // generate the arguments the function is being called with
        output << "(";
        for (auto expr : node.exprs){
            expr->accept(*this);

            if (expr != node.exprs.back())
                output << ", ";
        }
        output << ")";

        output << ")";
    }

    void CCodeGenerator::visit(Type &node) {
        unordered_map<Type, string>::iterator got;

        // generate the currect c type, based on sppl's types
        switch (node.type) {
            case Types::FLOAT:
                last_type = "double";
                break;
            case Types::CHAR:
                last_type = "int";
                break;
            case Types::INT:
                last_type = "int";
                break;
            case Types::BOOL:
                last_type = "int";
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
                    last_type = generate_signature(node);
                }
                break;
            case Types::STRING:
                // the string type is generate at the start of the generation.
                // we therefor just use the name that this type was give.
                last_type = string_type_name;

                // strings are allocated on the heap, and we therefor need a pointer to it.
                last_type += " *";
                break;
            case Types::LIST:
                got = lists.find(node);

                if (got != lists.end()) {
                    last_type = got->second;
                } else {
                    last_type = generate_list(node);
                }
                last_type += " *";
                break;
            default:
                break;
        }
    }

    string CCodeGenerator::generate_list(Type &type) {
        // result is needed, so we don't start generating something in a signature in the header file
        stringstream result;
        string name = "gl";
        int tap_count = 0;
        name += to_string(list_count);

        // increase list count, so next list doesn't have the same name
        list_count++;

        type.types.front()->accept(*this);

        /* generation of list starts here */
        result << "typedef struct " << name << res_endl;tap_count++;
        result << "{" << res_endl;
        result << last_type << " *items;" << res_endl;
        result << "int head;" << res_endl;
        result << "int size;" << res_endl; tap_count--;
        result << "}" << name << ";" << res_endl;
        result << res_endl;
        /* generation of list ends here */

        /* generation of list push starts here */
        result << name << " *gpush_" << name << "(" << name << " *this, " << last_type << " item)" << res_endl; tap_count++;
        result << "{" << res_endl;
        result << "if (this->head >= this->size)" << res_endl; tap_count++;
        result << "{" << res_endl;
        result << "this->items = (" << last_type << " *)realloc(this->items, (this->size *= 2) * sizeof(" << last_type << "));" << res_endl; tap_count--;
        result << "}" << res_endl;
        result << res_endl;
        result << "this->items[this->head++] = item;" << res_endl;
        result << "return this;" << res_endl; tap_count--;
        result << "}" << res_endl;
        result << res_endl;
        /* generation of list push ends here */

        /* generation of list constructer starts here */
        result << name << " *gcreate_" << name << "(int count, ...)" << res_endl; tap_count++;
        result << "{" << res_endl;
        result << "int i;" << res_endl;
        result << "int size = gnearest_pow2(count);" << res_endl;
        result << "" << name << " *res = (" << name << "*)malloc(sizeof(" << name << "));" << res_endl;
        result << res_endl;
        result << "res->head = 0;" << res_endl;
        result << "res->size = size;" << res_endl;
        result << "res->items = (" << last_type << " *)malloc(size * sizeof(" << last_type << "));" << res_endl;
        result << res_endl;
        result << "va_list args;" << res_endl;
        result << "va_start(args, count);" << res_endl;
        result << res_endl;
        result << "for(i = 0; i < count; i++)" << res_endl; tap_count++;
        result << "{" << res_endl;
        result << "gpush_" << name << "(res, va_arg(args, " << last_type << "));" << res_endl; tap_count--;
        result << "}" << res_endl;
        result << res_endl;
        result << "va_end(args);" << res_endl;
        result << "return res;" << res_endl; tap_count--;
        result << "}" << res_endl;
        result << res_endl;
        /* generation of list constructer ends here */

        /* generation of at function starts here */
        result << last_type << " gat_" << name << "(" << name << " *this, int index)" << res_endl; tap_count++;
        result << "{" << res_endl;
        result << "return this->items[this->head - (1 + index)];" << res_endl; tap_count--;
        result << "}" << res_endl;
        result << res_endl;
        /* generation of at function ends here */

        /* generation of list constructer starts here */
        result << name << " *gclone_" << name << "(" << name << " *list, int offset)" << res_endl; tap_count++;
        result << "{" << res_endl;
        result << "int i;" << res_endl;
        result << name << " *res = (" << name << "*)malloc(sizeof(" << name << "));" << res_endl;
        result << res_endl;
        result << "res->head = list->head - offset;" << res_endl;
        result << "res->size = list->size;" << res_endl;
        result << "res->items = (" << last_type << " *)malloc(list->size * sizeof(" << last_type << "));" << res_endl;
        result << res_endl; tap_count++;
        result << "for (i = 0; i < res->head; i++) {" << res_endl;
        result << "res->items[i] = list->items[i];" << res_endl; tap_count--;
        result << "}" << res_endl;
        result << "return res;" << res_endl;
        result << res_endl; tap_count--;
        result << "}" << res_endl;
        result << res_endl;
        /* generation of list constructer ends here */

        /* generation of compare function starts here */
        result << "int gcompare_" << name << "(" << last_type << " *list1, " << last_type << "* list2)" << res_endl; tap_count++;
        result << "{" << res_endl;
        result << "int i;" << res_endl; tap_count++;
        result << "if (list1->head != list2->head) {" << res_endl;
        result << "return 0;" << res_endl; tap_count--;
        result << "}" << res_endl;
        result << res_endl; tap_count++;
        result << "for (i = 0; i < list1->head; i++) {" << res_endl; tap_count++;
        result << "if ("; // Todo compare

        /*
        switch (type.types[0]->type) {
            case Types::LIST:
                result << "!gcompare_" << lists[type.types[0]] << "(list1->items[i], list2->items[i])";
                break;
            case Types::TUPLE:
                result << "!gcompare_" << tuples[type.types[0]] << "(list1->items[i], list2->items[i])";
                break;
            default:
                result << "list2->items[i] != list2->items[i]";
                break;
        }
         */

        result << ")" << res_endl;
        result << "return 0;"; tap_count--; tap_count--;
        result << "}" << res_endl; tap_count--;
        result << res_endl;
        result << "return 1;" << res_endl; tap_count--;
        result << "}" << res_endl;
        result << res_endl;
        /* generation of compare function ends here */

        /* TODO */

        // writing list to headerfile
        header<< result.str();

        // save list in signature hash map
        lists[type] = name;

        // return name of list generated
        return name;
    }

    string CCodeGenerator::generate_signature(Type &type) {
        // result is needed, so we don't start generating something in a signature in the headerfile
        stringstream result;
        string name = "gs";
        int tap_count = 0;
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

        result << ");" << res_endl;
        result << res_endl;

        // writing signature to headerfile
        header<< result.str();

        // save signature in signature hash map
        signatures[type] = name;

        // return name of signature generated
        return name;
    }

    string CCodeGenerator::generate_tuple(Type &type) {
        // result is needed, so we don't start generating something in a tuple in the headerfile
        stringstream result;
        string name = "gt";
        int tap_count = 0;
        name += to_string(tuple_count);

        // increase tuple count, so next tuple doesn't have the same name
        tuple_count++;

        /* generation of tuple starts here */
        result << "typedef struct " << name << res_endl; tap_count++;
        result << "{" << res_endl;
        // generate an item for each type in the tuple
        for (size_t i = 0; i < type.types.size(); ++i) {
            type.types[i]->accept(*this); // generate the actual type of the item
            result << last_type << " gi" << i << ";" << endl; // give this item a unique name
        }
        tap_count--;
        result << "}" << name << ";" << res_endl;
        result << res_endl;
        /* generation of tuple ends here */

        /* generation of tuple contructor starts here */
        // give contructor a unique name
        result << name << " gcreate_" << name << "(";

        // generate an argument for each item in the struct
        for (size_t i = 0; i < type.types.size(); ++i) {
            type.types[i]->accept(*this); // generate the actual type of the argument
            result << last_type << " gi" << i;

            if (i < type.types.size() - 1) // don't print ", " after the last argument
                result << ", ";
        }

        result << ")" << res_endl; tap_count++;
        result << "{" << res_endl;

        // generate a result variable
        result << "" << name << " " << "res;" << res_endl;

        // for each item in res, assign values
        for (size_t i = 0; i < type.types.size(); ++i) {
            result << "res.gi" << i << " = gi" << i << ";" << res_endl;
        }
        result << "return res;" << res_endl; tap_count--;
        result << "}" << res_endl;
        result << res_endl;
        /* generation of tuple contructor ends here */

        /* generation of compare function starts here */
        result << "int gcompare_" << name << "(" << name << " *list1, " << name << "* list2)" << res_endl; tap_count++;
        result << "{" << res_endl;
        result << "int i;" << res_endl;
        result << "if (list1->head != list2->head)" << res_endl; tap_count++;
        result << "{" << res_endl;
        result << "return 0;" << res_endl; tap_count--;
        result << "}" << res_endl;
        result << res_endl; tap_count++;
        result << "for (i = 0; i < list1->head; i++) {" << res_endl; tap_count++;
        result << "if ("; // Todo compare

        /*
        switch (type.types[0]->type) {
            case Types::LIST:
                result << "!gcompare_" << lists[type.types[0]] << "(list1->items[i], list2->items[i])";
                break;
            case Types::TUPLE:
                result << "!gcompare_" << tuples[type.types[0]] << "(list1->items[i], list2->items[i])";
                break;
            default:
                result << "list2->items[i] != list2->items[i]";
                break;
        }
         */

        result << ")" << res_endl;
        result << "return 0;" << res_endl; tap_count--; tap_count--;
        result << "}" << res_endl;
        result << res_endl;
        result << "return 1;" << res_endl; tap_count--;
        result << "}" << res_endl;
        result << res_endl;
        /* generation of compare function ends here */

        // writing tuple to headerfile
        header<< result.str();

        // save tuple in tuple hash map
        tuples[type] = name;

        // return name of tuple generated
        return name;
    }

    void CCodeGenerator::generate_std() {

        header << "#include <stdarg.h>" << head_endl;
        header << "#include <stdio.h>" << head_endl;
        header << "#include <stdlib.h>" << head_endl;
        header << "#include <string.h>" << head_endl;
        header << head_endl;

        /* generating to nearest power of 2 function starts here */
        header << "int gnearest_pow2(int gn)" << head_endl; header_tap_count++;
        header << "{" << head_endl;
        header << "gn--;" << head_endl;
        header << "gn |= gn >> 1;" << head_endl;
        header << "gn |= gn >> 2;" << head_endl;
        header << "gn |= gn >> 4;" << head_endl;
        header << "gn |= gn >> 8;" << head_endl;
        header << "gn |= gn >> 16;" << head_endl;
        header << "gn++;" << head_endl;
        header << head_endl;
        header << "return gn + (gn == 0);" << head_endl; header_tap_count--;
        header << "}" << head_endl;
        header << head_endl;
        /* generating to nearest power of 2 function ends here */

        string_type_name = generate_list(real_string);
        lists[fake_string] = string_type_name;
        generate_list(string_list);

        /* generation of string constructer starts here */
        header << string_type_name << " *gcreate_string(char* values)" << head_endl; header_tap_count++;
        header << "{" << head_endl;
        header << "int i;" << head_endl;
        header << "int str_length = strlen(values);" << head_endl;
        header << "int size = gnearest_pow2(str_length);" << head_endl;
        header << string_type_name << " *res = (" << string_type_name << "*)malloc(sizeof(" << string_type_name << "));" << head_endl;
        header << head_endl;
        header << "res->head = 0;" << head_endl;
        header << "res->size = size;" << head_endl;
        header << "res->items = (int *)malloc(size * sizeof(int));" << head_endl;
        header << head_endl;
        header << "for(i = str_length - 1; i >= 0; i--)" << head_endl;
        header << "{" << head_endl; header_tap_count++;
        header << "gpush_" << string_type_name << "(res, values[i]);" << head_endl; header_tap_count--;
        header << "}" << head_endl;
        header << head_endl;
        header << "return res;" << head_endl;
        header_tap_count--;
        header << "}" << head_endl;
        header << head_endl;
        header << head_endl;
        /* generation of string constructer ends here */

        /* generation of string compare here */
        header << "int gcompare_string(" << string_type_name << "*string, char* values, int offset)" << head_endl; header_tap_count++;
        header << "{" << head_endl;
        header << "int i, j;" << head_endl;
        header << "int size = strlen(values);" << head_endl;
        header << head_endl;
        header << "if (size == string->head - offset)" << head_endl; header_tap_count++;
        header << "{" << head_endl;
        header << "for (i = 0, j = size - 1; i < size; i++, j--)" << head_endl; header_tap_count++;
        header << "{" << head_endl; header_tap_count++;
        header << "if (string->items[j] != values[i])" << head_endl;
        header << "return 0;" << head_endl; header_tap_count--; header_tap_count--;
        header << "}" << head_endl; header_tap_count--;
        header << "}" << head_endl;
        header << "else" << head_endl; header_tap_count++;
        header << "{" << head_endl;
        header << "return 0;" << head_endl;  header_tap_count--;
        header << "}" << head_endl;
        header << head_endl;
        header << "return 1;" << head_endl; header_tap_count--;
        header << "}" << head_endl;
        header << head_endl;
        /* generation of string compare ends here */

    }

    string CCodeGenerator::tap_endl(int i) {
        string res = "\n";

        for (int j = 0; j < i; j++) {
            res += "    ";
        }

        return res;
    }
}
