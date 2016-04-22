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

    unique_ptr<Node> IntPattern::clone() {
        return unique_ptr<Node>((Node*)new IntPattern(Val, Loc));
    }

    unique_ptr<Node> FloatPattern::clone() {
        return unique_ptr<Node>((Node*)new FloatPattern(Val, Loc));
    }

    unique_ptr<Node> CharPattern::clone() {
        return unique_ptr<Node>((Node*)new CharPattern(Val, Loc));
    }

    unique_ptr<Node> BoolPattern::clone() {
        return unique_ptr<Node>((Node*)new BoolPattern(Val, Loc));
    }

    unique_ptr<Node> StringPattern::clone() {
        return unique_ptr<Node>((Node*)new StringPattern(Val, Loc));
    }

    unique_ptr<Node> IdPattern::clone() {
        return unique_ptr<Node>((Node*)new IdPattern(Val, Loc));
    }

    unique_ptr<Node> ListPattern::clone() {
        auto Res = new ListPattern(vector<unique_ptr<Pattern>>(), Loc);

        for (auto &Pat: Patterns) {
            auto Clone = Pat->clone().release();
            Res->Patterns.push_back(unique_ptr<Pattern>((Pattern*)Clone));
        }

        return unique_ptr<Node>((Node*)Res);
    }

    unique_ptr<Node> TuplePattern::clone() {
        auto Res = new TuplePattern(vector<unique_ptr<Pattern>>(), Loc);

        for (auto &Pat: Patterns) {
            auto Clone = Pat->clone().release();
            Res->Patterns.push_back(unique_ptr<Pattern>((Pattern*)Clone));
        }

        return unique_ptr<Node>((Node*)Res);
    }

    unique_ptr<Node> ListSplit::clone() {
        auto NewLeft = Left->clone().release();
        auto NewRight = Right->clone().release();
        auto Res = new ListSplit(unique_ptr<Pattern>((Pattern*)NewLeft), unique_ptr<Pattern>((Pattern*)NewRight), Loc);
        return unique_ptr<Node>((Node*)Res);
    }

    unique_ptr<Node> WildPattern::clone() {
        return unique_ptr<Node>((Node*)new WildPattern(Loc));
    }

    unique_ptr<Node> AlgebraicPattern::clone() {
        auto Res = new AlgebraicPattern(Constructor, vector<unique_ptr<Pattern>>(), Loc);

        for (auto &Pat: Patterns) {
            auto Clone = Pat->clone().release();
            Res->Patterns.push_back(unique_ptr<Pattern>((Pattern*)Clone));
        }

        return unique_ptr<Node>((Node*)Res);
    }

    unique_ptr<Node> ParPattern::clone() {
        auto NewChild = Pat->clone().release();
        return unique_ptr<Node>((Node*)new ParPattern(unique_ptr<Pattern>((Pattern*)NewChild), Loc));
    }


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