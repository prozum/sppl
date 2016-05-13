#include "CCodeGen.h"

using namespace common;
using namespace std;
using namespace codegen;

// Binary Operators
void CCodeGen::visit(Or &Node) {
    ExprStack.top() << "(";
    Node.Left->accept(*this);
    ExprStack.top() << "||";
    Node.Right->accept(*this);
    ExprStack.top() << ")";
}

void CCodeGen::visit(And &Node) {
    ExprStack.top() << "(";
    Node.Left->accept(*this);
    ExprStack.top() << "&&";
    Node.Right->accept(*this);
    ExprStack.top() << ")";
}

void CCodeGen::visit(Equal &Node) {
    outputEqual(Node.Left->RetTy, *Node.Left.get(), *Node.Right.get());
}

void CCodeGen::visit(NotEqual &Node) {
    ExprStack.top() << "(!";
    outputEqual(Node.RetTy, *Node.Left.get(), *Node.Right.get());
    ExprStack.top() << ")";
}

void CCodeGen::visit(Lesser &Node) {
    ExprStack.top() << "(";
    Node.Left->accept(*this);
    ExprStack.top() << "<";
    Node.Right->accept(*this);
    ExprStack.top() << ")";
}

void CCodeGen::visit(LesserEq &Node) {
    ExprStack.top() << "(";
    Node.Left->accept(*this);
    ExprStack.top() << "<=";
    Node.Right->accept(*this);
    ExprStack.top() << ")";
}

void CCodeGen::visit(Greater &Node) {
    ExprStack.top() << "(";
    Node.Left->accept(*this);
    ExprStack.top() << ">";
    Node.Right->accept(*this);
    ExprStack.top() << ")";
}

void CCodeGen::visit(GreaterEq &Node) {
    ExprStack.top() << "(";
    Node.Left->accept(*this);
    ExprStack.top() << ">=";
    Node.Right->accept(*this);
    ExprStack.top() << ")";
}

void CCodeGen::visit(Add &Node) {
    ExprStack.top() << "(";
    Node.Left->accept(*this);
    ExprStack.top() << "+";
    Node.Right->accept(*this);
    ExprStack.top() << ")";
}

void CCodeGen::visit(Sub &Node) {
    ExprStack.top() << "(";
    Node.Left->accept(*this);
    ExprStack.top() << "-";
    Node.Right->accept(*this);
    ExprStack.top() << ")";
}

void CCodeGen::visit(Mul &Node) {
    ExprStack.top() << "(";
    Node.Left->accept(*this);
    ExprStack.top() << "*";
    Node.Right->accept(*this);
    ExprStack.top() << ")";
}

void CCodeGen::visit(Div &Node) {
    ExprStack.top() << "(";
    Node.Left->accept(*this);
    ExprStack.top() << "/";
    Node.Right->accept(*this);
    ExprStack.top() << ")";
}

void CCodeGen::visit(Mod &Node) {
    ExprStack.top() << "(";
    Node.Left->accept(*this);
    ExprStack.top() << "%";
    Node.Right->accept(*this);
    ExprStack.top() << ")";
}

void CCodeGen::visit(ListAdd &Node) {
    string ListName = getList(Node.RetTy);

    // Use pregenerated push function to push thing onto list
    // Look at generate_list(Type &type) for the generation of this function
    ExprStack.top() << GGenerated << GAdd << ListName << "(";
    Node.Right->accept(*this);
    ExprStack.top() << ", ";
    Node.Left->accept(*this);
    ExprStack.top() << ")";
}

void CCodeGen::visit(ProducerConsumer &Node) {
    // TODO
    throw std::runtime_error("Not implemented");
}

void CCodeGen::visit(Concat &Node) {
    string ListName = getList(Node.Left->RetTy);

    ExprStack.top() << GGenerated << GConcat << ListName << "(";
    Node.Left->accept(*this);
    ExprStack.top() << ", ";
    Node.Right->accept(*this);
    ExprStack.top() << ")";
}


// Unary Operators
void CCodeGen::visit(ParExpr &Node) {
    ExprStack.top() << "(";
    Node.Child->accept(*this);
    ExprStack.top() << ")";
}

void CCodeGen::visit(Not &Node) {
    ExprStack.top() << "(!";
    Node.Child->accept(*this);
    ExprStack.top() << ")";
}

void CCodeGen::visit(To &Node) {
    // TODO
    throw std::runtime_error("Not implemented");
}

void CCodeGen::visit(Negative &Node) {
    ExprStack.top() << "(-";
    Node.Child->accept(*this);
    ExprStack.top() << ")";
}


// Literals
void CCodeGen::visit(BoolExpr &Node) {
    ExprStack.top() << Node.Val;
}

void CCodeGen::visit(StringExpr &Node) {
    // gcreate_string is generate by generate_std. It creates string base on
    // a char*
    ExprStack.top() << GGenerated << GCreate << GString << "(" << Node.str() << ")";
}

