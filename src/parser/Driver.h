#pragma once

#include "Error.h"
#include "Node.h"
#include "Parser.h"
#include "Scanner.h"
#include "Scope.h"
#include "Visitor.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace parser {

enum class SourceType {
    UNKNOWN,
    FILE,
    FILES,
    STRING,
    STREAM,
};

class Driver {
  public:
    Driver(ostream *out = &cout, ostream *hout = &cout, ostream *mout = &cout);

    SourceType SrcType = SourceType::UNKNOWN;
    std::string Source;

    // Code, header and message streams
    istream *In;
    ostream *Out;
    ostream *HOut;
    ostream *MOut;

    // File streams
    ifstream FIn;
    ofstream FOut;
    ofstream FHOut;

    unique_ptr<common::Program> Prog;
    Scanner Scr;
    Parser Psr;
    common::Scope Global;

    bool TraceScanning = false;
    bool TraceParsing = false;

    bool nextInput();
    vector<string> Files;
    size_t CurFile = 0;

    void setOutput(string Filename);
    void setHeaderOutput(string Filename);

    bool parseStream(std::istream &In, const std::string &Src = "stream");
    bool parseString(const std::string &Input,
                     const std::string &Src = "string");
    bool parseFile(const std::string &Filename);
    bool parseFiles(const vector<std::string> &Filenames);

    bool accept(common::Visitor &V);

    void error(const common::Location &Loc, const std::string &Msg);
    void error(const std::string &Msg);
    void showError(Error Err);
};
}
