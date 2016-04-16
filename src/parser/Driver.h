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

        SourceType SrcType;
        std::string Src;

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

        void setOutput(string filename);
        void setInputs(vector<string> filenames);
        void setHeaderOutput(string filename);

        bool parseStream(std::istream &in, const std::string &src = "stream input");
        bool parseString(const std::string &input, const std::string &src = "string stream");
        bool parseFile(const std::string &filename);
        bool parseFiles();

        bool accept(common::Visitor &visitor);

        void error(const common::Location &loc, const std::string &msg);
        void error(const std::string &msg);
        void showError(Error err);
    };
}
