#include "Node.h"
#include "Visitor.h"
using namespace std;

namespace common {

    void Program::accept(Visitor &v) { v.visit(*this); }
    void Function::accept(Visitor &v) { v.visit(*this); }
    void Case::accept(Visitor &v) { v.visit(*this); }
    void Or::accept(Visitor &v) { v.visit(*this); }
    void And::accept(Visitor &v) { v.visit(*this); }
    void Equal::accept(Visitor &v) { v.visit(*this); }
    void NotEqual::accept(Visitor &v) { v.visit(*this); }
    void Lesser::accept(Visitor &v) { v.visit(*this); }
    void Greater::accept(Visitor &v) { v.visit(*this); }
    void LesserEq::accept(Visitor &v) { v.visit(*this); }
    void GreaterEq::accept(Visitor &v) { v.visit(*this); }
    void Add::accept(Visitor &v) { v.visit(*this); }
    void Sub::accept(Visitor &v) { v.visit(*this); }
    void Mul::accept(Visitor &v) { v.visit(*this); }
    void Div::accept(Visitor &v) { v.visit(*this); }
    void Mod::accept(Visitor &v) { v.visit(*this); }
    void ListAdd::accept(Visitor &v) { v.visit(*this); }
    void Par::accept(Visitor &v) { v.visit(*this); }
    void Not::accept(Visitor &v) { v.visit(*this); }
    void Int::accept(Visitor &v) { v.visit(*this); }
    void Float::accept(Visitor &v) { v.visit(*this); }
    void Bool::accept(Visitor &v) { v.visit(*this); }
    void Char::accept(Visitor &v) { v.visit(*this); }
    void String::accept(Visitor &v) { v.visit(*this); }
    void List::accept(Visitor &v) { v.visit(*this); }
    void Tuple::accept(Visitor &v) { v.visit(*this); }
    void Id::accept(Visitor &v) { v.visit(*this); }
    void Call::accept(Visitor &v) { v.visit(*this); }
    void ListPattern::accept(Visitor &v) { v.visit(*this); }
    void TuplePattern::accept(Visitor &v) { v.visit(*this); }
    void ListSplit::accept(Visitor &v) { v.visit(*this); }
    void Type::accept(Visitor &v) { v.visit(*this); }


    bool Type::operator==(const Type &other) const {
        if (this->type == other.type){
            switch (this->type) {
                case Types::LIST:
                    return *this->types[0] == *other.types[0];
                case Types::SIGNATURE:
                case Types::TUPLE:
                    if (this->types.size() != other.types.size())
                        return false;

                    for (unsigned int i = 0; i < this->types.size(); ++i) {
                        if (*this->types[i] != *other.types[i]){
                            return false;
                        }
                    }
                default:
                    return true;
            }
        } else {
            return false;
        }
    }

    bool Type::operator!=(const Type &other) const {
        return !(*this == other);
    }

    Node::Node()
            { }

    Node::Node(Location loc) :
            loc(loc) { }

    Expr::Expr() { }

    Expr::Expr(Location loc) :
            Node(loc) { }

    BinaryOp::BinaryOp() { }

    BinaryOp::BinaryOp(Expr *left,
                       Expr *right) :
            left(left),
            right(right) { }

    BinaryOp::BinaryOp(Expr *left,
                       Expr *right,
                       Location loc) :
            Expr(loc),
            left(left),
            right(right) { }

    UnaryOp::UnaryOp() { }

    UnaryOp::UnaryOp(Expr *child) :
            child(child) { }

    UnaryOp::UnaryOp(Expr *child,
                     Location loc) :
            Expr(loc),
            child(child) { }

    Pattern::Pattern() { }

    Pattern::Pattern(Location loc) :
            Expr(loc) { }

    Program::Program() { }

    Program::Program(vector<Function *> funcs)
    {
        for (auto i : funcs)
            this->funcs.push_back(shared_ptr<Function>(i));
    }

