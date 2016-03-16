#include "CppCodeGenerator.h"
#include <string>
#include <iostream>
#include <sstream>
#include <Scope.h>

using namespace common;
using namespace std;

namespace codegen
{
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
        stringstream func_name;

        generate_std();

        // TODO real header
        outputt() << "#include \"test.h\"" << endl;
        outputt() << endl;
        outputt() << "int main(int argc, char** argv)" << endl;
        outputt() << "{" << endl;
        output_tap_count++;
        outputt() << lists[string_list] << " *args = gcreate_" << lists[string_list] << "(0);" << endl;
        outputt() << "int i;" << endl;
        outputt() << "for(i = 0; i < argc; i++)" << endl;
        outputt() << "{" << endl;
        output_tap_count++;
        outputt() << "gpush_" << lists[string_list] << "(args, gcreate_string(argv[i]));" << endl;
        output_tap_count--;
        outputt() << "}" << endl;
        outputt() << endl;
        outputt() << "printf(\"%d\\n\", umain(args));" << endl;
        outputt() << "return 0;" << endl;
        output_tap_count--;
        outputt() << "}" << endl;
        outputt() << endl;

        for (auto f : node.funcs) {
            // save a unique name for each function the program
            func_name << "u" << f->id;
            f->scope->real_ids[f->id] = func_name.str();
            func_name.str("");
        }

