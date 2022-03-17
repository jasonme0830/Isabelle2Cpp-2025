#include "code.hpp"
#include "funcentity.hpp"
#include "../utility/format.hpp"

#include <fstream>

using namespace std;

namespace hol2cpp {
size_t theIndentSize = 4;

Code::Code() : headers_{"cstdlib"} {
    // ...
}

Datatype &Code::entry_datatype(const string &name) {
    names_of_datatypes_.push_back(name);
    return datatypes_[name];
}

Datatype *Code::find_datatype(const string &name) {
    if (datatypes_.count(name)) {
        return &datatypes_[name];
    } else {
        return nullptr;
    }
}

void Code::bind_cons(const string &cons, Datatype &datatype) {
    cons_types_.emplace(cons, datatype);
}

Datatype *Code::find_datatype_by_cons(const string &name) {
    if (cons_types_.count(name)) {
        return &cons_types_.at(name);
    } else {
        return nullptr;
    }
}

FuncEntity &Code::entry_func_entity(const string &name) {
    func_entities_.emplace(name, *this);
    names_of_func_entities_.push_back(name);
    return func_entities_.at(name);
}

FuncEntity *Code::find_func_entity(const string &name) {
    if (func_entities_.count(name)) {
        return &func_entities_.at(name);
    } else {
        return nullptr;
    }
}

void Code::add_short_def(const string &name, Ptr<ShortDef> short_def) {
    short_defs_.emplace(name, move(short_def));
}

const ShortDef *Code::get_short_def(const string &name) const {
    if (short_defs_.count(name)) {
        return short_defs_.at(name).get();
    } else {
        return nullptr;
    }
}

void Code::add_header(const string &header) {
    headers_.insert(header);
}

void Code::pop_datatype() {
    auto name = names_of_datatypes_.back();
    for (auto &cons : datatypes_[name].constructors()) {
        cons_types_.erase(cons);
    }
    datatypes_.erase(name);
    names_of_datatypes_.pop_back();
}

void Code::pop_function() {
    auto name = names_of_func_entities_.back();
    func_entities_.erase(name);
    names_of_func_entities_.pop_back();
}

const set<string> &Code::headers() const {
    return headers_;
}

vector<reference_wrapper<const Datatype>> Code::datatypes() const {
    vector<reference_wrapper<const Datatype>> res;
    for (auto &name : names_of_datatypes_) {
        res.emplace_back(datatypes_.at(name));
    }
    return res;
}

vector<reference_wrapper<const FuncEntity>> Code::func_entities() const {
    vector<reference_wrapper<const FuncEntity>> res;
    for (auto &name : names_of_func_entities_) {
        res.emplace_back(func_entities_.at(name));
    }
    return res;
}
} // namespace hol2cpp
