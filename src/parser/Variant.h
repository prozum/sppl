#pragma once

#include "Common.h"

#include <cstring>
#include <string>

using namespace common;
namespace parser {
    union value {
        unique_ptr<Function> func;
        unique_ptr<Case> cse;
        unique_ptr<Pattern> pattern;
        unique_ptr<Expr> expr;
        Type type;

        vector<unique_ptr<Function>> func_vec;
        vector<unique_ptr<Case>> cse_vec;
        vector<unique_ptr<Pattern>> pattern_vec;
        vector<unique_ptr<Expr>> expr_vec;
        vector<Type> type_vec;

        unique_ptr<string> str;
        char chr;
        long long_int;
        long double long_double;
        bool boolean;

        value() {
            memset( this, 0, sizeof(value) );
        }

        ~value() {
        }
    };
    class Variant {
    public:
        Variant();
        value val;
        Variant &operator=(const Variant &other);
    };
}