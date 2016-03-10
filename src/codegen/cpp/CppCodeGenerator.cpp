#include "CppCodeGenerator.h"
#include <string>
#include <iostream>
#include <sstream>

using namespace common;
using namespace std;


CCodeGenerator::CCodeGenerator(std::ostream &os, std::ostream &head)
        : CodeGenerator::CodeGenerator(os), header(head)
{
}

void CCodeGenerator::visit(Program &node)
{
    for (auto f : node.funcs) {
        f->accept(*this);
    }
}

void CCodeGenerator::visit(Function &node)
{
    stringstream arg_name;

    // generate return type
    node.types.back()->accept(*this);

    // give function a unique name
    output<< " u" << node.id << "( ";

    // generate function arguments
    for (arg_count = 0; arg_count < node.types.size() - 1; arg_count++) {
        arg_name << "ga" << arg_count;
        node.types[arg_count]->accept(*this);
        arg_names.push_back(arg_name.str());
        output << " " << arg_name;

        if (arg_count < node.types.size() - 2) // don't generate ", " after last argument
            output<< ", ";
    }

    output<< ") {" << endl;

    for (auto c : node.cases) {
        c->accept(*this);
        real_ids.clear();
    }

    output<< "}" << endl;
    arg_names.clear();
}

void CCodeGenerator::visit(Case &node)
{
    output<< "if (";

    id_context = IdContext::PATTERN;
    for (int i = 0; i < node.patterns.size(); ++i) {
        output<< "ga" << i << " == ";
        node.patterns[i]->accept(*this);

        if (i < node.patterns.size() - 1 )
            output<< " && ";
    }

    id_context = IdContext::EXPR;

    output<< ") {" << endl;
    output<< "return ";
    node.expr->accept(*this);
    output<< endl << "}" << endl;
}

void CCodeGenerator::visit(Or &node)
{
    output<< "(";
    node.left->accept(*this);
    output<< "||";
    node.right->accept(*this);
    output<< ")";
}

void CCodeGenerator::visit(And &node)
{
    output<< "(";
    node.left->accept(*this);
    output<< "&&";
    node.right->accept(*this);
    output<< ")";
}

void CCodeGenerator::visit(Equal &node)
{
    output<< "(";
    node.left->accept(*this);
    output<< "==";
    node.right->accept(*this);
    output<< ")";
}

void CCodeGenerator::visit(NotEqual &node)
{
    output<< "(";
    node.left->accept(*this);
    output<< "!=";
    node.right->accept(*this);
    output<< ")";
}

void CCodeGenerator::visit(Lesser &node)
{
    output<< "(";
    node.left->accept(*this);
    output<< "<";
    node.right->accept(*this);
    output<< ")";
}

void CCodeGenerator::visit(LesserEq &node)
{
    output<< "(";
    node.left->accept(*this);
    output<< "<=";
    node.right->accept(*this);
    output<< ")";
}

void CCodeGenerator::visit(Greater &node)
{
    output<< "(";
    node.left->accept(*this);
    output<< ">";
    node.right->accept(*this);
    output<< ")";
}

void CCodeGenerator::visit(GreaterEq &node)
{
    output<< "(";
    node.left->accept(*this);
    output<< ">=";
    node.right->accept(*this);
    output<< ")";
}

void CCodeGenerator::visit(Add &node)
{
    output<< "(";
    node.left->accept(*this);
    output<< "+";
    node.right->accept(*this);
    output<< ")";
}

void CCodeGenerator::visit(Sub &node)
{
    output<< "(";
    node.left->accept(*this);
    output<< "-";
    node.right->accept(*this);
    output<< ")";
}

void CCodeGenerator::visit(Mul &node)
{
    output<< "(";
    node.left->accept(*this);
    output<< "*";
    node.right->accept(*this);
    output<< ")";
}

void CCodeGenerator::visit(Div &node)
{
    output<< "(";
    node.left->accept(*this);
    output<< "/";
    node.right->accept(*this);
    output<< ")";
}

