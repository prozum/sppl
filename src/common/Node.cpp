#include "Node.h"
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
                        if (!(*this->types[i] == *other.types[i])){
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

    Node::Node() :
            line_no(0) { }

    Node::Node(int line_no) :
            line_no(line_no) { }

    Expr::Expr() { }

    Expr::Expr(int line_no) :
            Node(line_no) { }

    BinaryOp::BinaryOp() { }

    BinaryOp::BinaryOp(Expr *left,
                       Expr *right) :
            left(left),
            right(right) { }

    BinaryOp::BinaryOp(Expr *left,
                       Expr *right,
                       int line_no) :
            Expr(line_no),
            left(left),
            right(right) { }

    UnaryOp::UnaryOp() { }

    UnaryOp::UnaryOp(Expr *child) :
            child(child) { }

    UnaryOp::UnaryOp(Expr *child,
                     int line_no) :
            Expr(line_no),
            child(child) { }

    Pattern::Pattern() { }

    Pattern::Pattern(int line_no) :
            Expr(line_no) { }

    Program::Program() { }

    Program::Program(vector<Function *> funcs) :
            funcs(funcs) { }

    Program::Program(vector<Function *> funcs,
                     int line_no) :
            Node(line_no),
            funcs(funcs) { }

    Program::Program(string id,
                       Expr *expr,
                       int line_no) :
            Node(line_no),
            funcs() {

        funcs.push_back(new Function(id, vector<Type *>(), vector<Case *>()));
        funcs[0]->cases.push_back(new Case(expr, vector<Pattern *>()));
    }

    Function::Function() { }

    Function::Function(string id) :
            id(id) { }

    Function::Function(string id,
                       vector<Type *> types) :
            id(id),
            types(types) { }

    Function::Function(string id,
                       vector<Type *> types,
                       int line_no) :
            Node(line_no),
            id(id),
            types(types) { }

    Function::Function(string id,
                       vector<Type *> types,
                       vector<Case *> cases) :
            id(id),
            types(types),
            cases(cases) { }

    Function::Function(string id,
                       vector<Type *> types,
                       vector<Case *> cases,
                       int line_no) :
            Node(line_no),
            id(id),
            types(types),
            cases(cases) { }

    Case::Case() { }

    Case::Case(Expr *expr) : expr(expr) { }

    Case::Case(Expr *expr,
               vector<Pattern *> patterns) :
            expr(expr),
            patterns(patterns) { }

    Case::Case(Expr *expr,
               vector<Pattern *> patterns,
               int line_no) :
            Node(line_no),
            expr(expr),
            patterns(patterns) { }

    Or::Or() { }

    Or::Or(Expr *left, 
           Expr *right) : 
            BinaryOp(left, right) { }

    Or::Or(Expr *left, 
           Expr *right, 
           int line_no) : 
            BinaryOp(left, right, line_no) { }

    And::And() { }

    And::And(Expr *left, 
             Expr *right) :
            BinaryOp(left, right) { }

    And::And(Expr *left, 
             Expr *right, 
             int line_no) :
            BinaryOp(left, right, line_no) { }

    Equal::Equal() { }

    Equal::Equal(Expr *left, 
                 Expr *right) :
            BinaryOp(left, right) { }

    Equal::Equal(Expr *left, 
                 Expr *right, 
                 int line_no) :
            BinaryOp(left, right, line_no) { }

    NotEqual::NotEqual() { }

    NotEqual::NotEqual(Expr *left, 
                       Expr *right) :
            BinaryOp(left, right) { }

    NotEqual::NotEqual(Expr *left, 
                       Expr *right, 
                       int line_no) :
            BinaryOp(left, right, line_no) { }

    Lesser::Lesser() { }

    Lesser::Lesser(Expr *left, 
                   Expr *right) :
            BinaryOp(left, right) { }

    Lesser::Lesser(Expr *left, 
                   Expr *right, 
                   int line_no) :
            BinaryOp(left, right, line_no) { }

    Greater::Greater() { }

    Greater::Greater(Expr *left, 
                     Expr *right) :
            BinaryOp(left, right) { }

    Greater::Greater(Expr *left, 
                     Expr *right, 
                     int line_no) :
            BinaryOp(left, right, line_no) { }

    LesserEq::LesserEq() { }

    LesserEq::LesserEq(Expr *left, 
                       Expr *right) :
            BinaryOp(left, right) { }

    LesserEq::LesserEq(Expr *left, 
                       Expr *right, 
                       int line_no) :
            BinaryOp(left, right, line_no) { }

    GreaterEq::GreaterEq() { }

    GreaterEq::GreaterEq(Expr *left, 
                         Expr *right) :
            BinaryOp(left, right) { }

    GreaterEq::GreaterEq(Expr *left, 
                         Expr *right, 
                         int line_no) :
            BinaryOp(left, right, line_no) { }

    Add::Add() { }

    Add::Add(Expr *left, 
             Expr *right) :
            BinaryOp(left, right) { }

    Add::Add(Expr *left, 
             Expr *right, 
             int line_no) :
            BinaryOp(left, right, line_no) { }

    Sub::Sub() { }

    Sub::Sub(Expr *left, 
             Expr *right) :
            BinaryOp(left, right) { }

    Sub::Sub(Expr *left, 
             Expr *right, 
             int line_no) :
            BinaryOp(left, right, line_no) { }

    Mul::Mul() { }

    Mul::Mul(Expr *left, 
             Expr *right) :
            BinaryOp(left, right) { }

    Mul::Mul(Expr *left, 
             Expr *right, 
             int line_no) :
            BinaryOp(left, right, line_no) { }

    Div::Div() { }

    Div::Div(Expr *left, 
             Expr *right) :
            BinaryOp(left, right) { }

    Div::Div(Expr *left, 
             Expr *right, 
             int line_no) :
            BinaryOp(left, right, line_no) { }

    Mod::Mod() { }

    Mod::Mod(Expr *left, 
             Expr *right) :
            BinaryOp(left, right) { }

    Mod::Mod(Expr *left, 
             Expr *right, 
             int line_no) :
            BinaryOp(left, right, line_no) { }

    ListAdd::ListAdd() { }

    ListAdd::ListAdd(Expr *left, 
                     Expr *right) :
            BinaryOp(left, right) { }

    ListAdd::ListAdd(Expr *left, 
                     Expr *right, 
                     int line_no) :
            BinaryOp(left, right, line_no) { }
    
    Par::Par() { }

    Par::Par(Expr *child) : 
            UnaryOp(child) { }

    Par::Par(Expr *child, 
             int line_no) : 
            UnaryOp(child, line_no) { }

    Not::Not() { }

    Not::Not(Expr *child) :
            UnaryOp(child) { }

    Not::Not(Expr *child, 
             int line_no) :
            UnaryOp(child, line_no) { }

    Int::Int(long value) : 
            value(value) { }

    Int::Int(long value, 
             int line_no) : 
            Pattern(line_no),
            value(value) { }

    Float::Float(double value) :
            value(value) { }

    Float::Float(double value,
                 int line_no) :
            Pattern(line_no),
            value(value) { }

    Bool::Bool(bool value) :
            value(value) { }

    Bool::Bool(bool value,
               int line_no) :
            Pattern(line_no),
            value(value) { }

    Char::Char(char value) :
            value(value) { }

    Char::Char(char value,
               int line_no) :
            Pattern(line_no),
            value(value) { }

    String::String(string value) :
            value(value) { }

    String::String(string value,
                   int line_no) :
            Pattern(line_no),
            value(value) { }

    ListPattern::ListPattern() { }

    ListPattern::ListPattern(vector<Pattern *> patterns)
            : patterns(patterns) { }

    ListPattern::ListPattern(vector<Pattern *> patterns,
                             int line_no) :
            Pattern(line_no),
            patterns(patterns) { }

    TuplePattern::TuplePattern() { }

    TuplePattern::TuplePattern(vector<Pattern *> patterns)
            : patterns(patterns) { }

    TuplePattern::TuplePattern(vector<Pattern *> patterns,
                               int line_no) :
            Pattern(line_no),
            patterns(patterns) { }

    ListSplit::ListSplit() { }

    ListSplit::ListSplit(Pattern *left,
                         Pattern *right) :
            left(left),
            right(right) { }

    ListSplit::ListSplit(Pattern *left,
                         Pattern *right,
                         int line_no) :
            Pattern(line_no),
            left(left),
            right(right) { }

    List::List() { }

    List::List(vector<Expr *> exprs) :
            exprs(exprs) { }

    List::List(vector<Expr *> exprs,
               int line_no) :
            Expr(line_no),
            exprs(exprs) { }

    Tuple::Tuple() { }

    Tuple::Tuple(vector<Expr *> exprs) :
            exprs(exprs) { }

    Tuple::Tuple(vector<Expr *> exprs,
                 int line_no) :
            Expr(line_no),
            exprs(exprs) { }

    Id::Id() { }

    Id::Id(string id) :
            id(id) { }

    Id::Id(string id,
           int line_no) :
            Pattern(line_no),
            id(id) { }

    Call::Call() { }

    Call::Call(Expr *callee) :
            callee(callee) { }

    Call::Call(Expr *callee,
               vector<Expr *> exprs) :
            callee(callee),
            exprs(exprs) { }

    Call::Call(Expr *callee,
               vector<Expr *> exprs,
               int line_no) :
            Expr(line_no),
            callee(callee),
            exprs(exprs) { }

    Type::Type() { }

    Type::Type(Types type) :
            type(type) { }

    Type::Type(Types type,
               int line_no) :
            Node(line_no),
            type(type) { }

    Type::Type(Types type,
               vector<Type *> types) :
            type(type),
            types(types) { }

    Type::Type(Types type,
               vector<Type *> types,
               int line_no) :
            Node(line_no),
            type(type),
            types(types) { }

    Type::~Type() { }
}