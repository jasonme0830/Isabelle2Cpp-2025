#include <cctype>
#include <exception>
#include <algorithm>
#include "ast.hpp"
#include "code.hpp"

using namespace std;

namespace
{
static bool is_number(const string &str)
{
    for (auto ch : str)
    {
        if (!isdigit(ch))
        {
            return false;
        }
    }
    return true;
}
}

namespace hol2cpp
{
AST::~AST() = default;
Type::~Type() = default;
Expr::~Expr() = default;

string NormalType::gen_typeinfo(FuncEntity &entity) const
{
    static map<string, string> mapping
    {
        { "nat", "std::uint64_t" },
        { "int", "std::int64_t" }
    };
    return mapping.count(name) ? mapping[name] : name;
}

string ArgumentType::gen_typeinfo(FuncEntity &entity) const
{
    return entity.add_argument_type(name);
}

string TemplateType::gen_typeinfo(FuncEntity &entity) const
{
    static map<string, string> mapping
    {
        { "set",    "std::set" },
        { "option", "std::optional" },
        { "list",   "std::list" }
    };
    return mapping.count(name)
           ? mapping[name] + '<' + arg->gen_typeinfo(entity) + '>'
           : name + '<' + arg->gen_typeinfo(entity) + '>';
}

string FuncType::gen_typeinfo(FuncEntity &entity) const
{
    string type = "std::function<";
    type += result_type()->gen_typeinfo(entity) + '(';
    for (size_t i = 0; i < types.size() - 1; ++i)
    {
        if (i == 0)
        {
            type += types[i]->gen_typeinfo(entity);
        } else
        {
            type += ", " + types[i]->gen_typeinfo(entity);
        }
    }
    type += ")>";
    return type;
}

void FuncType::build_entity(FuncEntity &entity) const
{
    entity.add_type(result_type()->gen_typeinfo(entity));
    for (size_t i = 0; i < types.size() - 1; ++i)
    {
        entity.add_type(types[i]->gen_typeinfo(entity));
    }
}

void VarExpr::gen_pattern(FuncEntity &entity, const string &prev) const
{
    bool postfix = true;
    if (is_number(name))
    {
        entity.add_pattern("if (" + prev + " != " + name + ") {");
    }
    else if (name == "True")
    {
        entity.add_pattern("if (" + prev + "!= true) {");
    }
    else if (name == "False")
    {
        entity.add_pattern("if (" + prev + "!= false) {");
    }
    else if (name == "Nil")
    {
        entity.add_pattern("if (!" + prev + ".empty()) {");
    }
    else
    {
        postfix = false;
        entity.add_pattern("auto " + name + " = " + prev + ";");
    }

    if (postfix)
    {
        entity.add_pattern(Code::raw_indent() + "break;");
        entity.add_pattern("}");
    }
}

string VarExpr::gen_expr(FuncEntity &entity) const
{
    if (is_number(name))
    {
        return name;
    }
    else if (name == "True")
    {
        return "true";
    }
    else if (name == "False")
    {
        return "false";
    }
    else if (name == "Nil")
    {
        // this may cause errors
        return entity.types().front() + "{}";
    }
    else
    {
        return name;
    }
}

void ConsExpr::gen_pattern(FuncEntity &entity, const string &prev) const
{
    if (constructor == entity.name())
    {
        assert(args.size() == entity.types().size() - 1);
        for (size_t i = 0; i < args.size(); ++i)
        {
            args[i]->gen_pattern(entity, "arg" + to_string(i + 1));
        }
    }
    else if (constructor == "Suc")
    {
        auto temp = entity.gen_temp();
        entity.add_pattern("auto " + temp + " = (" + prev + ") - 1;");
        args[0]->gen_pattern(entity, temp);
    }
    else if (constructor == "Cons")
    {
        auto temp = entity.gen_temp();
        entity.add_pattern("auto " + temp + " = " + prev + ".front();");
        args[0]->gen_pattern(entity, temp);

        temp = entity.gen_temp();
        entity.add_pattern(prev + ".pop_front();");
        entity.add_pattern("auto " + temp + " = " + prev + ";");
        args[1]->gen_pattern(entity, temp);
    }
    else
    {
        throw std::runtime_error("no such name: " + constructor);
    }
}

string ConsExpr::gen_expr(FuncEntity &entity) const
{
    if (constructor == "Suc")
    {
        assert(args.size() == 1);
        auto expr = args[0]->gen_expr(entity);
        auto temp = entity.gen_temp();
        entity.add_expr("auto " + temp + " = (" + expr + ") + 1");
        return temp;
    }
    else if (constructor == "Cons")
    {
        assert(args.size() == 2);
        auto x = args[0]->gen_expr(entity);
        auto xs = args[1]->gen_expr(entity);
        auto temp = entity.gen_temp();
        entity.add_expr("auto " + temp + " = " + xs + ";");
        entity.add_expr(temp + ".push_front(" + x + ");");
        return temp;
    }
    else
    {
        string expr = constructor + '(';
        for (size_t i = 0; i < args.size(); ++i)
        {
            if (i == 0)
            {
                expr += args[i]->gen_expr(entity);
            }
            else
            {
                expr += ", " + args[i]->gen_expr(entity);
            }
        }
        auto temp = entity.gen_temp();
        entity.add_expr("auto " + temp + " = " + expr + ");");
        return temp;
    }
}

void Equation::build_entity(FuncEntity &entity) const
{
    entity.entry_euation();
    pattern->gen_pattern(entity, "");
    entity.add_expr("return " + expr->gen_expr(entity) + ";");
}

void FuncDecl::build_entity(FuncEntity &entity) const
{
    entity.name() = name;
    type->build_entity(entity);
    for (auto &equation : equations)
    {
        equation->build_entity(entity);
    }
}
}
