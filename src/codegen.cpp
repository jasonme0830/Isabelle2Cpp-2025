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

void VarExpr::gen_pattern(FuncEntity &entity,
    const string &prev) const
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
        return "{}";
    }
    else
    {
        return name;
    }
}

void ConsExpr::gen_pattern(FuncEntity &entity,
    const string &prev) const
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
        args[0]->gen_pattern(entity, "(" + prev + ") - 1");
    }
    else if (constructor == "Cons")
    {
        args[0]->gen_pattern(entity, prev + ".front()");
        entity.add_pattern(prev + ".pop_front();");
        args[1]->gen_pattern(entity, prev);
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
        return "(" + expr + ") + 1";
    }
    else if (constructor == "Cons")
    {
        assert(args.size() == 2);
        auto x = args[0]->gen_expr(entity);
        auto xs = args[1]->gen_expr(entity);
        auto temp = entity.gen_temp();
        if (xs == "{}")
        {
            entity.add_expr("auto " + temp + " = std::list<decltype(" + x + ")>{" + x + "};");
        }
        else
        {
            entity.add_expr("auto " + temp + " = " + xs + ";");
            entity.add_expr(temp + ".push_front(" + x + ");");
        }
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
        return expr + ')';
    }
}

void BinaryOpExpr::gen_pattern(FuncEntity &entity,
    const string &prev) const
{
    switch (op)
    {
    case BOp::LogicAnd:
        break;
    case BOp::LogicOr:
        break;
    case BOp::LogicEq:
        break;
    case BOp::LogicNoteq:
        break;

    case BOp::OrderLe:
        break;
    case BOp::OrderLt:
        break;
    case BOp::OrderGe:
        break;
    case BOp::OrderGt:
        break;

    case BOp::SetInter:
        break;
    case BOp::SetUnion:
        break;
    case BOp::SetSubseteq:
        break;
    case BOp::SetSubset:
        break;
    case BOp::SetSupseteq:
        break;
    case BOp::SetSupset:
        break;
    case BOp::SetIn:
        break;
    case BOp::SetNotin:
        break;

    case BOp::NumAdd:
        break;
    case BOp::NumSub:
        break;
    case BOp::NumMul:
        break;
    case BOp::NumDiv:
        break;
    case BOp::NumMod:
        break;
    case BOp::NumPow:
        break;

    case BOp::ListCons:
    {
        lhs->gen_pattern(entity, prev + ".front()");
        entity.add_pattern(prev + ".pop_front();");
        rhs->gen_pattern(entity, prev);
    }
        break;
    case BOp::ListApp:
        break;
    }
}

string BinaryOpExpr::gen_expr(FuncEntity &entity) const
{
    auto l = lhs->gen_expr(entity);
    auto r = rhs->gen_expr(entity);
    string temp;

    switch (op)
    {
    case BOp::LogicAnd:
        break;
    case BOp::LogicOr:
        break;
    case BOp::LogicEq:
        break;
    case BOp::LogicNoteq:
        break;

    case BOp::OrderLe:
        break;
    case BOp::OrderLt:
        break;
    case BOp::OrderGe:
        break;
    case BOp::OrderGt:
        break;

    case BOp::SetInter:
        break;
    case BOp::SetUnion:
        break;
    case BOp::SetSubseteq:
        break;
    case BOp::SetSubset:
        break;
    case BOp::SetSupseteq:
        break;
    case BOp::SetSupset:
        break;
    case BOp::SetIn:
        break;
    case BOp::SetNotin:
        break;

    case BOp::NumAdd:
        break;
    case BOp::NumSub:
        break;
    case BOp::NumMul:
        break;
    case BOp::NumDiv:
        break;
    case BOp::NumMod:
        break;
    case BOp::NumPow:
        break;

    case BOp::ListCons:
    {
        temp = entity.gen_temp();
        if (r == "{}")
        {
            entity.add_expr("auto " + temp + " = std::list<decltype(" + l + ")>{" + l + "};");
        }
        else
        {
            entity.add_expr("auto " + temp + " = " + r + ";");
            entity.add_expr(temp + ".push_front(" + l + ");");
        }
    }
        break;
    case BOp::ListApp:
    {
        if (l == "{}" and r == "{}")
        {
            return "{}";
        }
        else if (l == "{}")
        {
            return r;
        }
        else if (r == "{}")
        {
            return l;
        }
        else
        {
            temp = entity.gen_temp();
            entity.add_expr("auto " + temp + " = " + l + ";");
            entity.add_expr(
                temp + ".insert(" + temp + ".end(), "
              + r + ".begin(), " + r + ".end());");
        }
    }
        break;
    }

    return temp;
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
