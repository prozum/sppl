#include "DataGenerator.h"

using namespace common;
using namespace std;

namespace profiler {
    Pattern *DataGenerator::generate(Types t) {
        switch (t) {
            case INT:
                return new Int(rand());
            case FLOAT:
                return new Float(str(to_string(rand())) + "." + str(to_string(rand())));
            case CHAR:
                return new Char((char)rand()) &c;
            default:
                return NULL;
        }
    }

    Pattern *DataGenerator::generate(Type *t) {

        auto lp = new ListPattern();

        switch (t->node_type->type) {
            case INT:
                for (int i = 0; i < length; ++i) {
                    lp->patterns.push_back(generate(t->node_type->type));
                }
                return lp;
            case FLOAT:
                for (int i = 0; i < length; ++i) {
                    lp->patterns.push_back(generate(t->node_type->type));
                }
                return lp;
            case CHAR:
                for (int i = 0; i < length; ++i) {
                    lp->patterns.push_back(generate(t->node_type->type));
                }
                return lp;
            case LIST:
                length = length/2;
                for (int i = 0; i < length; ++i) {
                    lp->patterns.push_back(generate(t->types[0]->types[0]);
                }
                return lp;
            case TUPLE:
                for (int i = 0; i < length; ++i) {
                    lp->patterns.push_back(generate(&(t->types[0]->types)));
                }
                return lp;
            default:
                return NULL;
        }
    }

    Pattern *DataGenerator::generate(vector<Type *> *ts) {

        auto tp = new TuplePattern();

        for (int i = 0; i < (*ts).size(); ++i) {
            switch ((*ts)[i]->node_type->type) {
                case INT:
                    tp->patterns.push_back(generate((*ts)[i]->node_type->type));
                    break;
                case FLOAT:
                    tp->patterns.push_back(generate((*ts)[i]->node_type->type));
                    break;
                case CHAR:
                    tp->patterns.push_back(generate((*ts)[i]->node_type->type));
                    break;
                case LIST:
                    tp->patterns.push_back(generate((*ts)[i]->types[0]->node_type);
                    break;
                case TUPLE:
                    tp->patterns.push_back(generate(&((*ts)[i]->types)));
            }
        }

        return tp;
    }

    void DataGenerator::dealloc_type(Pattern *p) {
        delete p;
    }

    void DataGenerator::dealloc_type(ListPattern *p) {
        for (int i = 0; i < p->patterns.size(); ++i) {
            if (typeid(p->patterns[i]) == typeid(ListPattern*)) {
                dealloc_type((ListPattern*)p->patterns[i]);
            } else if (typeid(p->patterns[i]) == typeid(TuplePattern*)) {
                dealloc_type((TuplePattern*)p->patterns[i]);
            } else {
                dealloc_type(p->patterns[i]);
            }
        }
    }

    void DataGenerator::dealloc_type(TuplePattern *p) {
        for (int i = 0; i < p->patterns.size(); ++i) {
            if (typeid(p->patterns[i]) == typeid(ListPattern*)) {
                dealloc_type((ListPattern*)p->patterns[i]);
            } else if (typeid(p->patterns[i]) == typeid(TuplePattern*)) {
                dealloc_type((TuplePattern*)p->patterns[i]);
            } else {
                dealloc_type(p->patterns[i]);
            }
        }
    }
}