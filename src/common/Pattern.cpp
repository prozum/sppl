#include "Pattern.h"
#include "Visitor.h"

namespace common {
    void IntPattern::accept(Visitor &V) { V.visit(*this); }
    void FloatPattern::accept(Visitor &V) { V.visit(*this); }
    void CharPattern::accept(Visitor &V) { V.visit(*this); }
    void IdPattern::accept(Visitor &V) { V.visit(*this); }
    void ListPattern::accept(Visitor &V) { V.visit(*this); }
    void TuplePattern::accept(Visitor &V) { V.visit(*this); }
    void ListSplit::accept(Visitor &V) { V.visit(*this); }
    void WildPattern::accept(Visitor &V) { V.visit(*this); }

    Pattern::Pattern(Location Loc) :
            Node(Loc) { }

    Pattern::Pattern(Type Ty, Location Loc) :
            Node(Ty, Loc) { }

    IntPattern::IntPattern(long Val,
                           Location Loc) :
            Pattern(Type(TypeId::INT), Loc),
            Val(Val) { }

    FloatPattern::FloatPattern(double Val,
                               Location Loc) :
            Pattern(Type(TypeId::FLOAT), Loc),
            Val(Val) { }

    CharPattern::CharPattern(char Val,
                             Location Loc) :
            Pattern(Type(TypeId::CHAR), Loc),
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

    IdPattern::IdPattern(string Val,
           Location Loc) :
            Pattern(Loc),
            Val(Val) { }

    WildPattern::WildPattern(Location Loc) :
            Pattern(Loc) { }

    string IdPattern::str() {
        return Val;
    }

    string IntPattern::str() {
        return to_string(Val);
    }

    string FloatPattern::str() {
        string Res = to_string(Val);

        if (Res.find('.') == Res.npos)
            Res += ".0";

        return Res;
    }

    string CharPattern::str() {
        return "\'" + to_string(Val) + "\'";
    }

    string ListPattern::str() {
        return "[" + strJoin(Patterns, ", ") + "]";
    }

    string TuplePattern::str() {
        return "(" + strJoin(Patterns, ", ") + ")";
    }

    string WildPattern::str() {
        return "_";
    }

    string ListSplit::str() {
        return "(" + Left->str() + " : " + Right->str() + ")";
    }
}