void CCodeGenerator::visit(Mod &node)
{
    output<< "(";
    node.left->accept(*this);
    output<< "%";
    node.right->accept(*this);
    output<< ")";
}

void CCodeGenerator::visit(ListAdd &node)
{
    
}

void CCodeGenerator::visit(Par &node)
{
    output<< "(";
    node.child->accept(*this);
    output<< ")";
}

void CCodeGenerator::visit(Not &node)
{
    output<< "(";
    output<< "!";
    node.child->accept(*this);
    output<< ")";
}

void CCodeGenerator::visit(ListPattern &node)
{

}

void CCodeGenerator::visit(TuplePattern &node)
{
    
}

void CCodeGenerator::visit(ListSplit &node)
{
    
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
    output << node.value;
}

void CCodeGenerator::visit(String &node)
{
    /* TODO */
}

void CCodeGenerator::visit(List &node)
{
    /* TODO */
}

void CCodeGenerator::visit(Tuple &node)
{
    string name;
    auto got = tuples.find(node.node_type);

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
            real_ids.insert({node.id, arg_names[arg_count]});
            output<< arg_names[arg_count];
            break;
        case IdContext::EXPR:
            output<< real_ids[node.id];
            break;
    }
}

void CCodeGenerator::visit(Call &node)
{
    output<< "(";
    node.callee->accept(*this);

    output<< "(";
    for (auto expr : node.exprs){
        expr->accept(*this);

        if (expr != node.exprs.back())
            output<< ", ";
    }
    output<< ")";

    output<< ")";
}

void CCodeGenerator::visit(Type &node) {
    switch (node.type) {
        case FLOAT:
            output << "double";
            break;
        case CHAR:
            output << "char";
            break;
        case INT:
            output << "int";
            break;
        case BOOL:
            output << "int";
            break;
        case TUPLE:
            auto got = tuples.find(node);

            if (got != tuples.end()) {
                output << got->second;
            } else {
                output << generate_tuple(node);
            }
            break;
        case SIGNATURE:
            /* TODO */
        case STRING:
            /* TODO */
        case LIST:
            /* TODO */
        default:
            break;
    }
}

string CCodeGenerator::generate_tuple(Type &type){
    // result is needed, so we don't start generating a tuple in a tuple in the header file
    stringstream result;
    stringstream name;

    // give tuple unique name
    name << "gt" << tuple_count;

    /* generation of tuple starts here */
    result << "typedef struct " << name << endl;
    result << "{" << endl;

    // generate an item for each type in the tuple
    for (int i = 0; i < type.types.size(); ++i) {
        result << "\t";
        type.types[i]->accept(*this); // generate the actual type of the item
        result << " i" << i << ";" << endl; // give this item a unique name
    }

    result << "} " << name << endl;
    result << endl;
    /* generation of tuple ends here */

    /* generation of tuple contructor starts here */
    // give contructor a unique name
    result << name << " gcreate_" << name << "(";

    // generate an argument for each item in the struct
    for (int i = 0; i < type.types.size(); ++i) {
        type.types[i]->accept(*this); // generate the actual type of the argument
        result << " i" << i;

        if (i < type.types.size() - 1) // don't print ", " after the last argument
            result << ", ";
    }
    result << ")" << endl;
    result << "{" << endl;

    // generate a result variable
    result << "\t" << name << " " << "res;" << endl;

    // for each item in res, assign values
    for (int i = 0; i < type.types.size(); ++i) {
        result << "\tres.i" << i << " = i" << i << ";" << endl;
    }
    result << "return res" << endl;
    result << "}" << endl;
    /* generation of tuple contructor ends here */

    // increase tuple count, so next tuple doesn't have the same name
    tuple_count++;

    // writing tuple to header file
    header << result;

    // save tuple in tuple hash map
    tuples[type] = name.str();

    // return name of tuple generated
    return name.str();
}
