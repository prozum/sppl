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

    enum class SourceType {
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
        Scanner Snr;
        Parser Psr;
        common::Scope Global;

        bool TraceScanning = false;
        bool TraceParsing = false;

        bool nextInput();
        vector<string> Filenames;
        size_t CurFile = 0;

        void setOutput(string Filename);
        void setInputs(vector<string> Filenames);
        void setHeaderOutput(string Filename);

        bool parseStream(std::istream &In, const std::string &Src = "stream");
        bool parseString(const std::string &Input, const std::string &Src = "string");
        bool parseFile(const std::string &Filename);
        bool parseFiles();

        bool accept(common::Visitor &V);

        void error(const common::Location &Loc, const std::string &Msg);
        void error(const std::string &Msg);
        void showError(Error Err);
    };
}
