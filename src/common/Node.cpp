#include "Node.h"

#include "Visitor.h"

using namespace std;

namespace common {

    void Program::accept(Visitor &v) { v.visit(*this); }
    void Function::accept(Visitor &v) { v.visit(*this); }
    void Case::accept(Visitor &v) { v.visit(*this); }

    void List::accept(Visitor &v) { v.visit(*this); }
    void Tuple::accept(Visitor &v) { v.visit(*this); }
    void Call::accept(Visitor &v) { v.visit(*this); }

    Node::Node(Location loc) :
            type(TypeId::UNKNOWN), loc(loc) { }

    Node::Node(Type type, Location loc) :
            type(type), loc(loc) { }

    Expr::Expr(Location loc) :
            Node(loc) { }

    Expr::Expr(Type type, Location loc) :
            Node(type, loc) { }


    Program::Program(vector<unique_ptr<Function>> funcs,
                     Location loc) :
            Node(loc),
            funcs(move(funcs)) { }

    Program::Program(unique_ptr<Expr> expr,
                     Location loc) :
                     Node(loc) {

        funcs.push_back(make_unique<Function>(move(expr)));
    }

    Function::Function(unique_ptr<Expr> expr) :
            Node(loc),
            id(ANON_FUNC_NAME),
            signature(Type(TypeId::UNKNOWN)),
            is_anon(true) {
        cases.push_back(make_unique<Case>(move(expr), vector<unique_ptr<Pattern>>(), expr->loc));
    }

    Function::Function(string id,
                       Type sign,
                       Location loc) :
            Node(sign.subtypes.front(), loc),
            id(id),
            signature(sign) { }

    Case::Case(unique_ptr<Expr> expr,
               vector<unique_ptr<Pattern>> patterns,
               Location loc) :
            Node(loc),
            expr(move(expr)),
            patterns(move(patterns)) { }

    Call::Call(unique_ptr<Expr> callee,
               vector<unique_ptr<Expr>> exprs,
               Location loc) :
            Expr(loc),
            callee(move(callee)),
            exprs(move(exprs))
    {
    }

    string Program::str() {
        string str;
        for (auto& func: funcs) {
            str += func->str();
        }

        return str;
    }

    string Function::str() {
        string str("def " + id + " : ");

        for (auto &type: signature.subtypes) {
            str += type.str();
            if (type != signature.subtypes.back())
                str += " -> ";
        }

        str += '\n';

        for (auto &cse : cases) {
            str += cse->str() + "\n";
        }

        return str + "\n";
    }

    string Case::str() {
        string str("\t| ");

        for (auto &pattern: patterns) {
            str += pattern->str();
            if (pattern != patterns.back())
                str += ' ';
        }

        return str + " = " + expr->str();
    }

    string ListPattern::str() {
        string str("[");

        for (auto &pattern: patterns) {
            str += pattern->str();
            if (pattern != patterns.back())
                str += ", ";
        }

        return str + "]";
    }

    string TuplePattern::str() {
        string str("(");

        for (auto &pattern: patterns) {
            str += pattern->str();
            if (pattern != patterns.back())
                str += ", ";
        }

        return str + ")";
    }

    string ListSplit::str() {
        return "(" + left->str() + " : " + right->str() + ")";
    }

    string Or::str() {
        return left->str() + " || " + right->str();
    }

    string And::str() {
        return left->str() + " && " + right->str();
    }

    string Equal::str() {
        return left->str() + " == " + right->str();
    }

    string NotEqual::str() {
        return left->str() + " != " + right->str();
    }

    string Lesser::str() {
        return left->str() + " < " + right->str();
    }

    string Greater::str() {
        return left->str() + " > " + right->str();
    }

    string LesserEq::str() {
        return left->str() + " <= " + right->str();
    }

    string GreaterEq::str() {
        return left->str() + " >= " + right->str();
    }

    string Add::str() {
        return left->str() + " + " + right->str();
    }

    string Sub::str() {
        return left->str() + " - " + right->str();
    }

    string Mul::str() {
        return left->str() + " * " + right->str();
    }

    string Div::str() {
        return left->str() + " / " + right->str();
    }

    string Mod::str() {
        return left->str() + " % " + right->str();
    }

    string ListAdd::str() {
        return left->str() + " : " + right->str();
    }

    string Par::str() {
        return "(" + child->str() + ")";
    }

    string Not::str() {
        return "!" + child->str();
    }

    string Int::str() {
        return to_string(value);
    }

    string Float::str() {
        return value == (long)value ? to_string(value) + ".0" : to_string(value);
    }

    string Bool::str() {
        return value ? "True" : "False";
    }

    string Char::str() {
        return "\'" + to_string(value) + "\'";
    }

    string String::str() {
        return "\"" + value + "\"";
    }

    string List::str() {
        string str("[");

        for (auto &expr: exprs) {
            str += expr->str();
            if (expr != exprs.back())
                str += ", ";
        }

        return str + "]";
    }

    string Tuple::str() {
        string str("(");

        for (auto &expr: exprs) {
            str += expr->str();
            if (expr != exprs.back())
                str += ", ";
        }

        return str + ")";
    }

    string Id::str() {
        return id;
    }

    string Call::str() {
        string str(callee->str() + "(");

        for (auto &expr: exprs) {
            str += expr->str();
            if (expr != exprs.back())
                str += ", ";
        }

        return str + ")";
    }


}
