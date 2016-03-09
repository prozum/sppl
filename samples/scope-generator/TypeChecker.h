//
// Created by hejsil on 3/4/16.
//

#ifndef SPPL_TYPECHECKER_H
#define SPPL_TYPECHECKER_H

#include "Visitor.h"
#include <stack>
#include <vector>
#include <Node.h>

using namespace common;
using namespace std;

class TypeChecker : public Visitor {
public:
    bool is_valid = true;

    virtual void visit(Program *node);
    virtual void visit(Function *node);
    virtual void visit(Case *node);
    virtual void visit(Or *node);
    virtual void visit(And *node);
    virtual void visit(Equal *node);
    virtual void visit(NotEqual *node);
    virtual void visit(Lesser *node);
    virtual void visit(Greater *node);
    virtual void visit(LesserEq *node);
    virtual void visit(GreaterEq *node);
    virtual void visit(Add *node);
    virtual void visit(Sub *node);
    virtual void visit(Mul *node);
    virtual void visit(Div *node);
    virtual void visit(Mod *node);
    virtual void visit(ListAdd *node);
    virtual void visit(Par *node);
    virtual void visit(Not *node);
    virtual void visit(Int *node);
    virtual void visit(Float *node);
    virtual void visit(Bool *node);
    virtual void visit(Char *node);
    virtual void visit(String *node);
    virtual void visit(ListPattern *node);
    virtual void visit(TuplePattern *node);
    virtual void visit(ListSplit *node);
    virtual void visit(List *node);
    virtual void visit(Tuple *node);
    virtual void visit(Id *node);
    virtual void visit(Call *node);
    virtual void visit(Type *node);
private:
    stack<vector<Type>> type_stack;
    Function *current_func;

    bool check_types(Type *type1, Type *type2);
    TypeEnum type_cast(TypeEnum type);
    Type *get_type(Id *node, Scope *scope);
};


#endif //SPPL_TYPECHECKER_H
