#pragma once
#include <memory>
#include <vector>

namespace ctree {
class Node {
public:
    virtual void outputCode(std::ostream &Output, int TapCount = 0) { };
    virtual ~Node() { };
};

class Statement : public Node {
public:
    virtual void outputCode(std::ostream &Output, int TapCount = 0) { };
};

class Expression : public Node {
public:
    virtual void outputCode(std::ostream &Output, int TapCount = 0) { };
};

class Include : public Node {
public:
    std::string Path;

    Include(bool InOutput, std::string Path)
            : Path(Path) {};

    void outputCode(std::ostream &Output, int TapCount = 0);
};

class Block : public Statement {
public:
    std::vector<Statement*> Stmts;

    ~Block() {
        for (auto Stmt: Stmts) {
            delete Stmt;
        }
    }

    void outputCode(std::ostream &Output, int TapCount);
};

class Function : public Node {
public:
    std::string Id;
    std::string ReturnType;
    std::vector<std::pair<std::string, std::string>> Args;
    std::unique_ptr<Block> Blk = nullptr;

    Function(std::string Return, std::string Id,
             Block* Blk)
            : Id(Id), ReturnType(Return),
              Blk(Blk) {};

    void outputCode(std::ostream &Output, int TapCount);
    void outputForward(std::ostream &Output, int TapCount = 0);
};

class Struct : public Node {
public:
    std::string Id;
    std::vector<std::pair<std::string, std::string>> Fields;

    Struct(std::string Id)
            : Id(Id) {};

    void outputCode(std::ostream &Output, int TapCount);
    void outputForward(std::ostream &Output, int TapCount = 0);
};

class Decl : public Statement {
public:
    std::string Type;
    std::unique_ptr<Expression> Expr = nullptr;

    Decl(std::string Type, Expression* Expr)
            : Type(Type), Expr(Expr) {};

    void outputCode(std::ostream &Output, int TapCount);
};

class Program : public Node {
public:
    std::vector<Include*> Includes;
    std::vector<Struct*> Structs;
    std::vector<Function*> Functions;
    std::vector<Decl*> Globals;

    ~Program() {
        for (auto Incl: Includes) {
            delete Incl;
        }
        for (auto Strt: Structs) {
            delete Strt;
        }
        for (auto Func: Functions) {
            delete Func;
        }
        for (auto Global: Globals) {
            delete Global;
        }
    }

    void outputCode(std::ostream &Output, int TapCount);
};

class ExprStmt : public Statement {
public:
    std::unique_ptr<Expression> Expr = nullptr;

    ExprStmt(Expression* Expr) : Expr(Expr) {};

    void outputCode(std::ostream &Output, int TapCount);
};

class IfElse : public Statement {
public:
    bool IsElse;
    std::unique_ptr<Expression> Expr = nullptr;
    std::unique_ptr<Block> Blk = nullptr;

    IfElse(bool IsElse, Expression* Expr,
           Block* Blk)
            : IsElse(IsElse), Expr(Expr),
              Blk(Blk) {};

    void outputCode(std::ostream &Output, int TapCount);
};

class For : public Statement {
public:
    std::unique_ptr<Expression> Expr1 = nullptr;
    std::unique_ptr<Expression> Expr2 = nullptr;
    std::unique_ptr<Expression> Expr3 = nullptr;
    std::unique_ptr<Block> Blk = nullptr;

    For(Expression* Expr1, Expression* Expr2,
        Expression* Expr3, Block* Blk)
            : Expr1(Expr1), Expr2(Expr2),
              Expr3(Expr3), Blk(Blk) {};

    void outputCode(std::ostream &Output, int TapCount);
};

class While : public Statement {
public:
    std::unique_ptr<Expression> Expr = nullptr;
    std::unique_ptr<Block> Blk = nullptr;

    While(Expression* Expr, Block* Blk)
            : Expr(Expr), Blk(Blk) {};

    void outputCode(std::ostream &Output, int TapCount);
};

class Return : public Statement {
public:
    std::unique_ptr<Expression> Expr = nullptr;

    Return(Expression* Expr)
            : Expr(Expr) {};

    void outputCode(std::ostream &Output, int TapCount);
};

class Goto : public Statement {
public:
    std::string Label;

    Goto(std::string Label)
            : Label(Label) {};

    void outputCode(std::ostream &Output, int TapCount);
};

class Label : public Statement {
public:
    std::string Id;

    Label(std::string Id)
            : Id(Id) {};

    void outputCode(std::ostream &Output, int TapCount);
};

class BinOp : public Expression {
public:
    std::string Op;
    std::unique_ptr<Expression> Left = nullptr;
    std::unique_ptr<Expression> Right = nullptr;

    BinOp(std::string Op, Expression* Left,
          Expression* Right)
            : Op(Op), Left(Left),
              Right(Right) {};

    void outputCode(std::ostream &Output, int TapCount);
};

class UnOp : public Expression {
public:
    bool Prefix;
    std::string Op;
    std::unique_ptr<Expression> Child = nullptr;

    UnOp(std::string Op, Expression* Child,
         bool Prefix = true)
            : Prefix(Prefix), Op(Op),
              Child(Child) {};

    void outputCode(std::ostream &Output, int TapCount);
};

class Call : public Expression {
public:
    std::unique_ptr<Expression> Callee = nullptr;
    std::vector<Expression*> Args;

    Call(Expression* Callee)
            : Callee(Callee) {};

    ~Call() {
        for (auto Arg: Args) {
            delete Arg;
        }
    }

    void outputCode(std::ostream &Output, int TapCount);
};

class Int : public Expression {
public:
    long Value;

    Int(int Value) : Value(Value) {};

    void outputCode(std::ostream &Output, int TapCount);
};

class Float : public Expression {
public:
    long double Value;

    Float(long double Value) : Value(Value) {};

    void outputCode(std::ostream &Output, int TapCount);
};

class Char : public Expression {
public:
    char Value;

    Char(char Value) : Value(Value) {};

    void outputCode(std::ostream &Output, int TapCount);
};

class String : public Expression {
public:
    std::string Value;

    String(std::string Value) : Value(Value) {};

    void outputCode(std::ostream &Output, int TapCount);
};

class Ident : public Expression {
public:
    std::string Value;

    Ident(std::string Value) : Value(Value) {};

    void outputCode(std::ostream &Output, int TapCount);
};

class ArrayLiteral : public Expression {
public:
    std::vector<Expression*> Exprs;

    ~ArrayLiteral() {
        for (auto Expr: Exprs) {
            delete Expr;
        }
    }

    void outputCode(std::ostream &Output, int TapCount);
};

}


