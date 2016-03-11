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
    }

    void CCodeGenerator::visit(Program &node)
    {
        stringstream func_name;
        scope_generator.visit(node);
        type_checker.visit(node);

        outputt()<< "#include <stdarg.h>" << endl;
        outputt()<< "#include <stdio.h>" << endl;
        outputt()<< "#include <stdlib.h>" << endl;
        // TODO real header
        outputt() << "#include \"test.h\"" << endl;
        outputt()<< endl;
        outputt()<< "int main(int argc, char** argv)" << endl;
        outputt()<< "{" << endl;
        output_tap_count++;
        outputt()<< "return umain();" << endl;
        output_tap_count--;
        outputt()<< "}" << endl;
        outputt()<< endl;


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
        for (int i = 0; i < node.types.size() - 1; i++) {
            node.types[i]->accept(*this);
            arg_name << "ga" << i;
            arg_names.push_back(arg_name.str());
            func_name << last_type.str() << " " << arg_name.str();

            if (i < node.types.size() - 2) // don't generate ", " after last argument
                func_name<< ", ";

            arg_name.str("");
        }

        func_name << ")";

        node.types.back()->accept(*this);

        // generate function decleration in header
        headert()<< last_type.str() << " " << func_name.str() << ";" << endl;

        // generate function in output
        outputt()<< last_type.str() << " " << func_name.str() << endl;

        // generate cases
        outputt()<< "{" << endl;
        output_tap_count++;

        for (auto c : node.cases) {
            c->accept(*this);
        }

        output_tap_count--;
        outputt()<< "}" << endl;
        arg_names.clear();
    }

    void CCodeGenerator::visit(Case &node)
    {
        // TODO better pattern matching

        if (node.patterns.size() != 0) {
            // generate if-statement for matching
            outputt() << "if (";

            id_context = IdContext::PATTERN;
            for (arg_count = 0; arg_count < node.patterns.size(); ++arg_count) {
                output << "ga" << arg_count << " == ";
                node.patterns[arg_count]->accept(*this);

                if (arg_count < node.patterns.size() - 1 )
                    output << " && ";
            }

            output << ")" << endl;
        }

        outputt()<< "{" << endl;
        output_tap_count++;

        // generate return expression
        id_context = IdContext::EXPR;
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
        /* TODO */
        throw "Not implemented";
    }

    void CCodeGenerator::visit(TuplePattern &node)
    {
        /* TODO */
        throw "Not implemented";
    }

    void CCodeGenerator::visit(ListSplit &node)
    {
        /* TODO */
        throw "Not implemented";
    }

    void CCodeGenerator::visit(Int &node)
    {
        output << node.value;
    }

    void CCodeGenerator::visit(Float &node)
    {
        output << node.value;
    }

    void CCodeGenerator::visit(Bool &node)
    {
        output << node.value;
    }

    void CCodeGenerator::visit(Char &node)
    {
        output << "'" << node.value << "'";
    }

    void CCodeGenerator::visit(String &node)
    {
        /* TODO */
        throw "Not implemented";
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

        output << "gcreate_" << name << "(";
        for (auto expr : node.exprs){
            expr->accept(*this);

            if (expr != node.exprs.back())
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
        switch (id_context) {
            case IdContext::PATTERN:
                node.scope->real_ids[node.id] = arg_names[arg_count];
                output << arg_names[arg_count];
                break;
            case IdContext::EXPR:
                switch (node.node_type->type){
                    case Types::LIST:
                        output << node.scope->get_real_id(node.id);
                        break;
                    case Types::STRING:
                        output << node.scope->get_real_id(node.id);
                        break;
                    default:
                        output << node.scope->get_real_id(node.id);
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
                last_type.str("double");
                break;
            case Types::CHAR:
                last_type.str("char");
                break;
            case Types::INT:
                last_type.str("int");
                break;
            case Types::BOOL:
                last_type.str("int");
                break;
            case Types::TUPLE:
                got = tuples.find(node);

                if (got != tuples.end()) {
                    last_type.str(got->second);
                } else {
                    last_type.str(generate_tuple(node));
                }
                break;
            case Types::SIGNATURE:
                got = signatures.find(node);

                if (got != signatures.end()) {
                    last_type.str(got->second);
                } else {
                    last_type.str(generate_signature(node));
                }
                break;
            case Types::STRING:
                break;
            case Types::LIST:
                got = lists.find(node);

                if (got != lists.end()) {
                    last_type.str(got->second);
                } else {
                    last_type.str(generate_list(node));
                }
                last_type << " *";
            default:
                break;
        }
    }

    string CCodeGenerator::generate_list(Type &type) {
        // t(result, tap_count) is needed, so we don't start generating something in a signature in the headert()file
        stringstream result;
        string name = "gl";
        int tap_count = 0;
        name += to_string(list_count);

        type.types.front()->accept(*this);

        /* generation of list starts here */
        t(result, tap_count) << "typedef struct " << name << endl;
        t(result, tap_count) << "{" << endl;
        tap_count++;
        t(result, tap_count) << last_type.str() << " *items;" << endl;
        t(result, tap_count) << "int head;" << endl;
        t(result, tap_count) << "int size;" << endl;
        tap_count--;
        t(result, tap_count) << "};";
        /* generation of list ends here */

        /* generation of list push starts here */
        t(result, tap_count) << name << " *gpush_" << name << "(" << name << " *this, ";
        t(result, tap_count) << last_type.str() << " item)" << endl;
        t(result, tap_count) << "{" << endl;
        tap_count++;
        t(result, tap_count) << "if (this->head >= this->size)" << endl;
        t(result, tap_count) << "{" << endl;
        tap_count++;
        t(result, tap_count) << "this->items = (" << last_type.str() << " *)realloc(this->items, (this->size *= 2) * sizeof(" << last_type.str() << ");" << endl;
        tap_count--;
        t(result, tap_count) << "}" << endl;
        t(result, tap_count) << "this->items[this->head++] = item;" << endl;
        t(result, tap_count) << "return this;" << endl;
        tap_count--;
        t(result, tap_count) << "}" << endl;
        /* generation of list push ends here */

        /* generation of list constructer starts here */
        t(result, tap_count) << name << " *gcreate_" << name << "(int count, ...)" << endl;
        t(result, tap_count) << "{" << endl;
        tap_count++;
        t(result, tap_count) << "int i;" << endl;
        // rounding the count to next power of 2 (32 bit int specific)
        t(result, tap_count) << "int size = count - 1;" << endl;
        t(result, tap_count) << "size = (size |= (size |= (size |= (size |= (size |= size >> 1) >> 2) >> 4) >> 8) >> 16) + ((size += 1) == 0);" << endl;
        t(result, tap_count) << "" << name << " *res = (" << name << "*)malloc(sizeof(" << name << "));" << endl;
        t(result, tap_count) << "res->head = 0;" << endl;
        t(result, tap_count) << "res->size = size;" << endl;
        t(result, tap_count) << "res->items = (" << last_type.str() << " *)malloc(size * sizeof(" << last_type.str() << "));";
        t(result, tap_count) << "va_list args;" << endl;
        t(result, tap_count) << "va_start(args, count);" << endl;
        t(result, tap_count) << "for(i = count - 1; i >= 0; i--)" << endl;
        t(result, tap_count) << "{" << endl;
        tap_count++;
        t(result, tap_count) << "gpush_" << name << "(res, va_arg(args, i));" << endl;
        tap_count--;
        t(result, tap_count) << "}" << endl;
        t(result, tap_count) << "va_end(args);" << endl;
        tap_count--;
        t(result, tap_count) << "}" << endl;
        /* generation of list constructer ends here */

        /* TODO */

        // increase list count, so next list doesn't have the same name
        list_count++;

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

        type.types.back()->accept(*this);
        t(result, tap_count) << "typedef " << last_type.str() << " (* " << name << ")(";

        for (int i = 0; i < type.types.size() - 1; ++i) {
            type.types[i]->accept(*this); // generate the actual type of the argument
            t(result, tap_count) << last_type.str();

            if (i < type.types.size() - 2) // don't print ", " after the last argument
                t(result, tap_count) << ", ";
        }

        t(result, tap_count) << ")";

        // increase signature count, so next signature doesn't have the same name
        sig_count++;

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

        /* generation of tuple starts here */
        t(result, tap_count) << "typedef struct " << name << endl;
        t(result, tap_count) << "{" << endl;
        tap_count++;
        // generate an item for each type in the tuple
        for (int i = 0; i < type.types.size(); ++i) {
            type.types[i]->accept(*this); // generate the actual type of the item
            t(result, tap_count) << last_type.str() << " i" << i << ";" << endl; // give this item a unique name
        }
        tap_count--;
        t(result, tap_count) << "}" << name << ";" << endl;
        t(result, tap_count) << endl;
        /* generation of tuple ends here */

        /* generation of tuple contructor starts here */
        // give contructor a unique name
        t(result, tap_count) << name << " gcreate_" << name << "(";

        // generate an argument for each item in the struct
        for (int i = 0; i < type.types.size(); ++i) {
            type.types[i]->accept(*this); // generate the actual type of the argument
            t(result, tap_count) << last_type.str() << " i" << i;

            if (i < type.types.size() - 1) // don't print ", " after the last argument
                t(result, tap_count) << ", ";
        }
        t(result, tap_count) << ")" << endl;
        t(result, tap_count) << "{" << endl;
        tap_count++;

        // generate a t(result, tap_count) variable
        t(result, tap_count) << "" << name << " " << "res;" << endl;

        // for each item in res, assign values
        for (int i = 0; i < type.types.size(); ++i) {
            t(result, tap_count) << "res.i" << i << " = i" << i << ";" << endl;
        }
        t(result, tap_count) << "return res;" << endl;
        tap_count--;
        t(result, tap_count) << "}" << endl;
        /* generation of tuple contructor ends here */

        // increase tuple count, so next tuple doesn't have the same name
        tuple_count++;

        // writing tuple to headert()file
        headert()<< result.str();

        // save tuple in tuple hash map
        tuples[type] = name;

        // return name of tuple generated
        return name;
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
