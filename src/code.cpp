#include "code.hpp"
#include "format.hpp"
#include "funcentity.hpp"

using namespace std;

namespace hol2cpp
{
Code::Code(ostream &out)
  : out_(out), indent_(0), headers_{"cstdlib"}
{
    // ...
}

DataType
&Code::entry_data_type(const string &name)
{
    names_of_data_types_.push_back(name);
    return data_types_[name];
}

DataType
*Code::find_data_type(const string &name)
{
    if (data_types_.count(name))
    {
        return &data_types_[name];
    }
    else
    {
        return nullptr;
    }
}

void
Code::bind_cons(const string &cons, DataType &data_type)
{
    cons_types_.emplace(cons, data_type);
}

DataType
*Code::find_data_type_by_cons(const string &name)
{
    if (cons_types_.count(name))
    {
        return &cons_types_.at(name);
    }
    else
    {
        return nullptr;
    }
}

FuncEntity
&Code::entry_func_entity(const string &name)
{
    func_entities_.emplace(name, *this);
    names_of_func_entities_.push_back(name);
    return func_entities_.at(name);
}

FuncEntity
*Code::find_func_entity(const string &name)
{
    if (func_entities_.count(name))
    {
        return &func_entities_.at(name);
    }
    else
    {
        return nullptr;
    }
}

void
Code::generate()
{
    for (const auto &header : headers_)
    {
        "#include <$>\n"_fs.outf(out_, header);
    }   out_ << endl;

    if (headers_.count("memory")) {
        "template<typename T, typename Cons> T construct(Cons cons) { return std::make_shared<typename T::element_type>(cons); }\n\n"_fs.outf(out_);
    }

    for (auto &name : names_of_data_types_)
    {
        gen_data_type(data_types_[name]);
    }

    for (auto &name : names_of_func_entities_)
    {
        gen_single_func(func_entities_.at(name));
    }
}

size_t
&Code::indent_size()
{
    static size_t value = 4;
    return value;
}

void
Code::add_header(const string &header)
{
    headers_.insert(header);
}

void
Code::gen_data_type(DataType &data_type)
{
    indent_ = 0;

    "enum $Cons {\n"_fs.outf(newline(), data_type.name());
    add_indent();
    for (auto &cons : data_type.constructors())
    {
        "$,\n"_fs.outf(newline(), cons);
    }
    sub_indent();
    "};\n\n"_fs.outf(newline());

    if (data_type.is_normal_type())
    {
        gen_normal_type_header(data_type);
    }
    else
    {
        gen_template_type_header(data_type);
    }
    gen_type_rest(data_type);
}

void
Code::gen_type_rest(DataType &data_type)
{
    const auto &origin = data_type.name();
    auto name = data_type.name();
    if (data_type.is_recuisive())
    {
        name += "Elem";
    }

    "struct $ {\n"_fs.outf(newline(), name);
    add_indent();
    auto &components = data_type.components();
    for (size_t i = 0; i < components.size(); ++i)
    {
        "struct c$ {\n"_fs.outf(newline(), i + 1);
        add_indent();
        for (size_t j = 0; j < components[i].size(); ++j)
        {
            "$ p$;\n"_fs.outf(newline(), components[i][j], j + 1);
        }
        sub_indent();
        "};\n"_fs.outf(newline());
    }   out_ << endl;

        "$($Cons cons) : cons(cons) {}\n\n"_fs.outf(newline(), name, origin);

    for (size_t i = 0; i < components.size(); ++i)
    {
        "c$ &get_c$() {\n"_fs.outf(newline(), i + 1, i + 1);
        add_indent();
            "return std::get<c$>(value);\n"_fs.outf(newline(), i + 1);
        sub_indent();
        "};\n"_fs.outf(newline());

        "void set_c$("_fs.outf(newline(), i + 1);
        for (size_t j = 0; j < components[i].size(); ++j)
        {
            if (j == 0)
            {
                "$ _p1"_fs.outf(out_, components[i][j]);;
            }
            else
            {
                ", $ _p$"_fs.outf(out_, components[i][j], j + 1);
            }
        }   ") {\n"_fs.outf(out_);

        add_indent();
            "value = c${"_fs.outf(newline(), i + 1);
        for (size_t j = 0; j < components[i].size(); ++j)
        {
            if (j == 0)
            {
                "_p1"_fs.outf(out_);
            }
            else
            {
                ", _p$"_fs.outf(out_, j + 1);
            }
        }   "};\n"_fs.outf(out_);
        sub_indent();
        "}\n"_fs.outf(newline());
    }
        out_ << endl;

        "$Cons cons;\n"_fs.outf(newline(), origin);
        "std::variant<"_fs.outf(newline());
        for (size_t i = 0; i < components.size(); ++i)
        {
            if (i == 0)
            {
                "c$"_fs.outf(out_, i + 1);;
            }
            else
            {
                ", c$"_fs.outf(out_, i + 1);
            }
        }
        "> value;\n"_fs.outf(out_);
    sub_indent();
    "};\n\n"_fs.outf(newline());
}

void
Code::gen_normal_type_header(DataType &data_type)
{
    if (!data_type.is_recuisive())
    {
        return;
    }

    const auto &origin = data_type.name();
    auto name = data_type.name() + "Elem";

    "struct $;\n"_fs                        .outf(newline(), name);
    "using $ = std::shared_ptr<$>;\n\n"_fs  .outf(newline(), origin, name);
}

void
Code::gen_template_type_header(DataType &data_type)
{
    const auto &origin = data_type.name();
    auto name = data_type.name();
    auto &targs = data_type.template_args();
    "template<"_fs.outf(newline());
    for (size_t i = 0; i < targs.size(); ++i)
    {
        if (i == 0)
        {
            "typename $"_fs.outf(out_, targs[i]);
        }
        else
        {
            ", typename $"_fs.outf(out_, targs[i]);
        }
    }
    ">\n"_fs.outf(out_);

    if (!data_type.is_recuisive())
    {
        return;
    }
    else
    {
        name += "Elem";
    }

    "struct $;\n"_fs.outf(newline(), name);
    if (data_type.is_recuisive())
    {
        "template<"_fs.outf(newline());
        for (size_t i = 0; i < targs.size(); ++i)
        {
            if (i == 0)
            {
                "typename $"_fs.outf(out_, targs[i]);
            }
            else
            {
                ", typename $"_fs.outf(out_, targs[i]);
            }
        }   ">\n"_fs.outf(out_);
        "using $ = std::shared_ptr<$<"_fs.outf(newline(), origin, name);
        for (size_t i = 0; i < targs.size(); ++i)
        {
            if (i == 0)
            {
                out_ << targs[i];
            }
            else
            {
                ", $"_fs.outf(out_, targs[i]);
            }
        }   ">>;\n\n"_fs.outf(out_);
    }

    "template<"_fs.outf(newline());
    for (size_t i = 0; i < targs.size(); ++i)
    {
        if (i == 0)
        {
            "typename $"_fs.outf(out_, targs[i]);
        }
        else
        {
            ", typename $"_fs.outf(out_, targs[i]);
        }
    }   ">\n"_fs.outf(out_);
}

void
Code::gen_single_func(FuncEntity &entity)
{
    indent_ = 0;
    if (entity.template_args().empty())
    {
        gen_normal_func(entity);
    }
    else
    {
        gen_template_func(entity);
    }
}

void
Code::gen_normal_func(FuncEntity &entity)
{
    auto &types = entity.types();
    "$ $("_fs.outf(newline(), types.back(), entity.name());
    for (size_t i = 0; i < types.size() - 1; ++i)
    {
        if (i == 0)
        {
            "const $ &arg$"_fs.outf(out_, types[i], i + 1);
        }
        else
        {
            ", const $ &arg$"_fs.outf(out_, types[i], i + 1);
        }
    }   ") {\n"_fs.outf(out_);

    auto &patterns = entity.patterns();
    auto &exprs = entity.exprs();
    add_indent();
    for (size_t i = 0; i < patterns.size(); ++i)
    {
        "for (;;) {\n"_fs.outf(newline());

        add_indent();
        for (auto &pattern : patterns[i])
        {
            newline() << pattern << endl;
        }
        for (auto &expr : exprs[i])
        {
            newline() << expr << endl;
        }
        sub_indent();

        "}\n"_fs.outf(newline());
    }   "std::abort();\n"_fs.outf(newline());
    sub_indent();
    "}\n\n"_fs.outf(newline());
}

void
Code::gen_template_func(FuncEntity &entity)
{
    auto &template_args = entity.template_args();
    "template<"_fs.outf(newline());
    for (size_t i = 0; i < template_args.size(); ++i)
    {
        if (i == 0)
        {
            "typename $"_fs.outf(out_, template_args[i]);
        }
        else
        {
            ", typename $"_fs.outf(out_, template_args[i]);
        }
    }   ">\n"_fs.outf(out_);
    gen_normal_func(entity);
}

ostream
&Code::newline()
{
    out_ << string(indent_, ' ');
    return out_;
}

void
Code::add_indent()
{
    indent_ += indent_size();
}

void
Code::sub_indent()
{
    indent_ -= indent_size();
}
} // namespace hol2cpp
