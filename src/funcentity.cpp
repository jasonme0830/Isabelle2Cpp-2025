#include "code.hpp"
#include "error.hpp"
#include "format.hpp"
#include "funcentity.hpp"

#undef ERROR
#define ERROR CodegenError

using namespace std;

namespace hol2cpp {
TypeInfo::TypeInfo(string name) : name(move(name)) {}

string TypeInfo::to_str() const {
    if (arguments.empty()) {
        return name;
    }

    if (is_function()) {
        auto type = name + '<' + arguments.back().to_str() + '(';
        for (size_t i = 0; i < arguments.size() - 1; ++i) {
            if (i == 0) {
                type += arguments[i].to_str();
            } else {
                type += ", " + arguments[i].to_str();
            }
        }
        return type + ")>";
    } else {
        auto type = name + '<';
        for (size_t i = 0; i < arguments.size(); ++i) {
            if (i == 0) {
                type += arguments[i].to_str();
            } else {
                type += ", " + arguments[i].to_str();
            }
        }
        return type + '>';
    }
}

string TypeInfo::to_str_with(const std::string &name) const {
    if (arguments.empty()) {
        return name;
    }

    if (is_function()) {
        auto type = name + '<' + arguments.back().to_str() + '(';
        for (size_t i = 0; i < arguments.size() - 1; ++i) {
            if (i == 0) {
                type += arguments[i].to_str();
            } else {
                type += ", " + arguments[i].to_str();
            }
        }
        return type + ")>";
    } else {
        auto type = name + '<';
        for (size_t i = 0; i < arguments.size(); ++i) {
            if (i == 0) {
                type += arguments[i].to_str();
            } else {
                type += ", " + arguments[i].to_str();
            }
        }
        return type + '>';
    }
}

bool TypeInfo::empty() const {
    return name.empty();
}

bool TypeInfo::is_function() const {
    return name == "std::function";
}

const TypeInfo &TypeInfo::result_typeinfo() {
    assertt(is_function());
    return arguments.back();
}

FuncEntity::FuncEntity(Code &code)
  : code_(code), indent_(0) {
    // ...
}

Code &FuncEntity::code() {
    return code_;
}

FuncEntity &FuncEntity::add_indent() {
    indent_ += Code::indent_size();
    return *this;
}

FuncEntity &FuncEntity::sub_indent() {
    indent_ -= Code::indent_size();
    return *this;
}

string &FuncEntity::name() {
    return name_;
}

const string &FuncEntity::name() const {
    return name_;
}

void FuncEntity::add_typeinfo(TypeInfo typeinfo) {
    typeinfos_.push_back(move(typeinfo));
}

string FuncEntity::add_argument_type(string name) {
    if (!template_mapping_.count(name)) {
        template_mapping_[name] = template_args_.size();
        template_args_.push_back("T" + to_string(template_args_.size() + 1));
    }
    return template_args_[template_mapping_[name]];
}

const TypeInfo &FuncEntity::result_typeinfo() {
    return typeinfos_.back();
}

const vector<TypeInfo> &FuncEntity::typeinfos() const {
    return typeinfos_;
}

const vector<string> &FuncEntity::template_args() const {
    return template_args_;
}

std::map<std::string, std::string> &FuncEntity::varrm_mapping() {
    return varrm_mapping_;
}

string FuncEntity::gen_temp() {
    return "temp" + to_string(temp_count_++);
}

void FuncEntity::entry_euqation() {
    temp_count_ = 0;
    patterns_.emplace_back();
    exprs_.emplace_back();
    varrm_mapping_.clear();
}

void FuncEntity::add_pattern(const string &pattern) {
    patterns_.back().push_back(pattern + ";");
}

void FuncEntity::add_pattern_cond(const string &cond) {
    patterns_.back().push_back("if ($) {"_fs.format(cond));
    add_indent();
        patterns_.back().push_back(string(indent_, ' ') + "break;");
    sub_indent();
    patterns_.back().push_back("}");
}

FuncEntity &FuncEntity::add_expr(const string &expr) {
    exprs_.back().push_back(std::string(indent_, ' ') + expr);
    return *this;
}

const vector<vector<string>> &FuncEntity::patterns() const {
    return patterns_;
}

const vector<vector<string>> &FuncEntity::exprs() const {
    return exprs_;
}
} // namespace hol2cpp
