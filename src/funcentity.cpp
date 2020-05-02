#include "code.hpp"
#include "funcentity.hpp"

using namespace std;

namespace hol2cpp
{
FuncEntity::FuncEntity(Code &code)
  : code_(code), indent_(0) {}

Code
&FuncEntity::code()
{
    return code_;
}

void
FuncEntity::add_indent()
{
    indent_ += Code::indent_size();
}

void
FuncEntity::sub_indent()
{
    indent_ -= Code::indent_size();
}

string
&FuncEntity::name()
{
    return name_;
}

const string
&FuncEntity::name()
const
{
    return name_;
}

void
FuncEntity::add_type(string type)
{
    types_.push_back(move(type));
}

string
FuncEntity::add_argument_type(string name)
{
    if (!template_mapping_.count(name))
    {
        template_mapping_[name] = template_args_.size();
        template_args_.push_back("T" + to_string(template_args_.size()));
    }
    return template_args_[template_mapping_[name]];
}

const string
&FuncEntity::result_type()
{
    return types_.back();
}

const vector<string>
&FuncEntity::types()
const
{
    return types_;
}

const vector<string>
&FuncEntity::template_args()
const
{
    return template_args_;
}

string
FuncEntity::gen_temp()
{
    return "temp" + to_string(temp_count_++);
}

void
FuncEntity::entry_euation()
{
    temp_count_ = 0;
    patterns_.emplace_back();
    exprs_.emplace_back();
}

void
FuncEntity::add_pattern(const string &pattern)
{
    patterns_.back().push_back(string(indent_, ' ') + pattern);
}

void
FuncEntity::add_expr(const string &expr)
{
    exprs_.back().push_back(string(indent_, ' ') + expr);
}

const vector<vector<string>>
&FuncEntity::patterns()
const
{
    return patterns_;
}

const vector<vector<string>>
&FuncEntity::exprs()
const
{
    return exprs_;
}
}
