#pragma once

#include <string>
#include <vector>

#include "Node.h"

namespace parser {

class Driver
{
    public:
        Driver();

        bool trace_scanning;
        bool trace_parsing;

        std::string source;
        bool parse_stream(std::istream& in,
                          const std::string& src = "stream input");
        bool parse_string(const std::string& input,
                          const std::string& src = "string stream");
        bool parse_file(const std::string& filename);

        void error(const common::Location &loc, const std::string &msg);
        void error(const std::string &msg);

        class Scanner* lexer;
        common::Program *program;
        common::Program *global_scope;

    void accept(common::Visitor &visitor);
};
}
