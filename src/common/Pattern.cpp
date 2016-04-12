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
            Pattern(loc),
            value(value) { }

    Float::Float(double value,
                 Location loc) :
            Pattern(loc),
            value(value) { }

    Bool::Bool(bool value,
               Location loc) :
            Pattern(loc),
            value(value) { }

    Char::Char(char value,
               Location loc) :
            Pattern(loc),
            value(value) { }

    String::String(string value,
                   Location loc) :
            Pattern(loc),
            value(value) { }

    ListPattern::ListPattern(vector<unique_ptr<Pattern>> patterns,
                             Location loc) :
            Pattern(loc),
            patterns(move(patterns))
    {
    }

    TuplePattern::TuplePattern(vector<unique_ptr<Pattern>> patterns,
                               Location loc) :
            Pattern(loc),
            patterns(move(patterns))
    {
    }

    ListSplit::ListSplit(unique_ptr<Pattern> left,
                         unique_ptr<Pattern> right,
                         Location loc) :
            Pattern(loc),
            left(move(left)),
            right(move(right)) { }

    List::List(vector<unique_ptr<Expr>> exprs,
               Location loc) :
            Expr(loc),
            exprs(move(exprs)) { }

    Tuple::Tuple(vector<unique_ptr<Expr>> exprs,
                 Location loc) :
            Expr(loc),
            exprs(move(exprs))
    {
    }

    Id::Id(string id,
           Location loc) :
            Pattern(loc),
            id(id) { }
}