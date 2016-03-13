#pragma once

#include <Node.h>

namespace profiler {
    class DataGenerator {
    public:
        Pattern *generate(Types t);

        Pattern *generate(Type *t);

        Pattern *generate(vector<Type *> *ts);

        void dealloc_type(ListPattern *p);

        void dealloc_type(TuplePattern *p);

        void dealloc_type(Pattern *p);

        // length of list that should be generated
        int length;
    };
}
