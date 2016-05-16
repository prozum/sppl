#include "CCodeGenOld.h"

using namespace common;
using namespace std;
using namespace codegen;

// Binary Operators
void CCodeGenOld::visit(Or &Node) {
    ExprStack.top() << "(";
    Node.Left->accept(*this);
    ExprStack.top() << ") || (";
    Node.Right->accept(*this);
    ExprStack.top() << ")";
}

void CCodeGenOld::visit(And &Node) {
    ExprStack.top() << "(";
    Node.Left->accept(*this);
    ExprStack.top() << ") && (";
    Node.Right->accept(*this);
    ExprStack.top() << ")";
}

void CCodeGenOld::visit(Equal &Node) {
    outputEqual(Node.Left->RetTy, *Node.Left.get(), *Node.Right.get());
}

void CCodeGenOld::visit(NotEqual &Node) {
    ExprStack.top() << "!(";
    outputEqual(Node.RetTy, *Node.Left.get(), *Node.Right.get());
    ExprStack.top() << ")";
}

void CCodeGenOld::visit(Lesser &Node) {
    ExprStack.top() << "(";
    Node.Left->accept(*this);
    ExprStack.top() << ") < (";
    Node.Right->accept(*this);
    ExprStack.top() << ")";
}

void CCodeGenOld::visit(LesserEq &Node) {
    ExprStack.top() << "(";
    Node.Left->accept(*this);
    ExprStack.top() << ") <= (";
    Node.Right->accept(*this);
    ExprStack.top() << ")";
}

void CCodeGenOld::visit(Greater &Node) {
    ExprStack.top() << "(";
    Node.Left->accept(*this);
    ExprStack.top() << ") > (";
    Node.Right->accept(*this);
    ExprStack.top() << ")";
}

void CCodeGenOld::visit(GreaterEq &Node) {
    ExprStack.top() << "(";
    Node.Left->accept(*this);
    ExprStack.top() << ") >= (";
    Node.Right->accept(*this);
    ExprStack.top() << ")";
}

void CCodeGenOld::visit(Add &Node) {
    ExprStack.top() << "(";
    Node.Left->accept(*this);
    ExprStack.top() << ") + (";
    Node.Right->accept(*this);
    ExprStack.top() << ")";
}

void CCodeGenOld::visit(Sub &Node) {
    ExprStack.top() << "(";
    Node.Left->accept(*this);
    ExprStack.top() << ") - (";
    Node.Right->accept(*this);
    ExprStack.top() << ")";
}

void CCodeGenOld::visit(Mul &Node) {
    ExprStack.top() << "(";
    Node.Left->accept(*this);
    ExprStack.top() << ") * (";
    Node.Right->accept(*this);
    ExprStack.top() << ")";
}

void CCodeGenOld::visit(Div &Node) {
    ExprStack.top() << "(";
    Node.Left->accept(*this);
    ExprStack.top() << ") / (";
    Node.Right->accept(*this);
    ExprStack.top() << ")";
}

void CCodeGenOld::visit(Mod &Node) {
    ExprStack.top() << "(";
    Node.Left->accept(*this);
    ExprStack.top() << ") % (";
    Node.Right->accept(*this);
    ExprStack.top() << ")";
}

void CCodeGenOld::visit(ListAdd &Node) {
    string ListName = getList(Node.RetTy);

    // Use pregenerated push function to push thing onto list
    // Look at generate_list(Type &type) for the generation of this function
    ExprStack.top() << GGenerated << GAdd << ListName << "(";
    Node.Right->accept(*this);
    ExprStack.top() << ", ";
    Node.Left->accept(*this);
    ExprStack.top() << ")";
}

void CCodeGenOld::visit(ProducerConsumer &Node) {
    // TODO
    throw std::runtime_error("Not implemented");
}

void CCodeGenOld::visit(Concat &Node) {
    string ListName = getList(Node.Left->RetTy);

    ExprStack.top() << GGenerated << GConcat << ListName << "(";
    Node.Left->accept(*this);
    ExprStack.top() << ", ";
    Node.Right->accept(*this);
    ExprStack.top() << ")";
}