void CCodeGen::visit(IdExpr &Node) {
    /*
    bool IsDeclared = false;

    for (auto &Decl : Prog->Decls) {
        if (typeid(*Decl.get()) == typeid(Function) &&
            ((Function *)Decl.get())->Id == Node.Val) {
            IsDeclared = true;
            break;
        }
    }

    if (IsDeclared) {
        ExprStack.top() << "&" << GGlobal;
    }
     */

    ExprStack.top() << GUser << Node.Val;
}

void CCodeGen::visit(IntExpr &Node) {
    ExprStack.top() << Node.Val;
}

void CCodeGen::visit(FloatExpr &Node) {
    ExprStack.top() << Node.str();
}

void CCodeGen::visit(CharExpr &Node) {
    ExprStack.top() << Node.str();
}


// Composits
void CCodeGen::visit(TupleExpr &Node) {
    string Name = getTuple(Node.RetTy);

    ExprStack.top() << GGenerated << GCreate << Name << "(";
    for (auto &Element : Node.Elements) {
        Element->accept(*this);

        if (Element != Node.Elements.back())
            ExprStack.top() << ", ";
    }
    ExprStack.top() << ")";
}

void CCodeGen::visit(ListExpr &Node) {
    string Name = getList(Node.RetTy);

    ExprStack.top() << GGenerated << GCreate << Name << "("
    << Node.Elements.size();
    for (size_t i = Node.Elements.size(); i > 0; --i) {
        ExprStack.top() << ", ";

        Node.Elements[i - 1]->accept(*this);
    }

    ExprStack.top() << ")";
}

void CCodeGen::visit(AlgebraicExpr &Node) {
    // TODO
    throw std::runtime_error("Not implemented");
}


// Others
void CCodeGen::visit(CallExpr &Node) {
    Node.Callee->accept(*this);

    ExprStack.top() << "(";
    for (auto &Expr: Node.Args) {
        Expr->accept(*this);

        if (Expr != Node.Args.back())
            ExprStack.top() << ", ";
    }
    ExprStack.top() << ")";

    /*
    string Name = GGenerated + GClosure + to_string(EnvCount++);
    string Assignment =
            getEnvironment(Node.Callee->RetTy) + "* " + Name + " = ";

    ExprStack.push(stringstream());
    // Generate the callee (aka, the function being called)
    Node.Callee->accept(*this);
    *Output << "        " << Assignment << ExprStack.top().str() << "; \n";
    ExprStack.pop();

    // Generate the arguments the function is being called with
    ExprStack.top() << Name << "->call(" << Name;
    for (auto &Arg : Node.Args) {
        ExprStack.top() << ", ";

        Arg->accept(*this);
    }
    ExprStack.top() << ")";
     */
}

void CCodeGen::visit(LambdaArg &Node) {

    // TODO
    throw std::runtime_error("Not implemented");
}

void CCodeGen::visit(LambdaFunction &Node) {

    // TODO
    throw std::runtime_error("Not implemented");
}

void CCodeGen::outputEqual(Type &Ty, Expression &Left, Expression &Right) {
    switch (Ty.Id) {
        case TypeId::TUPLE: {
            string Name = getTuple(Left.RetTy);

            ExprStack.top() << GGenerated << GCompare << Name << "(";
            Left.accept(*this);
            ExprStack.top() << ", ";
            Right.accept(*this);
            ExprStack.top() << ")";
            break;
        }
        case TypeId::LIST:
        case TypeId::STRING: {
            string Name = getList(Left.RetTy);

            ExprStack.top() << GGenerated << GCompare << Name << "(";
            Left.accept(*this);
            ExprStack.top() << ", ";
            Right.accept(*this);
            ExprStack.top() << ")";
            break;
        }
        default:
            ExprStack.top() << "(";
            Left.accept(*this);
            ExprStack.top() << "==";
            Right.accept(*this);
            ExprStack.top() << ")";
            break;
    }
}

void CCodeGen::visit(common::BinPrint &Node) {
    string Name = GTemp + to_string(TempCount++);

    ExprStack.push(stringstream());
    Node.Left->accept(*this);

    BeforeExpr[BeforeExprDepth] += "        " + getType(Node.RetTy) + " " + Name
                                    + " = " + ExprStack.top().str() + "; \n";
    ExprStack.pop();
    ExprStack.top() << Name;

    ExprStack.push(stringstream());
    Node.Right->accept(*this);

    BeforeExpr[BeforeExprDepth] += "        " + Prints[Node.Right->RetTy] + "("
                                   + ExprStack.top().str() + ", 1)" + "; \n";
    ExprStack.pop();
}

void CCodeGen::visit(common::UnPrint &Node) {
    string Name = GTemp + to_string(TempCount++);

    ExprStack.push(stringstream());
    Node.Child->accept(*this);

    BeforeExpr[BeforeExprDepth] += "        " + getType(Node.RetTy) + " " + Name
                                    + " = " + ExprStack.top().str() + "; \n";
    BeforeExpr[BeforeExprDepth] += "        " + Prints[Node.RetTy] + "(" + Name + ", 1)" + "; \n";
    ExprStack.pop();
    ExprStack.top() << Name;
}