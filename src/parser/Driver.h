#pragma once

#include "Node.h"
#include "Visitor.h"
#include "Scope.h"
#include "Scanner.h"
#include "Error.h"

#include <string>
#include <vector>

namespace parser {

class Driver
{
    public:
        Driver();

        bool trace_scanning;
        bool trace_parsing;
        unique_ptr<common::Scope> global;
        std::string source;

        bool parse_stream(std::istream& in, const std::string& src = "stream input");
        bool parse_string(const std::string& input, const std::string& src = "string stream");
        bool parse_file(const std::string& filename);

        bool accept(common::Visitor &visitor);

        void error(const common::Location &loc, const std::string &msg);
        void error(const std::string &msg);

        class Scanner* lexer;
        unique_ptr<common::Program> program;
};
}
