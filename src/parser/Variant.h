#pragma once

#include "Common.h"

#include <cstring>
#include <string>

using namespace common;
namespace parser {

    union Numeric {
        char chr;
        long long_int;
        long double long_double;
        bool boolean;
    };

    union Pointer {
        unique_ptr<Type> type;

        unique_ptr<Function> func;
        unique_ptr<Case> cse;
        unique_ptr<Pattern> pattern;
        unique_ptr<Expr> expr;
        unique_ptr<string> str;

        unique_ptr<vector<unique_ptr<Function>>> func_vec;
        unique_ptr<vector<unique_ptr<Case>>> cse_vec;
        unique_ptr<vector<unique_ptr<Pattern>>> pattern_vec;
        unique_ptr<vector<unique_ptr<Expr>>> expr_vec;


        Pointer() : type(nullptr) {
            //type.release();
            //type = unique_ptr<Type>(nullptr);
            //memset( this, unique_ptr<Pointer>(nullptr), sizeof(Pointer) );
        }

        ~Pointer() {
            //memset( &this->expr, 0, sizeof(decltype(this->expr)) );
            new (&type) unique_ptr<Type>(nullptr);
        }
    };

    class Variant {
    public:
        Variant();

        Numeric num;
        Pointer ptr;

        Variant &operator=(const Variant &other);
    };
}
