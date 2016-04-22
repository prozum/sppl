#pragma once

#include "Node.h"

#include <vector>
#include <unordered_map>
#include <unordered_set>

using namespace std;

namespace common {
    class Scope {
    public:
        unordered_map<string, Type> Decls;
        unordered_map<string, AlgebraicDT &> Types;
        unordered_map<string, Product &> Constructors;

        Scope *Parent;
        std::vector<unique_ptr<Scope>> Children;

        Scope() : Scope(nullptr) { }

        Scope(Scope *Scp);

        bool declExists(std::string Id);

        bool typeExists(std::string Id);

        bool conExists(std::string Id);

        Type getDeclType(std::string Id);

        AlgebraicDT &getADT(std::string Id);

        Product &getCon(std::string Id);

    private:
        template<class T>
        bool exists(std::string Id, unordered_map<string, T> Map);

        template<class T>
        T get(std::string Id, unordered_map<string, T> Map);
    };
}