    Program::Program(vector<Function *> funcs,
                     Location loc) :
            Node(loc)
    {
        for (auto i : funcs)
            this->funcs.push_back(shared_ptr<Function>(i));
    }

    Program::Program(Expr *expr,
                     Location loc) :
            Node(loc) {

        funcs.push_back(shared_ptr<Function>(new Function(ANON_FUNC_NAME, true)));
        funcs[0]->cases.push_back(shared_ptr<Case>(new Case(expr)));
    }

    Function::Function() { }

    Function::Function(string id, bool is_anon) :
            id(id) { this->is_anon = is_anon; }

    Function::Function(string id,
                       vector<Type *> types) :
            id(id)
    {
        for (auto i : types)
            this->types.push_back(shared_ptr<Type>(i));
    }

    Function::Function(string id,
                       vector<Type *> types,
                       Location loc) :
            Node(loc),
            id(id)
    {
        for (auto i : types)
            this->types.push_back(shared_ptr<Type>(i));
    }

    Function::Function(string id,
                       vector<Type *> types,
                       vector<Case *> cases) :
            id(id)
    {
        for (auto i : types)
            this->types.push_back(shared_ptr<Type>(i));

        for (auto i : cases)
            this->cases.push_back(shared_ptr<Case>(i));
    }

    Function::Function(string id,
                       vector<Type *> types,
                       vector<Case *> cases,
                       Location loc) :
            Node(loc),
            id(id)
    {
        for (auto i : types)
            this->types.push_back(shared_ptr<Type>(i));

        for (auto i : cases)
            this->cases.push_back(shared_ptr<Case>(i));
    }

    Case::Case() { }

    Case::Case(Expr *expr) : expr(expr) { }

    Case::Case(Expr *expr,
               vector<Pattern *> patterns) :
            expr(expr)
    {
        for (auto i : patterns)
            this->patterns.push_back(shared_ptr<Pattern>(i));
    }

    Case::Case(Expr *expr,
               vector<Pattern *> patterns,
               Location loc) :
            Node(loc),
            expr(expr)
    {
        for (auto i : patterns)
            this->patterns.push_back(shared_ptr<Pattern>(i));
    }

    Or::Or() { }

    Or::Or(Expr *left,
           Expr *right) :
            BinaryOp(left, right) { }

    Or::Or(Expr *left,
           Expr *right,
           Location loc) :
            BinaryOp(left, right, loc) { }

    And::And() { }

    And::And(Expr *left,
             Expr *right) :
            BinaryOp(left, right) { }

    And::And(Expr *left,
             Expr *right,
             Location loc) :
            BinaryOp(left, right, loc) { }

    Equal::Equal() { }

    Equal::Equal(Expr *left,
                 Expr *right) :
            BinaryOp(left, right) { }

    Equal::Equal(Expr *left,
                 Expr *right,
                 Location loc) :
            BinaryOp(left, right, loc) { }

    NotEqual::NotEqual() { }

    NotEqual::NotEqual(Expr *left,
                       Expr *right) :
            BinaryOp(left, right) { }

    NotEqual::NotEqual(Expr *left,
                       Expr *right,
                       Location loc) :
            BinaryOp(left, right, loc) { }

    Lesser::Lesser() { }

    Lesser::Lesser(Expr *left,
                   Expr *right) :
            BinaryOp(left, right) { }

    Lesser::Lesser(Expr *left,
                   Expr *right,
                   Location loc) :
            BinaryOp(left, right, loc) { }

    Greater::Greater() { }

    Greater::Greater(Expr *left,
                     Expr *right) :
            BinaryOp(left, right) { }

    Greater::Greater(Expr *left,
                     Expr *right,
                     Location loc) :
            BinaryOp(left, right, loc) { }

    LesserEq::LesserEq() { }

    LesserEq::LesserEq(Expr *left,
                       Expr *right) :
            BinaryOp(left, right) { }

    LesserEq::LesserEq(Expr *left,
                       Expr *right,
                       Location loc) :
            BinaryOp(left, right, loc) { }

