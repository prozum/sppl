#pragma once

#include "Node.h"
#include "Visitor.h"
#include "Scope.h"
#include "Scanner.h"
#include "Error.h"

#include <string>
#include <vector>
#include <fstream>
#include <sstream>

namespace parser {

    class Driver
    {
        public:
            Driver(ostream *out = &cout, ostream *hout = &cout, ostream *mout = &cout);

            // Code, header and message streams
            ostream *out;
            ostream *hout;
            ostream *mout;

            // File streams
            ofstream fout;
            ofstream fhout;

            Scanner scanner;
            Parser parser;

            bool trace_scanning;
            bool trace_parsing;
            common::Scope global;
            std::string source;

            void set_output(string filename);
            void set_header_output(string filename);

            bool parse_stream(std::istream& in, const std::string& src = "stream input");
            bool parse_string(const std::string& input, const std::string& src = "string stream");
            bool parse_file(const std::string& filename);

            bool accept(common::Visitor &visitor);

            void error(const common::Location &loc, const std::string &msg);
            void error(const std::string &msg);

            unique_ptr<common::Program> program;
    };
}
