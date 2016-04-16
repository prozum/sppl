#include "Pattern.h"
#include "Visitor.h"

namespace common {
    void Int::accept(Visitor &v) { v.visit(*this); }
    void Float::accept(Visitor &v) { v.visit(*this); }
    void Bool::accept(Visitor &v) { v.visit(*this); }
    void Char::accept(Visitor &v) { v.visit(*this); }
    void String::accept(Visitor &v) { v.visit(*this); }
    void Id::accept(Visitor &v) { v.visit(*this); }
    void ListPattern::accept(Visitor &v) { v.visit(*this); }
    void TuplePattern::accept(Visitor &v) { v.visit(*this); }
    void ListSplit::accept(Visitor &v) { v.visit(*this); }

    Pattern::Pattern(Location loc) :
            Expression(loc) { }

    Pattern::Pattern(Type type, Location loc) :
            Expression(type, loc) { }

    Int::Int(long value,
             Location loc) :
            Pattern(Type(TypeId::INT), loc),
            Val(value) { }

    Float::Float(double value,
                 Location loc) :
            Pattern(Type(TypeId::FLOAT), loc),
            Val(value) { }

    Bool::Bool(bool value,
               Location loc) :
            Pattern(Type(TypeId::BOOL), loc),
            Val(value) { }

    Char::Char(char value,
               Location loc) :
            Pattern(Type(TypeId::CHAR), loc),
            Val(value) { }

    String::String(string value,
                   Location loc) :
            Pattern(Type(TypeId::STRING), loc),
            Val(value) { }

    ListPattern::ListPattern(vector<unique_ptr<Pattern>> patterns,
                             Location loc) :
            Pattern(loc),
            Patterns(move(patterns)) { }

    TuplePattern::TuplePattern(vector<unique_ptr<Pattern>> patterns,
                               Location loc) :
            Pattern(loc),
            Patterns(move(patterns)) { }

    ListSplit::ListSplit(unique_ptr<Pattern> left,
                         unique_ptr<Pattern> right,
                         Location loc) :
            Pattern(loc),
            Left(move(left)),
            Right(move(right)) { }

    List::List(vector<unique_ptr<Expression>> exprs,
               Location loc) :
            Expression(Type(TypeId::LIST), loc),
            Elements(move(exprs)) { }

    Tuple::Tuple(vector<unique_ptr<Expression>> exprs,
                 Location loc) :
            Expression(Type(TypeId::TUPLE), loc),
            Elements(move(exprs)) { }

    Id::Id(string id,
           Location loc) :
            Pattern(loc),
            Val(id) { }

    string List::str() {
        string str("[");

        for (size_t i = 0; i < Elements.size(); i++) {
            str += Elements[i]->str();
            if (i + 1 != Elements.size())
                str += ", ";
        }

        return str + "]";
    }

    string Tuple::str() {
        string str("(");

        for (size_t i = 0; i < Elements.size(); i++) {
            str += Elements[i]->str();
            if (i + 1 != Elements.size())
                str += ", ";
        }

        return str + ")";
    }

    string Id::str() {
        return Val;
    }

    string Int::str() {
        return to_string(Val);
    }

    string Float::str() {
        string res = to_string(Val);

        if (res.find('.') == res.npos)
            res += ".0";

        return res;
    }

    string Bool::str() {
        return Val ? "True" : "False";
    }

    string Char::str() {
        string res = "\'";
        res += Val;
        res += "\'";

        return res;
    }

    string String::str() {
        return "\"" + Val + "\"";
    }

    string ListPattern::str() {
        string str("[");

        for (size_t i = 0; i < Patterns.size(); i++) {
            str += Patterns[i]->str();
            if (i + 1 != Patterns.size())
                str += ", ";
        }

        return str + "]";
    }

    string TuplePattern::str() {
        string str("(");

        for (size_t i = 0; i < Patterns.size(); i++) {
            str += Patterns[i]->str();
            if (i + 1 != Patterns.size())
                str += ", ";
        }

        return str + ")";
    }

    string ListSplit::str() {
        return "(" + Left->str() + " : " + Right->str() + ")";
    }
}