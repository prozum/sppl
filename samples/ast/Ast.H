#ifndef AST_HEADER
#define AST_HEADER

/* AST classes */
enum BinOp {
    ADD,
    SUP,
    MUL,
    DIV,
    MOD,
    AND,
    OR,
    EQUAL,
    NOTEQUAL,
    GREATER,
    LESSER,
    GREATEROREQ,
    LESSEROREQ,
    LISTADD
};

enum StdTypes {
    INT,
    FLOAT,
    BOOL,
    CHAR,
    STRING
};

enum StructTypes {
    Signature,
    Tuple
};

enum UnOp {
    NEG,
    PAR,
    NOT
};

class AstVisitor
{
public:
    /* TODO Implement visit for all classes  */
    virtual void visit(AstNode &n) = 0;
};

class AstNode  {
public:
    virtual void accept(AstVisitor &v);
};

template <typename T>
class AstMulti : AstNode {
public:
    std::vector<T> children;
};

class AstProgram : AstMulti<AstFunc> {

};

class AstFunc : AstNode {
public:
    std::string id;
    std::vector<AstType> sig;
    std::vector<AstCase> cases;
};

class AstCase : AstNode {
public:
    std::vector<AstExpr> patterns;
    AstExpr expr;
};

class AstType : AstNode {

};

class AstExpr : AstNode {

};

class AstStdType : AstType {
public:
    StdTypes type;
};

class AstListType : AstType {
public:
    AstType type;
};

class AstStructType : AstType {
public:
    StructTypes type;
    std::vector<AstType> types;
};

class AstBinaryOp : AstExpr {
    BinOp type;
    AstExpr left;
    AstExpr right;
};

class AstUnaryOp : AstExpr {
public:
    UnOp type;
    AstExpr child;
};

class AstInt : AstExpr {
public:
    int value;
};

class AstFloat : AstExpr {
public:
    float value;
};

class AstBool : AstExpr {
public:
    bool value;
};

class AstChar : AstExpr {
public:
    char value;
};

class AstString : AstExpr {
public:
    std::string value;
};

class AstId : AstExpr {
public:
    std::string id;
};

#endif
