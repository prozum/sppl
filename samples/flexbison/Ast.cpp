#include "Ast.h"
#include <iostream>
using namespace std;

void ast::Program::accept(Visitor *v) { v->visit(this); }

void ast::Function::accept(Visitor *v) { v->visit(this); }

void ast::Case::accept(Visitor *v) { v->visit(this); }

void ast::Or::accept(Visitor *v) { v->visit(this); }

void ast::And::accept(Visitor *v) { v->visit(this); }

void ast::Equal::accept(Visitor *v) { v->visit(this); }

void ast::NotEqual::accept(Visitor *v) { v->visit(this); }

void ast::Lesser::accept(Visitor *v) { v->visit(this); }

void ast::Greater::accept(Visitor *v) { v->visit(this); }

void ast::LesserEq::accept(Visitor *v) { v->visit(this); }

void ast::GreaterEq::accept(Visitor *v) { v->visit(this); }

void ast::Add::accept(Visitor *v) { v->visit(this); }

void ast::Sub::accept(Visitor *v) { v->visit(this); }

void ast::Mul::accept(Visitor *v) { v->visit(this); }

void ast::Div::accept(Visitor *v) { v->visit(this); }

void ast::Mod::accept(Visitor *v) { v->visit(this); }

void ast::ListAdd::accept(Visitor *v) { v->visit(this); }

void ast::Par::accept(Visitor *v) { v->visit(this); }

void ast::Not::accept(Visitor *v) { v->visit(this); }

void ast::Int::accept(Visitor *v) { v->visit(this); }

void ast::Float::accept(Visitor *v) { v->visit(this); }

void ast::Bool::accept(Visitor *v) { v->visit(this); }

void ast::Char::accept(Visitor *v) { v->visit(this); }

void ast::String::accept(Visitor *v) { v->visit(this); }

void ast::List::accept(Visitor *v) { v->visit(this); }

void ast::Tuple::accept(Visitor *v) { v->visit(this); }

void ast::Id::accept(Visitor *v) { v->visit(this); }

void ast::Call::accept(Visitor *v) { v->visit(this); }

void ast::IntType::accept(Visitor *v) { v->visit(this); }

void ast::FloatType::accept(Visitor *v) { v->visit(this); }

void ast::BoolType::accept(Visitor *v) { v->visit(this); }

void ast::CharType::accept(Visitor *v) { v->visit(this); }

void ast::StringType::accept(Visitor *v) { v->visit(this); }

void ast::ListType::accept(Visitor *v) { v->visit(this); }

void ast::TupleType::accept(Visitor *v) { v->visit(this); }

void ast::Signature::accept(Visitor *v) { v->visit(this); }

void ast::ListPattern::accept(ast::Visitor *v) { v->visit(this); }

void ast::TuplePattern::accept(ast::Visitor *v) { v->visit(this); }

void ast::ListSplit::accept(ast::Visitor *v) { v->visit(this); }
