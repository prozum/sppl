#pragma once

#include <ostream>
#include "Visitor.h"

namespace visitor {

    class CodeGenerator : public common::Visitor {
    public:
        CodeGenerator(std::ostream &);

    private:
        std::ostream &ostream;
    };

}