    GreaterEq::GreaterEq() { }

    GreaterEq::GreaterEq(Expr *left,
                         Expr *right) :
            BinaryOp(left, right) { }

    GreaterEq::GreaterEq(Expr *left,
                         Expr *right,
                         Location loc) :
            BinaryOp(left, right, loc) { }

    Add::Add() { }

    Add::Add(Expr *left,
             Expr *right) :
            BinaryOp(left, right) { }

    Add::Add(Expr *left,
             Expr *right,
             Location loc) :
            BinaryOp(left, right, loc) { }

    Sub::Sub() { }

    Sub::Sub(Expr *left,
             Expr *right) :
            BinaryOp(left, right) { }

    Sub::Sub(Expr *left,
             Expr *right,
             Location loc) :
            BinaryOp(left, right, loc) { }

    Mul::Mul() { }

    Mul::Mul(Expr *left,
             Expr *right) :
            BinaryOp(left, right) { }

    Mul::Mul(Expr *left,
             Expr *right,
             Location loc) :
            BinaryOp(left, right, loc) { }

    Div::Div() { }

    Div::Div(Expr *left,
             Expr *right) :
            BinaryOp(left, right) { }

    Div::Div(Expr *left,
             Expr *right,
             Location loc) :
            BinaryOp(left, right, loc) { }

    Mod::Mod() { }

    Mod::Mod(Expr *left,
             Expr *right) :
            BinaryOp(left, right) { }

    Mod::Mod(Expr *left,
             Expr *right,
             Location loc) :
            BinaryOp(left, right, loc) { }

    ListAdd::ListAdd() { }

    ListAdd::ListAdd(Expr *left,
                     Expr *right) :
            BinaryOp(left, right) { }

    ListAdd::ListAdd(Expr *left,
                     Expr *right,
                     Location loc) :
            BinaryOp(left, right, loc) { }

    Par::Par() { }

    Par::Par(Expr *child) :
            UnaryOp(child) { }

    Par::Par(Expr *child,
             Location loc) :
            UnaryOp(child, loc) { }

    Not::Not() { }

    Not::Not(Expr *child) :
            UnaryOp(child) { }

    Not::Not(Expr *child,
             Location loc) :
            UnaryOp(child, loc) { }

    Int::Int(long value) :
            value(value) { }

    Int::Int(long value,
             Location loc) :
            Pattern(loc),
            value(value) { }

    Float::Float(double value) :
            value(value) { }

    Float::Float(double value,
                 Location loc) :
            Pattern(loc),
            value(value) { }

    Bool::Bool(bool value) :
            value(value) { }

    Bool::Bool(bool value,
               Location loc) :
            Pattern(loc),
            value(value) { }

    Char::Char(char value) :
            value(value) { }

    Char::Char(char value,
               Location loc) :
            Pattern(loc),
            value(value) { }

    String::String(string value) :
            value(value) { }

    String::String(string value,
                   Location loc) :
            Pattern(loc),
            value(value) { }

    ListPattern::ListPattern() { }

    ListPattern::ListPattern(vector<Pattern *> patterns)
    {
        for (auto i : patterns)
            this->patterns.push_back(shared_ptr<Pattern>(i));
    }

    ListPattern::ListPattern(vector<Pattern *> patterns,
                             Location loc) :
            Pattern(loc)
    {
        for (auto i : patterns)
            this->patterns.push_back(shared_ptr<Pattern>(i));
    }

    TuplePattern::TuplePattern() { }

    TuplePattern::TuplePattern(vector<Pattern *> patterns)
    {
        for (auto i : patterns)
            this->patterns.push_back(shared_ptr<Pattern>(i));
    }

    TuplePattern::TuplePattern(vector<Pattern *> patterns,
                               Location loc) :
            Pattern(loc)
    {
        for (auto i : patterns)
            this->patterns.push_back(shared_ptr<Pattern>(i));
    }

    ListSplit::ListSplit() { }

    ListSplit::ListSplit(Pattern *left,
                         Pattern *right) :
            left(left),
            right(right) { }

