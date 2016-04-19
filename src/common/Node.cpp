#include "Node.h"
#include "Expression.h"
#include "Visitor.h"

using namespace std;

namespace common {

    void Program::accept(Visitor &V) { V.visit(*this); }
    void Function::accept(Visitor &V) { V.visit(*this); }
    void Case::accept(Visitor &V) { V.visit(*this); }

    Node::Node(Location Loc) :
            RetTy(TypeId::UNKNOWN), Loc(Loc) { }

    Node::Node(Type Ty, Location Loc) :
            RetTy(Ty), Loc(Loc) { }

    Program::Program(vector<unique_ptr<Declaration>> Decls,
                     Location Loc) :
            Node(Loc),
            Decls(move(Decls)) { }

    Program::Program(unique_ptr<Expression> AnonFunc,
                     Location Loc) :
                     Node(Loc) {

        Decls.push_back(make_unique<Function>(move(AnonFunc)));
    }

    Declaration::Declaration(Location Loc) :
            Node(Loc) { }

    Declaration::Declaration(Type Ty, Location Loc) :
            Node(Ty, Loc) { }

    Function::Function(unique_ptr<Expression> AnonFunc) :
            Declaration(AnonFunc->Loc),
            Id(ANON_FUNC_NAME),
            Signature(Type(TypeId::UNKNOWN)),
            Anon(true) {
        //Cases.push_back(make_unique<Case>(move(AnonFunc), vector<unique_ptr<Pattern>>(), AnonFunc->Loc));
    }

    Function::Function(string Id,
                       Type Ty,
                       Location Loc) :
            Declaration(Ty.Subtypes.front(), Loc),
            Id(Id),
            Signature(Ty) { }

    Case::Case(unique_ptr<Expression> Expr,
               unique_ptr<Expression> When,
               vector<unique_ptr<Pattern>> Patterns,
               Location Loc) :
            Node(Loc),
            Expr(move(Expr)),
            When(move(When)),
            Patterns(move(Patterns)) { }

    string Program::str() {
        string Str;
        for (auto &Func : Decls) {
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
