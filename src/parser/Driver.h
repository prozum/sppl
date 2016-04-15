#pragma once

#include "Node.h"
#include "Visitor.h"
#include "Scope.h"
#include "Error.h"
#include "Scanner.h"
#include "Parser.h"

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

namespace parser {

    enum class InputType {
        UNKNOWN,
        FILE,
        FILES,
        STRING,
        STREAM,
    };

    class Driver
    {
        public:
            Driver(ostream *out = &cout, ostream *hout = &cout, ostream *mout = &cout);

            InputType input_type;

            // Code, header and message streams
            istream *in;
            ostream *out;
            ostream *hout;
            ostream *mout;

            // File streams
            ifstream fin;
            ofstream fout;
            ofstream fhout;


            Scanner scanner;
            Parser parser;

            bool trace_scanning = false;
            bool trace_parsing = false;
            common::Scope global;
            std::string source;

            bool next_input();
            vector<string> filenames;
            size_t cur_file = 0;

            void set_output(string filename);
            void set_inputs(vector<string> filenames);
            void set_header_output(string filename);

            bool parse_stream(std::istream& in, const std::string& src = "stream input");
            bool parse_string(const std::string& input, const std::string& src = "string stream");
            bool parse_file(const std::string& filename);
            bool parse_files();

            bool accept(common::Visitor &visitor);

            void error(const common::Location &loc, const std::string &msg);
            void error(const std::string &msg);

            unique_ptr<common::Program> program;
    };
}
