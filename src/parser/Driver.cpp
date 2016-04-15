#include "Driver.h"


namespace parser {

    Driver::Driver(ostream *out, ostream *hout, ostream *mout) :
            out(out),
            hout(hout),
            mout(mout),
            parser(* this),
            trace_scanning(false),
            trace_parsing(false),
            global(new common::Scope()) { }

    bool Driver::parse_stream(std::istream &in, const std::string &sname)
    {
        source = sname; 

        scanner.switch_streams(in, * mout);

        scanner.set_debug(trace_scanning);
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

    void Driver::set_output(string filename) {
        //hout = std::ofstream(filename, std::ofstream::out);
        fout = std::ofstream(filename);
        out = &fout;
    }
}
