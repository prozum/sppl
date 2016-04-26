#include "CppCodeGenerator.h"

using namespace common;
using namespace std;
using namespace codegen;

// Binary Operators
void CCodeGenerator::visit(Or &Node) {
    ExprStack.top() << "(";
    Node.Left->accept(*this);
    ExprStack.top() << "||";
    Node.Right->accept(*this);
    ExprStack.top() << ")";
}

void CCodeGenerator::visit(And &Node) {
    ExprStack.top() << "(";
    Node.Left->accept(*this);
    ExprStack.top() << "&&";
    Node.Right->accept(*this);
    ExprStack.top() << ")";
}

void CCodeGenerator::visit(Equal &Node) {
    outputEqual(Node.RetTy, *Node.Left.get(), *Node.Right.get());
}

void CCodeGenerator::visit(NotEqual &Node) {
    ExprStack.top() << "(!";
    outputEqual(Node.RetTy, *Node.Left.get(), *Node.Right.get());
    ExprStack.top() << ")";
}

void CCodeGenerator::visit(Lesser &Node) {
    ExprStack.top() << "(";
    Node.Left->accept(*this);
    ExprStack.top() << "<";
    Node.Right->accept(*this);
    ExprStack.top() << ")";
}

void CCodeGenerator::visit(LesserEq &Node) {
    ExprStack.top() << "(";
    Node.Left->accept(*this);
    ExprStack.top() << "<=";
    Node.Right->accept(*this);
    ExprStack.top() << ")";
}

void CCodeGenerator::visit(Greater &Node) {
    ExprStack.top() << "(";
    Node.Left->accept(*this);
    ExprStack.top() << ">";
    Node.Right->accept(*this);
    ExprStack.top() << ")";
}

void CCodeGenerator::visit(GreaterEq &Node) {
    ExprStack.top() << "(";
    Node.Left->accept(*this);
    ExprStack.top() << ">=";
    Node.Right->accept(*this);
    ExprStack.top() << ")";
}

void CCodeGenerator::visit(Add &Node) {
    ExprStack.top() << "(";
    Node.Left->accept(*this);
    ExprStack.top() << "+";
    Node.Right->accept(*this);
    ExprStack.top() << ")";
}

void CCodeGenerator::visit(Sub &Node) {
    ExprStack.top() << "(";
    Node.Left->accept(*this);
    ExprStack.top() << "-";
    Node.Right->accept(*this);
    ExprStack.top() << ")";
}

void CCodeGenerator::visit(Mul &Node) {
    ExprStack.top() << "(";
    Node.Left->accept(*this);
    ExprStack.top() << "*";
    Node.Right->accept(*this);
    ExprStack.top() << ")";
}

void CCodeGenerator::visit(Div &Node) {
    ExprStack.top() << "(";
    Node.Left->accept(*this);
    ExprStack.top() << "/";
    Node.Right->accept(*this);
    ExprStack.top() << ")";
}

void CCodeGenerator::visit(Mod &Node) {
    ExprStack.top() << "(";
    Node.Left->accept(*this);
    ExprStack.top() << "%";
    Node.Right->accept(*this);
    ExprStack.top() << ")";
}

void CCodeGenerator::visit(ListAdd &Node) {
    string Name = getList(Node.RetTy);

    // Use pregenerated push function to push thing onto list
    // Look at generate_list(Type &type) for the generation of this function
    ExprStack.top() << GGenerated << GAdd << Name << "(";
    Node.Right->accept(*this);
    ExprStack.top() << ", ";
    Node.Left->accept(*this);
    ExprStack.top() << ")";
}

void CCodeGenerator::visit(ProducerConsumer &Node) {
    // TODO
    throw std::runtime_error("Not implemented");
}

void CCodeGenerator::visit(Concat &Node) {
    // TODO
    throw std::runtime_error("Not implemented");
}


// Unary Operators
void CCodeGenerator::visit(ParExpr &Node) {
    ExprStack.top() << "(";
    Node.Child->accept(*this);
    ExprStack.top() << ")";
}

void CCodeGenerator::visit(Not &Node) {
    ExprStack.top() << "(!";
    Node.Child->accept(*this);
    ExprStack.top() << ")";
}

void CCodeGenerator::visit(To &Node) {
    // TODO
    throw std::runtime_error("Not implemented");
}

void CCodeGenerator::visit(Negative &Node) {
    ExprStack.top() << "(-";
    Node.Child->accept(*this);
    ExprStack.top() << ")";
}


// Literals
void CCodeGenerator::visit(BoolExpr &Node) {
    ExprStack.top() << Node.Val;
}

void CCodeGenerator::visit(StringExpr &Node) {
    // gcreate_string is generate by generate_std. It creates string base on
    // a char*
    ExprStack.top() << GGenerated << GCreate << GString << "(" << Node.str()
                    << ")";
}

void CCodeGenerator::visit(IdExpr &Node) {
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

    ExprStack.top() << GUser << Node.Val;
}

void CCodeGenerator::visit(IntExpr &Node) {
    ExprStack.top() << Node.Val;
}

void CCodeGenerator::visit(FloatExpr &Node) {
    ExprStack.top() << Node.str();
}

void CCodeGenerator::visit(CharExpr &Node) {
    ExprStack.top() << Node.str();
}


// Composits
void CCodeGenerator::visit(TupleExpr &Node) {
    string Name = getTuple(Node.RetTy);

    // Create tuple
    ExprStack.top() << GGenerated << GCreate << Name << "(";
    for (auto &Element : Node.Elements) {
        Element->accept(*this);

        if (Element != Node.Elements.back())
            ExprStack.top() << ", ";
    }
    ExprStack.top() << ")";
}

void CCodeGenerator::visit(ListExpr &Node) {
    string Name = getList(Node.RetTy);

    // Create list
    ExprStack.top() << GGenerated << GCreate << Name << "("
    << Node.Elements.size();
    for (size_t i = Node.Elements.size(); i > 0; --i) {
        ExprStack.top() << ", ";

        Node.Elements[i - 1]->accept(*this);
    }

    ExprStack.top() << ")";
}

void CCodeGenerator::visit(AlgebraicExpr &Node) {

    // TODO
    throw std::runtime_error("Not implemented");
}


// Others
void CCodeGenerator::visit(CallExpr &Node) {
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
}

void CCodeGenerator::visit(LambdaArg &Node) {

    // TODO
    throw std::runtime_error("Not implemented");
}

void CCodeGenerator::visit(LambdaFunction &Node) {

    // TODO
    throw std::runtime_error("Not implemented");
}

void CCodeGenerator::outputEqual(Type &Ty, Expression &Left,
                                 Expression &Right) {
    string Name;

    switch (Ty.Id) {
        case TypeId::TUPLE:
            Name = getTuple(Left.RetTy);

            ExprStack.top() << GGenerated << GCompare << Name << "(";
            Left.accept(*this);
            ExprStack.top() << ", ";
            Right.accept(*this);
            ExprStack.top() << ")";
            break;
        case TypeId::LIST:
        case TypeId::STRING:
            Name = getList(Left.RetTy);

            ExprStack.top() << GGenerated << GCompare << Name << "(";
            Left.accept(*this);
            ExprStack.top() << ", ";
            Right.accept(*this);
            ExprStack.top() << ")";
            break;
        default:
            ExprStack.top() << "(";
            Left.accept(*this);
            ExprStack.top() << "==";
            Right.accept(*this);
            ExprStack.top() << ")";
            break;
    }
}