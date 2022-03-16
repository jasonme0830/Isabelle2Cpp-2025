#include "codegen.hpp"

using namespace std;

namespace hol2cpp {
const map<string, string> theNormalTypeMapping {
    { "nat", "std::uint64_t" },
    { "int", "std::int64_t" }
};
const map<string, string> theTemplateTypeMapping {
    { "set",    "std::set" },
    { "option", "std::optional" },
    { "list",   "std::list" },
    { "pair",   "std::pair" }
};

/**
 * @brief generate the TypeInfo of the Isabelle/HOL type in the given function entity
 * 
 * @param entity the corresponding function entity that records the occurred types
 * @return std::list<T1> for 'a list in "'a list => 'a list"
 */
TypeInfo NormalType::gen_typeinfo(FuncEntity &entity) const {
    if (theNormalTypeMapping.count(name)) {
        entity.code().add_header("cstdint");
        return TypeInfo(theNormalTypeMapping.at(name));
    } else {
        // e.g. bool
        return TypeInfo(name);
    }
}

TypeInfo ArgumentType::gen_typeinfo(FuncEntity &entity) const {
    return TypeInfo(entity.add_argument_type(name));
}

TypeInfo TemplateType::gen_typeinfo(FuncEntity &entity) const {
    static const map<string, string> mapping_header {
        { "set",    "set" },
        { "option", "optional" },
        { "list",   "list" },
        { "pair",   "utility" }
    };

    if (mapping_header.count(name)) {
        entity.code().add_header(mapping_header.at(name));
    }

    TypeInfo res(theTemplateTypeMapping.count(name) ? theTemplateTypeMapping.at(name) : name);
    for (auto &arg : args) {
        res.arguments.push_back(arg->gen_typeinfo(entity));
    }
    return res;
}

TypeInfo FuncType::gen_typeinfo(FuncEntity &entity) const {
    entity.code().add_header("functional");

    TypeInfo res("std::function");
    for (auto &arg : types) {
        res.arguments.push_back(arg->gen_typeinfo(entity));
    }
    return res;
}

/**
 * @brief apply the trans function on variable types,
 *  mainly used to generate the TypeInfo of arguments from abstract types
 *  see ConsExpr::gen_expr for user-defined datatypes
 */
TypeInfo NormalType::apply(function<TypeInfo(const string &)> &) const {
    return TypeInfo(name);
}

TypeInfo ArgumentType::apply(function<TypeInfo(const string &)> &trans) const {
    return trans(name);
}

TypeInfo TemplateType::apply(function<TypeInfo(const string &)> &trans) const {
    static const map<string, string> mapping_header {
        { "set",    "set" },
        { "option", "optional" },
        { "list",   "list" },
        { "pair",   "utility" }
    };

    auto it = theTemplateTypeMapping.find(name);
    TypeInfo res(it == theTemplateTypeMapping.end() ? name : it->second);
    for (auto &arg : args) {
        res.arguments.push_back(arg->apply(trans));
    }
    return res;
}

TypeInfo FuncType::apply(function<TypeInfo(const string &)> &trans) const {
    TypeInfo res("std::function");
    for (auto &type : types) {
        res.arguments.push_back(type->apply(trans));
    }
    return res;
}
} // namespace hol2cpp
