#include "Driver.h"
#include "Parser.h"
#include "Scanner.h"

namespace parser {
Driver::Driver(ostream *out, ostream *hout, ostream *mout)
    : Out(out), HOut(hout), MOut(mout), Scr(*this), Psr(*this),
      Global(new common::Scope()) {}

bool Driver::nextInput() {
    if (SrcType != SourceType::FILES)
        return false;
    if (Files.size() > CurFile) {
        FIn = ifstream(Files[CurFile]);
        In = &FIn;
        if (!FIn.good())
            return false;
        Scr.switch_streams(In, MOut);
        Source = Files[CurFile++];
        return true;
    }
    return false;
}

void Driver::setOutput(string Filename) {
    FOut = ofstream(Filename);
    Out = &FOut;
}

void Driver::setHeaderOutput(string Filename) {
    FHOut = ofstream(Filename);
    HOut = &FHOut;
}

bool Driver::parseStream(std::istream &In, const std::string &Src) {
    SrcType = SourceType::STREAM;
    Source = Src;

    Scr.switch_streams(&In, MOut);
    Scr.set_debug(TraceScanning);
    Psr.set_debug_level(TraceParsing);

    return (Psr.parse() == 0);
}

bool Driver::parseFile(const std::string &Filename) {
    SrcType = SourceType::FILE;
    Source = Filename;

    std::ifstream in(Filename.c_str());
    if (!in.good())
        return false;

    return parseStream(in, Filename);
}

bool Driver::parseFiles(const vector<string> &Filenames) {
    SrcType = SourceType::FILES;
    Files = Filenames;

    if (!nextInput())
        return false;
    if (Psr.parse() != 0)
        return false;

    return true;
}

bool Driver::parseString(const std::string &Input, const std::string &Src) {
    SrcType = SourceType::STRING;
    Source = Src;

    std::istringstream iss(Input);

    return parseStream(iss, Src);
}

bool Driver::accept(common::Visitor &V) {
    V.Errors.clear();

    try {
        Prog->accept(V);
    } catch (Error Err) {
        V.Errors.push_back(Err);
    }

    for (auto &Err : V.Errors) {
        showError(Err);
    }

    return !V.hasError();
}

void Driver::error(const common::Location &Loc, const std::string &Msg) {
    showError(Error(Msg, Loc));
}

void Driver::error(const std::string &Msg) { showError(Error(Msg)); }

void Driver::showError(Error Err) { *MOut << Err << endl; }
}
