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
            Expr(loc) { }

    Pattern::Pattern(Type type, Location loc) :
            Expr(type, loc) { }

    Int::Int(long value,
             Location loc) :
            Pattern(Type(TypeId::INT), loc),
            value(value) { }

    Float::Float(double value,
                 Location loc) :
            Pattern(Type(TypeId::FLOAT), loc),
            value(value) { }

    Bool::Bool(bool value,
               Location loc) :
            Pattern(Type(TypeId::BOOL), loc),
            value(value) { }

    Char::Char(char value,
               Location loc) :
            Pattern(Type(TypeId::CHAR), loc),
            value(value) { }

    String::String(string value,
                   Location loc) :
            Pattern(Type(TypeId::STRING), loc),
            value(value) { }

    ListPattern::ListPattern(vector<unique_ptr<Pattern>> patterns,
                             Location loc) :
            Pattern(loc),
            patterns(move(patterns)) { }

    TuplePattern::TuplePattern(vector<unique_ptr<Pattern>> patterns,
                               Location loc) :
            Pattern(loc),
            patterns(move(patterns)) { }

    ListSplit::ListSplit(unique_ptr<Pattern> left,
                         unique_ptr<Pattern> right,
                         Location loc) :
            Pattern(loc),
            left(move(left)),
            right(move(right)) { }

    List::List(vector<unique_ptr<Expr>> exprs,
               Location loc) :
            Expr(Type(TypeId::LIST), loc),
            exprs(move(exprs)) { }

    Tuple::Tuple(vector<unique_ptr<Expr>> exprs,
                 Location loc) :
            Expr(Type(TypeId::TUPLE), loc),
            exprs(move(exprs)) { }

    Id::Id(string id,
           Location loc) :
            Pattern(loc),
            id(id) { }

    string List::str() {
        string str("[");

        for (auto i = 0; i < exprs.size(); i++) {
            str += exprs[i]->str();
            if (i + 1 != exprs.size())
                str += ", ";
        }

        return str + "]";
    }

    string Tuple::str() {
        string str("(");

        for (auto i = 0; i < exprs.size(); i++) {
            str += exprs[i]->str();
            if (i + 1 != exprs.size())
                str += ", ";
        }

        return str + ")";
    }

    string Id::str() {
        return id;
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

    string ListPattern::str() {
        string str("[");

        for (auto i = 0; i < patterns.size(); i++) {
            str += patterns[i]->str();
            if (i + 1 != patterns.size())
                str += ", ";
        }

        return str + "]";
    }

    string TuplePattern::str() {
        string str("(");

        for (auto i = 0; i < patterns.size(); i++) {
            str += patterns[i]->str();
            if (i + 1 != patterns.size())
                str += ", ";
        }

        return str + ")";
    }

    string ListSplit::str() {
        return "(" + left->str() + " : " + right->str() + ")";
    }
}