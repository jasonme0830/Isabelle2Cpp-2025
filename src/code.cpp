#include "code.hpp"
#include "funcentity.hpp"

using namespace std;

namespace hol2cpp
{
Code::Code(ostream &out)
  : out_(out)
  , indent_(0)
  , headers_{"cstdlib"}
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
        out_ << "#include <" + header << ">" << endl;
    }
    out_ << endl;

    out_ << "template<typename T, typename Cons> T construct(Cons cons) { return std::make_shared<typename T::element_type>(cons); }\n" << endl;

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

    new_line() << "enum " << data_type.name() << "Cons {" << endl;
    add_indent();
    for (auto &cons : data_type.constructors())
    {
        new_line() << cons << "," << endl;
    }
    sub_indent();
    new_line() << "};\n\n";

    if (data_type.template_args().empty())
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

    new_line() << "struct " << name << " {" << endl;
    add_indent();
        auto &components = data_type.components();
        for (size_t i = 0; i < components.size(); ++i)
        {
            new_line() << "struct c" << to_string(i) << " {" << endl;
            add_indent();
            for (size_t j = 0; j < components[i].size(); ++j)
            {
                new_line() << components[i][j] << " p" + to_string(j) << ";" << endl;
            }
            sub_indent();
            new_line() << "};" << endl;
        }
        out_ << endl;

        new_line() << name << "(" << origin << "Cons cons" << ") : cons(cons) {}\n\n";

        for (size_t i = 0; i < components.size(); ++i)
        {
            new_line() << "c" << to_string(i) << " &get_c" << to_string(i) << "() {\n";
            add_indent();
                new_line() << "return std::get<c" << to_string(i) << ">(value);\n";
            sub_indent();
            new_line() << "};\n";

            new_line() << "void set_c" << to_string(i) << "(";
            for (size_t j = 0; j < components[i].size(); ++j)
            {
                if (j == 0)
                {
                    out_ << components[i][j] << " _p0";
                }
                else
                {
                    out_ << ", " << components[i][j] << " _p" << to_string(j);
                }
            }
            out_ << ") {" << endl;

            add_indent();
                new_line() << "value = c" << to_string(i) << "{";
            for (size_t j = 0; j < components[i].size(); ++j)
            {
                if (j == 0)
                {
                    out_ << "_p0";
                }
                else
                {
                    out_ << ", _p" + to_string(j);
                }
            }
                out_  << "};" << endl;
            sub_indent();
            new_line() << "}\n";
        }
        out_ << "\n";

        new_line() << origin << "Cons cons;\n";
        new_line() << "std::variant<";
        for (size_t i = 0; i < components.size(); ++i)
        {
            if (i == 0)
            {
                out_ << "c" << to_string(i);
            }
            else
            {
                out_ << ", c" << to_string(i);
            }
        }
        out_ << "> value;\n";
    sub_indent();
    new_line() << "};\n\n";
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

    new_line() << "struct " << name << ";" << endl;
    new_line() << "using " << origin << " = std::shared_ptr<" << name << ">;\n\n";
}

void
Code::gen_template_type_header(DataType &data_type)
{
    const auto &origin = data_type.name();
    auto name = data_type.name();
    auto &targs = data_type.template_args();
    new_line() << "template<";
    for (size_t i = 0; i < targs.size(); ++i)
    {
        if (i == 0)
        {
            out_ << "typename " << targs[i];
        }
        else
        {
            out_ << ", typename " << targs[i];
        }
    }
    out_ << ">\n";

    if (!data_type.is_recuisive())
    {
        return;
    }
    else
    {
        name += "Elem";
    }

    new_line() << "struct " << name << ";" << endl;
    if (data_type.is_recuisive())
    {
        new_line() << "template<";
        for (size_t i = 0; i < targs.size(); ++i)
        {
            if (i == 0)
            {
                out_ << "typename " << targs[i];
            }
            else
            {
                out_ << ", typename " << targs[i];
            }
        }
        out_ << ">\n";
        new_line() << "using " << origin << " = std::shared_ptr<" << name << "<";
        for (size_t i = 0; i < targs.size(); ++i)
        {
            if (i == 0)
            {
                out_ << targs[i];
            }
            else
            {
                out_ << ", " << targs[i];
            }
        }
        out_ << ">>;\n\n";
    }

    new_line() << "template<";
    for (size_t i = 0; i < targs.size(); ++i)
    {
        if (i == 0)
        {
            out_ << "typename " << targs[i];
        }
        else
        {
            out_ << ", typename " << targs[i];
        }
    }
    out_ << ">" << endl;
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
    new_line() << types.back() << endl;
    new_line() << entity.name() << "(";
    for (size_t i = 0; i < types.size() - 1; ++i)
    {
        if (i == 0)
        {
            out_ << types[i] << " arg" << to_string(i);
        }
        else
        {
            out_ << ", " << types[i] << " arg" << to_string(i);
        }
    }
    out_ << ") {" << endl;

    auto &patterns = entity.patterns();
    auto &exprs = entity.exprs();
    add_indent();
    for (size_t i = 0; i < patterns.size(); ++i)
    {
        new_line() << "for (;;) {" << endl;

        add_indent();
        for (auto &pattern : patterns[i])
        {
            new_line() << pattern << endl;
        }
        for (auto &expr : exprs[i])
        {
            new_line() << expr << endl;
        }
        sub_indent();

        new_line() << "}" << endl;
    }
    new_line() << "std::abort();" << endl;
    sub_indent();
    new_line() << "}\n\n";
}

void
Code::gen_template_func(FuncEntity &entity)
{
    auto &template_args = entity.template_args();
    new_line() << "template<";
    for (size_t i = 0; i < template_args.size(); ++i)
    {
        if (i == 0)
        {
            out_ << "typename " << template_args[i];
        }
        else
        {
            out_ << ", typename " << template_args[i];
        }
    }
    out_ << ">" << endl;
    gen_normal_func(entity);
}

ostream
&Code::new_line()
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
