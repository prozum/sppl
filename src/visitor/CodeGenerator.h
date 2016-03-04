#pragma once

#include <ostream>
#include "Visitor.h"

namespace visitor {

    class CodeGenerator : public Visitor {
    public:
        CodeGenerator(std::ostream &);

    private:
        std::ostream &ostream;
    };

}