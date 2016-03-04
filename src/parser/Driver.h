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

    std::string streamname;

    bool parse_stream(std::istream& in,
                      const std::string& sname = "stream input");

    bool parse_string(const std::string& input,
                      const std::string& sname = "string stream");

    bool parse_file(const std::string& filename);

    void error(const class location& l, const std::string& m);

    void error(const std::string& m);

    class Scanner* lexer;

    ast::Program *main;
};

}