    ListSplit::ListSplit(Pattern *left,
                         Pattern *right,
                         Location loc) :
            Pattern(loc),
            left(left),
            right(right) { }

    List::List() { }

    List::List(vector<Expr *> exprs)
    {
        for (auto i : exprs)
            this->exprs.push_back(shared_ptr<Expr>(i));
    }

    List::List(vector<Expr *> exprs,
               Location loc) :
            Expr(loc)
    {
        for (auto i : exprs)
            this->exprs.push_back(shared_ptr<Expr>(i));
    }

    Tuple::Tuple() { }

    Tuple::Tuple(vector<Expr *> exprs)
    {
        for (auto i : exprs)
            this->exprs.push_back(shared_ptr<Expr>(i));
    }

    Tuple::Tuple(vector<Expr *> exprs,
                 Location loc) :
            Expr(loc)
    {
        for (auto i : exprs)
            this->exprs.push_back(shared_ptr<Expr>(i));
    }

    Id::Id() { }

    Id::Id(string id) :
            id(id) { }

    Id::Id(string id,
           Location loc) :
            Pattern(loc),
            id(id) { }

    Call::Call() { }

    Call::Call(Expr *callee) :
            callee(callee) { }

    Call::Call(Expr *callee,
               vector<Expr *> exprs) :
            callee(callee)
    {
        for (auto i : exprs)
            this->exprs.push_back(shared_ptr<Expr>(i));
    }

    Call::Call(Expr *callee,
               vector<Expr *> exprs,
               Location loc) :
            Expr(loc),
            callee(callee)
    {
        for (auto i : exprs)
            this->exprs.push_back(shared_ptr<Expr>(i));
    }

    Type::Type() { }

    Type::Type(Types type) :
            type(type) { }

    Type::Type(Types type,
               Location loc) :
            Node(loc),
            type(type) { }

    Type::Type(Types type,
               vector<Type *> types) :
            type(type)
    {
        for (auto i : types)
            this->types.push_back(shared_ptr<Type>(i));
    }

    Type::Type(Types type,
               vector<Type *> types,
               Location loc) :
            Node(loc),
            type(type)
    {
        for (auto i : types)
            this->types.push_back(shared_ptr<Type>(i));
    }

    Type::~Type() { }

    string Program::str() {
        string str;
        for (auto& func: funcs) {
            str += func->str();
        }

        return str;
    }

    string Function::str() {
        string str("def " + id + " : ");

        for (auto &type: types) {
            str += type->str();
            if (type != types.back())
                str += " -> ";
        }

        str += '\n';

        for (auto &cse : cases) {
            str += cse->str() + "\n";
        }

        return str + "\n";
    }

    Function::Function(string str, Type *type, Location loc)
            : Node(loc),
              id(str)
    {
        for (auto i : type->types)
            this->types.push_back(i);
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
        string res = to_string(value);

        if (res.find('.') == res.npos)
            res += ".0";

        return res;
    }

    string Bool::str() {
        return value ? "True" : "False";
    }

    string Char::str() {
        string res = "\'";
        res += value;
        res += "\'";

        return res;
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

    string Type::str() {
        switch (type) {
            case Types::INT:
                return "Int";
            case Types::FLOAT:
                return "Float";
            case Types::BOOL:
                return "Bool";
            case Types::CHAR:
                return "Char";
            case Types::STRING:
                return "String";
            case Types::LIST:
                return "[" + types[0]->str() + "]";
            case Types::TUPLE:
                return "(" + collection_str(*this, ", ") + ")";
            case Types::SIGNATURE:
                return  "(" + collection_str(*this, " -> ") + ")";
            case Types::EMPTYLIST:
                return "[]";
            default:
                throw Error::NotImplemented("");
        }
    }

    string collection_str(Type &node, const std::string split) {
        string str;
        for (auto &type: node.types) {
            str += type->str();
            if (type != node.types.back())
                str += split;
        }

        return str;
    }
}
