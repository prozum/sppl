#include "Driver.h"
#include "Scanner.h"
#include "Parser.h"


namespace parser {



    Driver::Driver(ostream *out, ostream *hout, ostream *mout) :
            out(out),
            hout(hout),
            mout(mout),
            scanner(* this),
            parser(* this),
            global(new common::Scope()) { }

    bool Driver::next_input() {
        if (input_type != InputType::FILES)
            return false;
        if (filenames.size() > cur_file) {
            fin = ifstream(filenames[cur_file++]);
            in = &fin;
            if (!fin.good())
                return false;
            scanner.switch_streams(in, mout);
            return true;
        }
        return false;
    }

    void Driver::set_output(string filename) {
        fout =  ofstream(filename);
        out = &fout;
    }

    void Driver::set_inputs(vector<string> filenames) {
        this->filenames = filenames;
    }

    void Driver::set_header_output(string filename) {
        fhout =  ofstream(filename);
        hout = &fhout;
    }

    bool Driver::parse_stream(std::istream &in, const std::string &sname)
    {
        input_type = InputType::STREAM;
        source = sname;
        scanner.switch_streams(&in, mout);
        scanner.set_debug(trace_scanning);
        parser.set_debug_level(trace_parsing);

        return (parser.parse() == 0);
    }

    bool Driver::parse_file(const std::string &filename)
    {
        input_type = InputType::FILE;

        std::ifstream in(filename.c_str());
        if (!in.good()) return false;

        return parse_stream(in, filename);
    }

    bool Driver::parse_files() {
        input_type = InputType::FILES;

        if (!next_input())
            return false;
        if (parser.parse() != 0)
            return false;

        return true;
    }

    bool Driver::parse_string(const std::string &input, const std::string &sname)
    {
        input_type = InputType::STRING;

        std::istringstream iss(input);

        return parse_stream(iss, sname);
    }

    bool Driver::accept(common::Visitor &visitor)
    {
        program->accept(visitor);

        for (auto &error: visitor.Errors) {
            *mout << error;
        }

        return !visitor.hasError();
    }

    void Driver::error(const common::Location& loc,
                       const std::string &msg)
    {
        *mout << Error(msg, loc);
    }

    void Driver::error(const std::string &msg)
    {
        *mout << Error(msg);
    }
}
