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

    // Parsing classes
    Scanner Scr;
    Parser Psr;
    std::unique_ptr<common::Program> Prog;
    common::Scope Global;

    // Input options
    std::vector<std::string> InFiles;
    size_t CurInFile = 0;
    SourceType SrcType = SourceType::UNKNOWN;
    std::string Source;

    // Out options
    bool Binary = false;
    bool JIT = false;
    std::string OutFile;
    std::string HOutFile;

    // Code, header and message streams
    std::istream *In;
    std::ostream *Out;
    std::ostream *HOut;
    std::ostream *MOut;

    // File streams
    std::ifstream FIn;
    std::ofstream FOut;
    std::ofstream FHOut;


    // Trace options
    bool TraceScanning = false;
    bool TraceParsing = false;

    // File management method
    void setOutput(std::string Filename);
    void setHeaderOutput(std::string Filename);
    bool nextInput();

    // Parse methods
    bool parseStream(std::istream &In, const std::string &Src = "stream");
    bool parseString(const std::string &Input, const std::string &Src = "string");
    bool parseFile(const std::string &Filename);
    bool parseFiles(const std::vector<std::string> &Filenames);

    // Error methods
    void error(const common::Location &Loc, const std::string &Msg);
    void error(const std::string &Msg);
    void showError(common::Error Err);

    bool accept(common::Visitor &V);
    void addExternFunc(std::string Name, common::Type Signature);
};
}
