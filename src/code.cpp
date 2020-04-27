#include "code.hpp"

using namespace std;

namespace hol2cpp
{
void
Code::generate()
{
    for (const auto &header : headers_)
    {
        cout << "#include <" + header << ">" << endl;
    }
    cout << endl;

    for (auto &entity : func_entities_)
    {
        generate_single(entity);
        out_ << endl;
    }
}

void
Code::generate_single(FuncEntity &entity)
{
    indent_ = 0;
    if (entity.template_args().empty())
    {
        generate_normal(entity);
    }
    else
    {
        generate_template(entity);
    }
}

void
Code::generate_normal(FuncEntity &entity)
{
    auto &types = entity.types();
    new_line() << types.front() << endl;
    new_line() << entity.name() << "(";
    for (size_t i = 1; i < types.size(); ++i)
    {
        if (i == 1)
        {
            out_ << types[i] << " arg" << to_string(i);
        }
        else
        {
            out_ << ", " << types[i] << " arg" << to_string(i);
        }
    }
    out_ << ") {" << endl;
    add_indent();

    auto &patterns = entity.patterns();
    auto &exprs = entity.exprs();
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

    sub_indent();
    new_line() << "}" << endl;
}

void
Code::generate_template(FuncEntity &entity)
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
    generate_normal(entity);
}
} // namespace hol2cpp
