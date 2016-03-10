#include "Node.h"
using namespace std;

namespace common {

    void Program::accept(Visitor &v) { v.visit(*this); }

    void Function::accept(Visitor &v) { v.visit(*this); }

    void Case::accept(Visitor &v) { v.visit(*this); }

    void Or::accept(Visitor &v) { v.visit(*this); }

    void And::accept(Visitor &v) { v.visit(*this); }

    void Equal::accept(Visitor &v) { v.visit(*this); }

    void NotEqual::accept(Visitor &v) { v.visit(*this); }

    void Lesser::accept(Visitor &v) { v.visit(*this); }

    void Greater::accept(Visitor &v) { v.visit(*this); }

    void LesserEq::accept(Visitor &v) { v.visit(*this); }

    void GreaterEq::accept(Visitor &v) { v.visit(*this); }

    void Add::accept(Visitor &v) { v.visit(*this); }

    void Sub::accept(Visitor &v) { v.visit(*this); }

    void Mul::accept(Visitor &v) { v.visit(*this); }

    void Div::accept(Visitor &v) { v.visit(*this); }

    void Mod::accept(Visitor &v) { v.visit(*this); }

    void ListAdd::accept(Visitor &v) { v.visit(*this); }

    void Par::accept(Visitor &v) { v.visit(*this); }

    void Not::accept(Visitor &v) { v.visit(*this); }

    void Int::accept(Visitor &v) { v.visit(*this); }

    void Float::accept(Visitor &v) { v.visit(*this); }

    void Bool::accept(Visitor &v) { v.visit(*this); }

    void Char::accept(Visitor &v) { v.visit(*this); }

    void String::accept(Visitor &v) { v.visit(*this); }

    void List::accept(Visitor &v) { v.visit(*this); }

    void Tuple::accept(Visitor &v) { v.visit(*this); }

    void Id::accept(Visitor &v) { v.visit(*this); }

    void Call::accept(Visitor &v) { v.visit(*this); }

    void ListPattern::accept(Visitor &v) { v.visit(*this); }

    void TuplePattern::accept(Visitor &v) { v.visit(*this); }

    void ListSplit::accept(Visitor &v) { v.visit(*this); }

    void Type::accept(Visitor &v) { v.visit(*this); }

    bool Type::operator==(const Type &other) const {
        if (this->type == other.type){
            switch (this->type) {
                case LIST:
                    return this->types[0] == other.types[0];
                case SIGNATURE:
                case TUPLE:
                    if (this->types.size() != other.types.size())
                        return false;

                    for (int i = 0; i < this->types.size(); ++i) {
                        if (this->types[i] != other.types[i]){
                            return false;
                        }
                    }
                default:
                    return true;
            }
        } else {
            return false;
        }
    }
}