        for (auto f : node.funcs) {
            f->accept(*this);
            outputt() << endl;
        }
    }

    void CCodeGenerator::visit(Function &node)
    {
        stringstream func_name;
        stringstream arg_name;

        // get the functions unique name
        func_name << node.scope->get_real_id(node.id) << "(";

        // generate function arguments
        for (size_t i = 0; i < node.types.size() - 1; i++) {
            node.types[i]->accept(*this);
            arg_name << "ga" << i;
            arg_names.push_back(arg_name.str());
            func_name << last_type << " " << arg_name.str();

            if (i < node.types.size() - 2) // don't generate ", " after last argument
                func_name<< ", ";

            arg_name.str("");
        }

        func_name << ")";

        node.types.back()->accept(*this);

        // generate function decleration in header
        headert() << last_type << " " << func_name.str() << ";" << endl;
        headert() << endl;

        // generate function in output
        outputt()<< last_type << " " << func_name.str() << endl;

        outputt()<< "{" << endl;
        output_tap_count++;

        // generate cases
        for (auto c : node.cases) {
            c->accept(*this);
            outputt() << endl;
            assignments.clear();
        }

        outputt() << endl;
        outputt() << "printf(\"No cases realized!\\n\");" << endl;
        outputt() << "exit(1);" << endl;
        output_tap_count--;
        outputt() << "}" << endl;
        outputt() << endl;
        arg_names.clear();
    }

    void CCodeGenerator::visit(Case &node)
    {
        if (node.patterns.size() != 0) {
            // generate if-statement for matching
            outputt() << "if (";

            id_context = IdContext::PATTERN;
            for (size_t i = 0; i < node.patterns.size(); i++) {
                arg_name_stack.push_back(arg_names[i]);
                node.patterns[i]->accept(*this);
                arg_name_stack.pop_back();
                output << last_pattern;

                if (i < node.patterns.size() - 1)
                    output << " && ";
            }

            output << ")" << endl;
        }

        outputt()<< "{" << endl;
        output_tap_count++;

        // generate return expression
        id_context = IdContext::EXPR;
        for (auto assign : assignments) {
            outputt() << assign << endl;
        }
        outputt()<< "return ";
        node.expr->accept(*this);
        output << ";" << endl;

        output_tap_count--;
        outputt() << "}" << endl;
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
        output << "(";
        node.left->accept(*this);
        output << "==";
        node.right->accept(*this);
        output << ")";
    }

    void CCodeGenerator::visit(NotEqual &node)
    {
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
        // result is needed, so we don't start generating something in a signature in the headert() file
        stringstream result;
        string arg_name = "";

        node.node_type->accept(*this);

        for (auto str : arg_name_stack) {
            arg_name += str;
        }

        result << arg_name << "->head - " << list_offsets.back() << " == " << node.patterns.size();

        for (size_t i = 0; i < node.patterns.size(); i++) {
            arg_name_stack.insert(arg_name_stack.begin(), "gat_" + lists[*node.node_type] + "(");
            arg_name_stack.push_back(", " + to_string(i + list_offsets.back()) + ")");
            list_offsets.push_back(0);
            node.patterns[i]->accept(*this);
            list_offsets.pop_back();
            arg_name_stack.pop_back();
            arg_name_stack.erase(arg_name_stack.begin());
            result << " && " << last_pattern;
        }

        last_pattern = "(" + result.str() + ")";
    }

    void CCodeGenerator::visit(TuplePattern &node)
    {
        // result is needed, so we don't start generating something in a signature in the headert() file
        stringstream result;

        node.node_type->accept(*this);

        for (size_t i = 0; i < node.patterns.size(); i++) {
            arg_name_stack.push_back(".gi" + to_string(i));
            node.patterns[i]->accept(*this);
            arg_name_stack.pop_back();
            result << last_pattern;

            if (i < node.patterns.size() - 1)
                result << " && ";
        }

        last_pattern = "(" + result.str() + ")";
    }

    void CCodeGenerator::visit(ListSplit &node)
    {
        stringstream result;

        arg_name_stack.insert(arg_name_stack.begin(), "gat_" + lists[*node.node_type] + "(");
        arg_name_stack.push_back(", " + to_string(list_offsets.back()) + ")");
        list_offsets.push_back(0);
        node.left->accept(*this);
        list_offsets.pop_back();
        arg_name_stack.pop_back();
        arg_name_stack.erase(arg_name_stack.begin());

        result << "(" << last_pattern << " && ";

        list_offsets[list_offsets.size() - 1]++;
        node.right->accept(*this);
        list_offsets[list_offsets.size() - 1]--;

        result << last_pattern << ")";

        last_pattern = result.str();
    }

    void CCodeGenerator::visit(Int &node)
    {
        if (id_context == IdContext::PATTERN) {
            string arg = "";

            for (auto str : arg_name_stack) {
                arg += str;
            }

            last_pattern = arg + " == " + to_string(node.value);
        } else {
            output << node.value;
        }
    }

    void CCodeGenerator::visit(Float &node)
    {
        if (id_context == IdContext::PATTERN) {
            string arg = "";

            for (auto str : arg_name_stack) {
                arg += str;
            }

            last_pattern = arg + " == " + to_string(node.value);
        } else {
            output << node.value;
        }

    }

    void CCodeGenerator::visit(Bool &node)
    {
        if (id_context == IdContext::PATTERN) {
            string arg = "";

            for (auto str : arg_name_stack) {
                arg += str;
            }

            last_pattern = arg + " == " + to_string(node.value);
        } else {
            output << node.value;
        }
    }

    void CCodeGenerator::visit(Char &node)
    {
        if (id_context == IdContext::PATTERN) {
            string arg = "";

            for (auto str : arg_name_stack) {
                arg += str;
            }

            last_pattern = arg + " == '" + node.value + "'";
        } else {
            output << "'" << node.value << "'";
        }
    }

    void CCodeGenerator::visit(String &node)
    {
        if (id_context == IdContext::PATTERN) {
            string arg = "";

            for (auto str : arg_name_stack) {
                arg += str;
            }

            last_pattern = "gcompare_string(" + arg + ", \"" + node.value + "\", " + to_string(list_offsets.back()) + ")";
        } else {
            output << "gcreate_string(\"" << node.value << "\")";
        }
    }

    void CCodeGenerator::visit(List &node)
    {
        string name;
        auto got = lists.find(*node.node_type);

        // If current list doesn't exists. Create it
        if (got == lists.end()) {
            name = generate_list(*node.node_type);
        } else {
            name = got->second;
        }

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
        auto got = tuples.find(*(node.node_type));

        // If current tuple doesn't exists. Create it
        if (got == tuples.end()) {
            name = generate_tuple(*node.node_type);
        } else {
            name = got->second;
        }

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

        switch (id_context) {
            case IdContext::PATTERN:

                for (auto str : arg_name_stack) {
                    name += str;
                }

                node.node_type->accept(*this);
                assign << last_type << " u" << node.id << " = ";

                if (node.node_type->type == Types::LIST || node.node_type->type == Types::STRING) {
                    assign << "gclone_" << lists[*node.node_type] << "(" << name << ", " << list_offsets.back() << ");";
                } else {
                    assign << name << ";";
                }

                assignments.push_back(assign.str());

                last_pattern = "1";
                break;
            case IdContext::EXPR:
                switch (node.node_type->type){
                    case Types::LIST:
                        output << "u" << node.id;
                        break;
                    case Types::STRING:
                        output << "u" << node.id;
                        break;
                    default:
                        output << "u" << node.id;
                        break;
                }
                break;
        }
    }

    void CCodeGenerator::visit(Call &node)
    {
        output << "(";
        node.callee->accept(*this);

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
            case Types::TUPLE:
                got = tuples.find(node);

                if (got != tuples.end()) {
                    last_type = got->second;
                } else {
                    last_type = generate_tuple(node);
                }
                break;
            case Types::SIGNATURE:
                got = signatures.find(node);

                if (got != signatures.end()) {
                    last_type = got->second;
                } else {
                    last_type = generate_signature(node);
                }
                break;
            case Types::STRING:
                last_type = string_type_name;
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
        // result is needed, so we don't start generating something in a signature in the headert() file
        stringstream result;
        string name = "gl";
        int tap_count = 0;
        name += to_string(list_count);

        // increase list count, so next list doesn't have the same name
        list_count++;

        type.types.front()->accept(*this);

        /* generation of list starts here */
        t(result, tap_count) << "typedef struct " << name << endl;
        t(result, tap_count) << "{" << endl;
        tap_count++;
        t(result, tap_count) << last_type << " *items;" << endl;
        t(result, tap_count) << "int head;" << endl;
        t(result, tap_count) << "int size;" << endl;
        tap_count--;
        t(result, tap_count) << "}" << name << ";" << endl;
        t(result, tap_count) << endl;
        /* generation of list ends here */

        /* generation of list push starts here */
        t(result, tap_count) << name << " *gpush_" << name << "(" << name << " *this, " << last_type << " item)" << endl;
        t(result, tap_count) << "{" << endl;
        tap_count++;
        t(result, tap_count) << "if (this->head >= this->size)" << endl;
        t(result, tap_count) << "{" << endl;
        tap_count++;
        t(result, tap_count) << "this->items = (" << last_type << " *)realloc(this->items, (this->size *= 2) * sizeof(" << last_type << "));" << endl;
        tap_count--;
        t(result, tap_count) << "}" << endl;
        t(result, tap_count) << endl;
        t(result, tap_count) << "this->items[this->head++] = item;" << endl;
        t(result, tap_count) << "return this;" << endl;
        tap_count--;
        t(result, tap_count) << "}" << endl;
        t(result, tap_count) << endl;
        /* generation of list push ends here */

        /* generation of list constructer starts here */
        t(result, tap_count) << name << " *gcreate_" << name << "(int count, ...)" << endl;
        t(result, tap_count) << "{" << endl;
        tap_count++;
        t(result, tap_count) << "int i;" << endl;
        t(result, tap_count) << "int size = gnearest_pow2(count);" << endl;
        t(result, tap_count) << "" << name << " *res = (" << name << "*)malloc(sizeof(" << name << "));" << endl;
        t(result, tap_count) << endl;
        t(result, tap_count) << "res->head = 0;" << endl;
        t(result, tap_count) << "res->size = size;" << endl;
        t(result, tap_count) << "res->items = (" << last_type << " *)malloc(size * sizeof(" << last_type << "));" << endl;
        t(result, tap_count) << endl;
        t(result, tap_count) << "va_list args;" << endl;
        t(result, tap_count) << "va_start(args, count);" << endl;
        t(result, tap_count) << endl;
        t(result, tap_count) << "for(i = 0; i < count; i++)" << endl;
        t(result, tap_count) << "{" << endl;
        tap_count++;
        t(result, tap_count) << "gpush_" << name << "(res, va_arg(args, " << last_type << "));" << endl;
        tap_count--;
        t(result, tap_count) << "}" << endl;
        t(result, tap_count) << endl;
        t(result, tap_count) << "va_end(args);" << endl;
        t(result, tap_count) << "return res;" << endl;
        tap_count--;
        t(result, tap_count) << "}" << endl;
        t(result, tap_count) << endl;
        /* generation of list constructer ends here */

        /* generation of at function starts here */
        t(result, tap_count) << last_type << " gat_" << name << "(" << name << " *this, int index)" << endl;
        t(result, tap_count) << "{" << endl;
        tap_count++;
        t(result, tap_count) << "return this->items[this->head - (1 + index)];" << endl;
        tap_count--;
        t(result, tap_count) << "}" << endl;
        t(result, tap_count) << endl;
        /* generation of at function ends here */

        /* generation of list constructer starts here */
        t(result, tap_count) << name << " *gclone_" << name << "(" << name << " *list, int offset)" << endl;
        t(result, tap_count) << "{" << endl;
        tap_count++;
        t(result, tap_count) << "int i;" << endl;
        t(result, tap_count) << name << " *res = (" << name << "*)malloc(sizeof(" << name << "));" << endl;
        t(result, tap_count) << endl;
        t(result, tap_count) << "res->head = list->head - offset;" << endl;
        t(result, tap_count) << "res->size = list->size;" << endl;
        t(result, tap_count) << "res->items = (" << last_type << " *)malloc(list->size * sizeof(" << last_type << "));" << endl;
        t(result, tap_count) << endl;
        t(result, tap_count) << "for (i = 0; i < res->head; i++)" << endl;
        t(result, tap_count) << "{" << endl;
        tap_count++;
        t(result, tap_count) << "res->items[i] = list->items[i];" << endl;
        tap_count--;
        t(result, tap_count) << "}" << endl;
        t(result, tap_count) << "return res;" << endl;
        t(result, tap_count) << endl;
        tap_count--;
        t(result, tap_count) << "}" << endl;
        t(result, tap_count) << endl;
        /* generation of list constructer ends here */

        /* TODO */

        // writing list to headert()file
        headert()<< result.str();

        // save list in signature hash map
        lists[type] = name;

        // return name of list generated
        return name;
    }

    string CCodeGenerator::generate_signature(Type &type) {
        // t(result, tap_count) is needed, so we don't start generating something in a signature in the headert()file
        stringstream result;
        string name = "gs";
        int tap_count = 0;
        name += to_string(sig_count);

        // increase signature count, so next signature doesn't have the same name
        sig_count++;

        type.types.back()->accept(*this);
        t(result, tap_count) << "typedef " << last_type << " (* " << name << ")(";

        for (size_t i = 0; i < type.types.size() - 1; ++i) {
            type.types[i]->accept(*this); // generate the actual type of the argument
            t(result, tap_count) << last_type;

            if (i < type.types.size() - 2) // don't print ", " after the last argument
                t(result, tap_count) << ", ";
        }

        t(result, tap_count) << ");" << endl;
        t(result, tap_count) << endl;

        // writing signature to headert()file
        headert()<< result.str();

        // save signature in signature hash map
        signatures[type] = name;

        // return name of signature generated
        return name;
    }

    string CCodeGenerator::generate_tuple(Type &type) {
        // t(result, tap_count) is needed, so we don't start generating something in a tuple in the headert()file
        stringstream result;
        string name = "gt";
        int tap_count = 0;
        name += to_string(tuple_count);

        // increase tuple count, so next tuple doesn't have the same name
        tuple_count++;

        /* generation of tuple starts here */
        t(result, tap_count) << "typedef struct " << name << endl;
        t(result, tap_count) << "{" << endl;
        tap_count++;
        // generate an item for each type in the tuple
        for (size_t i = 0; i < type.types.size(); ++i) {
            type.types[i]->accept(*this); // generate the actual type of the item
            t(result, tap_count) << last_type << " gi" << i << ";" << endl; // give this item a unique name
        }
        tap_count--;
        t(result, tap_count) << "}" << name << ";" << endl;
        t(result, tap_count) << endl;
        /* generation of tuple ends here */

        /* generation of tuple contructor starts here */
        // give contructor a unique name
        t(result, tap_count) << name << " gcreate_" << name << "(";

        // generate an argument for each item in the struct
        for (size_t i = 0; i < type.types.size(); ++i) {
            type.types[i]->accept(*this); // generate the actual type of the argument
            t(result, tap_count) << last_type << " gi" << i;

            if (i < type.types.size() - 1) // don't print ", " after the last argument
                t(result, tap_count) << ", ";
        }
        t(result, tap_count) << ")" << endl;
        t(result, tap_count) << "{" << endl;
        tap_count++;

        // generate a t(result, tap_count) variable
        t(result, tap_count) << "" << name << " " << "res;" << endl;

        // for each item in res, assign values
        for (size_t i = 0; i < type.types.size(); ++i) {
            t(result, tap_count) << "res.gi" << i << " = gi" << i << ";" << endl;
        }
        t(result, tap_count) << "return res;" << endl;
        tap_count--;
        t(result, tap_count) << "}" << endl;
        /* generation of tuple contructor ends here */

        t(result, tap_count) << endl;

        // writing tuple to headert()file
        headert()<< result.str();

        // save tuple in tuple hash map
        tuples[type] = name;

        // return name of tuple generated
        return name;
    }

    void CCodeGenerator::generate_std() {

        headert() << "#include <stdarg.h>" << endl;
        headert() << "#include <stdio.h>" << endl;
        headert() << "#include <stdlib.h>" << endl;
        headert() << "#include <string.h>" << endl;
        headert() << endl;

        /* generating to nearest power of 2 function starts here */
        headert() << "int gnearest_pow2(int gn)" << endl;
        headert() << "{" << endl;
        header_tap_count++;
        headert() << "gn--;" << endl;
        headert() << "gn |= gn >> 1;" << endl;
        headert() << "gn |= gn >> 2;" << endl;
        headert() << "gn |= gn >> 4;" << endl;
        headert() << "gn |= gn >> 8;" << endl;
        headert() << "gn |= gn >> 16;" << endl;
        headert() << "gn++;" << endl;
        headert() << endl;
        headert() << "return gn + (gn == 0);" << endl;
        header_tap_count--;
        headert() << "}" << endl;
        headert() << endl;
        /* generating to nearest power of 2 function ends here */

        string_type_name = generate_list(real_string);
        lists[fake_string] = string_type_name;
        generate_list(string_list);

        /* generation of string constructer starts here */
        headert() << string_type_name << " *gcreate_string(char* values)" << endl;
        headert() << "{" << endl;
        header_tap_count++;
        headert() << "int i;" << endl;
        headert() << "int str_length = strlen(values);" << endl;
        headert() << "int size = gnearest_pow2(str_length);" << endl;
        headert() << string_type_name << " *res = (" << string_type_name << "*)malloc(sizeof(" << string_type_name << "));" << endl;
        headert() << endl;
        headert() << "res->head = 0;" << endl;
        headert() << "res->size = size;" << endl;
        headert() << "res->items = (int *)malloc(size * sizeof(int));" << endl;
        headert() << endl;
        headert() << "for(i = str_length - 1; i >= 0; i--)" << endl;
        headert() << "{" << endl;
        header_tap_count++;
        headert() << "gpush_" << string_type_name << "(res, values[i]);" << endl;
        header_tap_count--;
        headert() << "}" << endl;
        headert() << endl;
        headert() << "return res;" << endl;
        header_tap_count--;
        headert() << "}" << endl;
        headert() << endl;
        /* generation of string constructer ends here */

        /* generation of string compare here */
        headert() << "int gcompare_string(" << string_type_name << "*string, char* values, int offset)" << endl;
        headert() << "{" << endl;
        header_tap_count++;
        headert() << "int i, j;" << endl;
        headert() << "int size = strlen(values);" << endl;
        headert() << endl;
        headert() << "if (size == string->head - offset)" << endl;
        headert() << "{" << endl;
        header_tap_count++;
        headert() << "for (i = 0, j = size - 1; i < size; i++, j--)" << endl;
        headert() << "{" << endl;
        header_tap_count++;
        headert() << "if (string->items[j] != values[i])" << endl;
        header_tap_count++;
        headert() << "return 0;" << endl;
        header_tap_count--;
        header_tap_count--;
        headert() << "}" << endl;
        header_tap_count--;
        headert() << "}" << endl;
        headert() << "else" << endl;
        headert() << "{" << endl;
        header_tap_count++;
        headert() << "return 0;" << endl;
        header_tap_count--;
        headert() << "}" << endl;
        headert() << endl;
        headert() << "return 1;" << endl;
        header_tap_count--;
        headert() << "}" << endl;
        headert() << endl;
        /* generation of string compare ends here */

        headert() << endl;
    }



    string CCodeGenerator::tap(int i) {
        string res = "";

        for (int j = 0; j < i; j++) {
            res += "    ";
        }

        return res;
    }

    ostream &CCodeGenerator::outputt() {
        return t(output, output_tap_count);
    }

    ostream &CCodeGenerator::headert() {
        return t(header, header_tap_count);
    }

    ostream &CCodeGenerator::t(ostream &stream, int i) {
        stream << tap(i);
        return stream;
    }
}
