#include "Pattern.h"
#include "Visitor.h"

namespace common {
    void IntPattern::accept(Visitor &V) { V.visit(*this); }
    void FloatPattern::accept(Visitor &V) { V.visit(*this); }
    void CharPattern::accept(Visitor &V) { V.visit(*this); }
    void BoolPattern::accept(Visitor &V) { V.visit(*this); }
    void StringPattern::accept(Visitor &V) { V.visit(*this); }
    void IdPattern::accept(Visitor &V) { V.visit(*this); }
    void ListPattern::accept(Visitor &V) { V.visit(*this); }
    void TuplePattern::accept(Visitor &V) { V.visit(*this); }
    void ListSplit::accept(Visitor &V) { V.visit(*this); }
    void WildPattern::accept(Visitor &V) { V.visit(*this); }
    void AlgebraicPattern::accept(Visitor &V) { V.visit(*this); }
    void ParPattern::accept(Visitor &V) { V.visit(*this); }

    Pattern::Pattern(Type Ty, Location Loc) :
            Node(Loc),
            RetTy(Ty) { }

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

    BoolPattern::BoolPattern(bool Val,
                             Location Loc) :
            Pattern(Type(TypeId::CHAR), Loc),
            Val(Val) { }

    StringPattern::StringPattern(string Val,
                                 Location Loc) :
            Pattern(Type(TypeId::CHAR), Loc),
            Val(Val) { }

    ListPattern::ListPattern(vector<unique_ptr<Pattern>> Patterns,
                             Location Loc) :
            Pattern(Type(TypeId::LIST), Loc),
            Patterns(move(Patterns)) { }

    TuplePattern::TuplePattern(vector<unique_ptr<Pattern>> Patterns,
                               Location Loc) :
            Pattern(Type(TypeId::TUPLE), Loc),
            Patterns(move(Patterns)) { }

    ListSplit::ListSplit(unique_ptr<Pattern> left,
                         unique_ptr<Pattern> Patterns,
                         Location Loc) :
            Pattern(Type(TypeId::UNKNOWN), Loc),
            Left(move(left)),
            Right(move(Patterns)) { }

    AlgebraicPattern::AlgebraicPattern(string Constructor,
                                       vector<unique_ptr<Pattern>> Patterns,
                                       Location Loc) :
            Pattern(Type(TypeId::CUSTOM), Loc),
            Constructor(Constructor),
            Patterns(move(Patterns)) { }

    IdPattern::IdPattern(string Val,
           Location Loc) :
            Pattern(Type(TypeId::UNKNOWN), Loc),
            Val(Val) { }

    WildPattern::WildPattern(Location Loc) :
            Pattern(Type(TypeId::UNKNOWN), Loc) { }

    ParPattern::ParPattern(unique_ptr<Pattern> Pat,
                           Location Loc) :
            Pattern(Type(TypeId::UNKNOWN), Loc),
            Pat(move(Pat)) { }

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
        return string("\'") + Val + "\'";
    }

    string BoolPattern::str() {
        return (Val) ? "True" : "Flase";
    }

    string StringPattern::str() {
        return "\"" + Val + "\"";
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
        return Left->str() + " : " + Right->str();
    }

    string AlgebraicPattern::str() {
        return Constructor + " " + strJoin(Patterns, " ");
    }

    string ParPattern::str() {
        return "(" + Pat->str() + ")";
    }
}