// Unary Operators
void CCodeGenOld::visit(ParExpr &Node) {
    Node.Child->accept(*this);
}

void CCodeGenOld::visit(Not &Node) {
    ExprStack.top() << "!(";
    Node.Child->accept(*this);
    ExprStack.top() << ")";
}

void CCodeGenOld::visit(To &Node) {
    // TODO
    throw std::runtime_error("Not implemented");
}

void CCodeGenOld::visit(Negative &Node) {
    ExprStack.top() << "-(";
    Node.Child->accept(*this);
    ExprStack.top() << ")";
}


// Literals
void CCodeGenOld::visit(BoolExpr &Node) {
    ExprStack.top() << Node.Val;
}

void CCodeGenOld::visit(StringExpr &Node) {
    // gcreate_string is generate by generate_std. It creates string base on
    // a char*
    ExprStack.top() << GGenerated << GCreate << GString << "(" << Node.str() << ")";
}

void CCodeGenOld::visit(IdExpr &Node) {
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

void CCodeGenOld::visit(IntExpr &Node) {
    ExprStack.top() << Node.Val;
}

void CCodeGenOld::visit(FloatExpr &Node) {
    ExprStack.top() << Node.str();
}

void CCodeGenOld::visit(CharExpr &Node) {
    ExprStack.top() << Node.str();
}


// Composits
void CCodeGenOld::visit(TupleExpr &Node) {
    string Name = getTuple(Node.RetTy);

    ExprStack.top() << GGenerated << GCreate << Name << "(";
    for (auto &Element : Node.Elements) {
        Element->accept(*this);

        if (Element != Node.Elements.back())
            ExprStack.top() << ", ";
    }
    ExprStack.top() << ")";
}

void CCodeGenOld::visit(ListExpr &Node) {
    string Name = getList(Node.RetTy);

    ExprStack.top() << GGenerated << GCreate << Name << "("
    << Node.Elements.size();
    for (size_t i = Node.Elements.size(); i > 0; --i) {
        ExprStack.top() << ", ";

        Node.Elements[i - 1]->accept(*this);
    }

    ExprStack.top() << ")";
}

void CCodeGenOld::visit(AlgebraicExpr &Node) {
    // TODO
    throw std::runtime_error("Not implemented");
}


// Others
void CCodeGenOld::visit(CallExpr &Node) {
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

void CCodeGenOld::visit(LambdaArg &Node) {

    // TODO
    throw std::runtime_error("Not implemented");
}

void CCodeGenOld::visit(LambdaFunction &Node) {

    // TODO
    throw std::runtime_error("Not implemented");
}

void CCodeGenOld::outputEqual(Type &Ty, Expression &Left, Expression &Right) {
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
            ExprStack.top() << ") == (";
            Right.accept(*this);
            ExprStack.top() << ")";
            break;
    }
}

void CCodeGenOld::visit(common::UnPrint &Node) {
    ExprStack.top() << Prints[Node.RetTy] << "(";
    Node.Child->accept(*this);
    ExprStack.top() << ", 1)";
}

void CCodeGenOld::visit(common::Assosiate &Node) {
}

void CCodeGenOld::visit(common::DoExpr &Node) {
    string Name = GTemp + to_string(TempCount++);

    for (auto &Expr: Node.Exprs) {
        ExprStack.push(stringstream());
        Expr->accept(*this);

        BeforeExpr[BeforeExprDepth] += "        " + ExprStack.top().str() + "; \n";

        ExprStack.pop();
    }

    ExprStack.push(stringstream());

    Node.ReturnExpr->accept(*this);

    BeforeExpr[BeforeExprDepth] += "        " + getType(Node.RetTy) + " " + Name
                                   + " = " + ExprStack.top().str() + "; \n";

    ExprStack.pop();
    ExprStack.top() << Name;
}