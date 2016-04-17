#include "Pattern.h"
#include "Visitor.h"

namespace common {
    void Int::accept(Visitor &V) { V.visit(*this); }
    void Float::accept(Visitor &V) { V.visit(*this); }
    void Bool::accept(Visitor &V) { V.visit(*this); }
    void Char::accept(Visitor &V) { V.visit(*this); }
    void String::accept(Visitor &V) { V.visit(*this); }
    void Id::accept(Visitor &V) { V.visit(*this); }
    void ListPattern::accept(Visitor &V) { V.visit(*this); }
    void TuplePattern::accept(Visitor &V) { V.visit(*this); }
    void ListSplit::accept(Visitor &V) { V.visit(*this); }

    Pattern::Pattern(Location Loc) :
            Expression(Loc) { }

    Pattern::Pattern(Type Ty, Location Loc) :
            Expression(Ty, Loc) { }

    Int::Int(long Val,
             Location Loc) :
            Pattern(Type(TypeId::INT), Loc),
            Val(Val) { }

    Float::Float(double Val,
                 Location Loc) :
            Pattern(Type(TypeId::FLOAT), Loc),
            Val(Val) { }

    Bool::Bool(bool Val,
               Location Loc) :
            Pattern(Type(TypeId::BOOL), Loc),
            Val(Val) { }

    Char::Char(char Val,
               Location Loc) :
            Pattern(Type(TypeId::CHAR), Loc),
            Val(Val) { }

    String::String(string Val,
                   Location Loc) :
            Pattern(Type(TypeId::STRING), Loc),
            Val(Val) { }

    ListPattern::ListPattern(vector<unique_ptr<Pattern>> Patterns,
                             Location Loc) :
            Pattern(Loc),
            Patterns(move(Patterns)) { }

    TuplePattern::TuplePattern(vector<unique_ptr<Pattern>> Patterns,
                               Location Loc) :
            Pattern(Loc),
            Patterns(move(Patterns)) { }

    ListSplit::ListSplit(unique_ptr<Pattern> left,
                         unique_ptr<Pattern> Patterns,
                         Location Loc) :
            Pattern(Loc),
            Left(move(left)),
            Right(move(Patterns)) { }

    List::List(vector<unique_ptr<Expression>> Elements,
               Location Loc) :
            Expression(Type(TypeId::LIST), Loc),
            Elements(move(Elements)) { }

    Tuple::Tuple(vector<unique_ptr<Expression>> Elements,
                 Location Loc) :
            Expression(Type(TypeId::TUPLE), Loc),
            Elements(move(Elements)) { }

    Id::Id(string Val,
           Location Loc) :
            Pattern(Loc),
            Val(Val) { }

    string List::str() {
        return "[" + strJoin(Elements, ", ") + "]";
    }

    string Tuple::str() {
        return "(" + strJoin(Elements, ", ") + ")";
    }

    string Id::str() {
        return Val;
    }

    string Int::str() {
        return to_string(Val);
    }

    string Float::str() {
        string Res = to_string(Val);

        if (Res.find('.') == Res.npos)
            Res += ".0";

        return Res;
    }

    string Bool::str() {
        return Val ? "True" : "False";
    }

    string Char::str() {
        return "\'" + to_string(Val) + "\'";
    }

    string String::str() {
        return "\"" + Val + "\"";
    }

    string ListPattern::str() {
        return "[" + strJoin(Patterns, ", ") + "]";
    }

    string TuplePattern::str() {
        return "(" + strJoin(Patterns, ", ") + ")";
    }

    string ListSplit::str() {
        return "(" + Left->str() + " : " + Right->str() + ")";
    }
}