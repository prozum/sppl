#include "Driver.h"

#include <fstream>
#include <sstream>
#include <ostream>

namespace parser {

Driver::Driver()
        : trace_scanning(false),
          trace_parsing(false),
          global(new common::Scope())
    {

    }

    bool Driver::parse_stream(std::istream &in, const std::string &sname)
    {
        source = sname;

        Scanner scanner(&in);
        scanner.set_debug(trace_scanning);
        this->lexer = &scanner;

        Parser parser(*this);
        parser.set_debug_level(trace_parsing);
        return (parser.parse() == 0);
    }

    bool Driver::parse_file(const std::string &filename)
    {
        std::ifstream in(filename.c_str());
        if (!in.good()) return false;
        return parse_stream(in, filename);
    }

    bool Driver::parse_string(const std::string &input, const std::string &sname)
    {
        std::istringstream iss(input);
        return parse_stream(iss, sname);
    }

    bool Driver::accept(common::Visitor &visitor)
    {
        program->accept(visitor);

        return !visitor.HasError();
    }

    void Driver::error(const common::Location& loc,
                       const std::string &msg)
    {
        std::cerr << loc << msg << std::endl;
    }

    void Driver::error(const std::string &msg)
    {
        std::cerr << msg << std::endl;
    }
}
