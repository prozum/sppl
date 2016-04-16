#include "Driver.h"
#include "Scanner.h"
#include "Parser.h"


namespace parser {



    Driver::Driver(ostream *out, ostream *hout, ostream *mout) :
            Out(out),
            HOut(hout),
            MOut(mout),
            Snr(* this),
            Psr(* this),
            Global(new common::Scope()) { }

    bool Driver::nextInput() {
        if (SrcType != SourceType::FILES)
            return false;
        if (Filenames.size() > CurFile) {
            FIn = ifstream(Filenames[CurFile]);
            In = &FIn;
            if (!FIn.good())
                return false;
            Snr.switch_streams(In, MOut);
            Src = Filenames[CurFile++];
            return true;
        }
        return false;
    }

    void Driver::setOutput(string filename) {
        FOut =  ofstream(filename);
        Out = &FOut;
    }

    void Driver::setInputs(vector<string> filenames) {
        this->Filenames = filenames;
    }

    void Driver::setHeaderOutput(string filename) {
        FHOut =  ofstream(filename);
        HOut = &FHOut;
    }

    bool Driver::parseStream(std::istream &in, const std::string &sname) {
        SrcType = SourceType::STREAM;
        Src = sname;

        Snr.switch_streams(&in, MOut);
        Snr.set_debug(TraceScanning);
        Psr.set_debug_level(TraceParsing);

        return (Psr.parse() == 0);
    }

    bool Driver::parseFile(const std::string &filename) {
        SrcType = SourceType::FILE;
        Src = filename;

        std::ifstream in(filename.c_str());
        if (!in.good()) return false;

        return parseStream(in, filename);
    }

    bool Driver::parseFiles() {
        SrcType = SourceType::FILES;

        if (!nextInput())
            return false;
        if (Psr.parse() != 0)
            return false;

        return true;
    }

    bool Driver::parseString(const std::string &input, const std::string &sname) {
        SrcType = SourceType::STRING;
        Src = sname;

        std::istringstream iss(input);

        return parseStream(iss, sname);
    }

    bool Driver::accept(common::Visitor &visitor) {
        visitor.Errors.clear();

        try {
            Prog->accept(visitor);
        }
        catch (Error err) {
            visitor.Errors.push_back(err);
        }

        for (auto &error : visitor.Errors) {
            showError(error);
        }

        return !visitor.hasError();
    }

    void Driver::error(const common::Location& loc,
                       const std::string &msg) {
        showError(Error(msg, loc));
    }

    void Driver::error(const std::string &msg) {
        showError(Error(msg));
    }

    void Driver::showError(Error err) {
        *MOut << err << endl;
    }
}
