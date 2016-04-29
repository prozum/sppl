#pragma once

#include "Error.h"
#include "Node.h"
#include "Parser.h"
#include "Scanner.h"
#include "Scope.h"
#include "Visitor.h"

#include <fstream>
#include <ostream>
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
    Driver(std::ostream *out = &std::cout, std::ostream *hout = &std::cout,
           std::ostream *mout = &std::cout);

    SourceType SrcType = SourceType::UNKNOWN;
    std::string Source;

    // Code, header and message streams
    std::istream *In;
    std::ostream *Out;
    std::ostream *HOut;
    std::ostream *MOut;

    // File streams
    std::ifstream FIn;
    std::ofstream FOut;
    std::ofstream FHOut;

    std::unique_ptr<common::Program> Prog;
    Scanner Scr;
    Parser Psr;
    common::Scope Global;

    bool TraceScanning = false;
    bool TraceParsing = false;

    bool nextInput();
    std::vector<std::string> Files;
    size_t CurFile = 0;


    void setOutput(std::string Filename);
    void setHeaderOutput(std::string Filename);

    bool parseStream(std::istream &In, const std::string &Src = "stream");
    bool parseString(const std::string &Input,
                     const std::string &Src = "std::string");
    bool parseFile(const std::string &Filename);
    bool parseFiles(const std::vector<std::string> &Filenames);

    bool accept(common::Visitor &V);

    void error(const common::Location &Loc, const std::string &Msg);
    void error(const std::string &Msg);
    void showError(common::Error Err);

    void addExternFunc(std::string Name, common::Type Signature);
};
}
