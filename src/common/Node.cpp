#include "Node.h"

#include "Visitor.h"

using namespace std;

namespace common {

    void Program::accept(Visitor &V) { V.visit(*this); }
    void Function::accept(Visitor &V) { V.visit(*this); }
    void Case::accept(Visitor &V) { V.visit(*this); }

    void List::accept(Visitor &V) { V.visit(*this); }
    void Tuple::accept(Visitor &V) { V.visit(*this); }
    void Call::accept(Visitor &V) { V.visit(*this); }

    Node::Node(Location Loc) :
            Ty(TypeId::UNKNOWN), Loc(Loc) { }

    Node::Node(Type Ty, Location Loc) :
            Ty(Ty), Loc(Loc) { }

    Expression::Expression(Location Loc) :
            Node(Loc) { }

    Expression::Expression(Type Ty, Location Loc) :
            Node(Ty, Loc) { }


    Program::Program(vector<unique_ptr<Function>> Funcs,
                     Location Loc) :
            Node(Loc),
            Funcs(move(Funcs)) { }

    Program::Program(unique_ptr<Expression> AnonFunc,
                     Location Loc) :
                     Node(Loc) {

        Funcs.push_back(make_unique<Function>(move(AnonFunc)));
    }

    Function::Function(unique_ptr<Expression> AnonFunc) :
            Node(AnonFunc->Loc),
            Id(ANON_FUNC_NAME),
            Signature(Type(TypeId::UNKNOWN)),
            Anon(true) {
        Cases.push_back(make_unique<Case>(move(AnonFunc), vector<unique_ptr<Pattern>>(), AnonFunc->Loc));
    }

    Function::Function(string Id,
                       Type Ty,
                       Location Loc) :
            Node(Ty.Subtypes.front(), Loc),
            Id(Id),
            Signature(Ty) { }

    Case::Case(unique_ptr<Expression> Expr,
               vector<unique_ptr<Pattern>> Patterns,
               Location Loc) :
            Node(Loc),
            Expr(move(Expr)),
            Patterns(move(Patterns)) { }

    Call::Call(unique_ptr<Expression> Callee,
               vector<unique_ptr<Expression>> Args,
               Location Loc) :
            Expression(Loc),
            Callee(move(Callee)),
            Args(move(Args))
    {
    }

    string Program::str() {
        string Str;
        for (auto &Func : Funcs) {
            Str += Func->str();
        }

        return Str;
    }

    string Function::str() {
        string Str("def " + Id + " : ");

        Str += Signature.str() + "\n";

        for (auto &Case : Cases) {
            Str += Case->str() + "\n";
        }

        return Str + "\n";
    }

    string Case::str() {
        return "\t| " + strJoin(Patterns, " ") + " = " + Expr->str();
    }

    string Call::str() {
        return Callee->str() + "(" + strJoin(Args, ", ") + ")";
    }

    template<class T>
	string strJoin(T &List, const std::string JoinStr) {
		string Str("(");
		for (size_t i = 0; i < List.size(); ++i) {
			Str += List[i]->str();
			if (i + 1 != List.size())
				Str += JoinStr;
		}

		return Str + ")";
	}
}
