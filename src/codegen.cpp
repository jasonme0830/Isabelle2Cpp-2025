#include <cctype>
#include <exception>
#include <algorithm>
#include "ast.hpp"
#include "code.hpp"

using namespace std;

namespace
{
static bool
is_number(const string &str)
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

static string
get_argument_type(const string &type)
{
    if (type.empty())
    {
        return type;
    }

    auto l = type.find('<');
    auto r = type.rfind('>');
    return type.substr(l + 1, r - l - 1);
}
}

namespace hol2cpp
{
AST::~AST() = default;
Type::~Type() = default;
Expr::~Expr() = default;

// --- generate typeinfo ---

string
NormalType::gen_typeinfo(FuncEntity &entity)
const
{
    static const map<string, string> mapping
    {
        { "nat", "std::uint64_t" },
        { "int", "std::int64_t" }
    };

    if (mapping.count(name))
    {
        entity.code().add_header("cstdint");
        return mapping.at(name);
    }
    return name;
}

string
ArgumentType::gen_typeinfo(FuncEntity &entity)
const
{
    return entity.add_argument_type(name);
}

string
TemplateType::gen_typeinfo(FuncEntity &entity)
const
{
    static const map<string, string> mapping
    {
        { "set",    "std::set" },
        { "option", "std::optional" },
        { "list",   "std::list" }
    };

    static const map<string, string> mapping_header
    {
        { "set",    "set" },
        { "option", "optional" },
        { "list",   "list" }
    };

    if (mapping_header.count(name))
    {
        entity.code().add_header(mapping_header.at(name));
    }

    return mapping.count(name)
           ? mapping.at(name) + '<' + arg->gen_typeinfo(entity) + '>'
           : name + '<' + arg->gen_typeinfo(entity) + '>';
}

string
FuncType::gen_typeinfo(FuncEntity &entity)
const
{
    entity.code().add_header("functional");

    string type = "std::function<";
    type += result_type()->gen_typeinfo(entity) + '(';
    for (size_t i = 0; i < types.size() - 1; ++i)
    {
        if (i == 0)
        {
            type += types[i]->gen_typeinfo(entity);
        }
        else
        {
            type += ", " + types[i]->gen_typeinfo(entity);
        }
    }
    type += ")>";
    return type;
}

// --- build entity ---

void
FuncType::build_entity(FuncEntity &entity)
const
{
    entity.add_type(result_type()->gen_typeinfo(entity));
    for (size_t i = 0; i < types.size() - 1; ++i)
    {
        entity.add_type(types[i]->gen_typeinfo(entity));
    }
}

void
Equation::build_entity(FuncEntity &entity)
const
{
    entity.entry_euation();
    pattern->gen_pattern(entity, "");
    entity.add_expr("return " + expr->gen_expr(entity, entity.result_type()) + ";");
}

void
FuncDecl::build_entity(FuncEntity &entity)
const
{
    entity.name() = name;
    type->build_entity(entity);
    for (auto &equation : equations)
    {
        equation->build_entity(entity);
    }
}

// --- generate pattern ---

void
VarExpr::gen_pattern(FuncEntity &entity, const string &prev)
const
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
    else if (name == "None")
    {
        entity.add_pattern("if (!" + prev + ".has_value()) {");
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

void
ConsExpr::gen_pattern(FuncEntity &entity, const string &prev)
const
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
        entity.add_pattern("if (" + prev + ".empty()) {");
        entity.add_pattern(Code::raw_indent() + "break;");
        entity.add_pattern("}");

        args[0]->gen_pattern(entity, prev + ".front()");
        entity.add_pattern(prev + ".pop_front();");
        args[1]->gen_pattern(entity, prev);
    }
    else
    {
        throw std::runtime_error("no such name: " + constructor);
    }
}

void
ListExpr::gen_pattern(FuncEntity &entity, const string &prev)
const
{
    if (exprs.empty())
    {
        VarExpr("Nil").gen_expr(entity, prev);
    }
    else
    {
        entity.add_pattern("if (" + prev + ".size() != " + to_string(exprs.size()) + ") {");
        entity.add_pattern(Code::raw_indent() + "break;");
        entity.add_pattern("}");

        for (const auto &expr : exprs)
        {
            expr->gen_pattern(entity, prev + ".front()");
            entity.add_pattern(prev + ".pop_front();");
        }
    }
}

void
BinaryOpExpr::gen_pattern(FuncEntity &entity, const string &prev)
const
{
    switch (op)
    {
    default:
        throw runtime_error("pattern should be consturctor");
    }
}

// --- generate expression ---

string
VarExpr::gen_expr(FuncEntity &entity, const string &type)
const
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
        return type.empty() ? "{}"s : (type + "()");
    }
    else
    {
        return name;
    }
}

string
ConsExpr::gen_expr(FuncEntity &entity, const string &type)
const
{
    if (constructor == "Suc")
    {
        assert(args.size() == 1);
        auto expr = args[0]->gen_expr(entity, type);
        return "(" + expr + ") + 1";
    }
    else if (constructor == "Cons")
    {
        assert(args.size() == 2);
        auto x = args[0]->gen_expr(entity, get_argument_type(type));
        auto xs = args[1]->gen_expr(entity, type);
        auto temp = entity.gen_temp();
        if (xs == "{}")
        {
            if (type.empty())
            {
                entity.add_expr("auto " + temp + " = std::list<decltype(" + x + ")>{" + x + "};");
            }
            else
            {
                entity.add_expr(type + " " + temp + " = {" + x + "};");
            }
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
                expr += args[i]->gen_expr(entity, "");
            }
            else
            {
                expr += ", " + args[i]->gen_expr(entity, "");
            }
        }
        return expr + ')';
    }
}

string
ListExpr::gen_expr(FuncEntity &entity, const string &type)
const
{
    if (exprs.empty())
    {
        return type.empty() ? "{}"s : (type + "()");
    }
    else
    {
        string res;
        if (type.empty())
        {
            res = "std::list<decltype(" +
                exprs.front()->gen_expr(entity, type) + ")>{";
        }
        else
        {
            res = type + "{";
        }

        for (size_t i = 0; i < exprs.size(); ++i)
        {
            if (i == 0)
            {
                res += exprs[i]->gen_expr(entity, type);
            }
            else
            {
                res += ", " + exprs[i]->gen_expr(entity, type);
            }
        }
        return res + "}";
    }
}

string
BinaryOpExpr::gen_expr(FuncEntity &entity, const string &type)
const
{
    auto l = lhs->gen_expr(entity, type);
    auto r = rhs->gen_expr(entity, type);

    switch (op)
    {
    case BOp::LogicAnd:
        return '(' + l + ") and (" + r + ')';
    case BOp::LogicOr:
        return '(' + l + ") or (" + r + ')';
    case BOp::LogicEq:
        return '(' + l + ") = (" + r + ')';
    case BOp::LogicNoteq:
        return '(' + l + ") != (" + r + ')';

    case BOp::OrderLe:
        return '(' + l + ") <= (" + r + ')';
    case BOp::OrderLt:
        return '(' + l + ") < (" + r + ')';
    case BOp::OrderGe:
        return '(' + l + ") >= (" + r + ')';
    case BOp::OrderGt:
        return '(' + l + ") > (" + r + ')';

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
        return '(' + l + ") + (" + r + ')';
    case BOp::NumSub:
        return '(' + l + ") - (" + r + ')';
    case BOp::NumMul:
        return '(' + l + ") * (" + r + ')';
    case BOp::NumDiv:
        return '(' + l + ") / (" + r + ')';
    case BOp::NumMod:
        return '(' + l + ") % (" + r + ')';
    case BOp::NumPow:
        return "pow(" + l + ", " + r + ')';

    case BOp::ListApp:
        if (l == "{}" and r == "{}")
        {
            return type.empty() ? "{}"s : (type + "()");
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
            auto temp = entity.gen_temp();
            entity.add_expr("auto " + temp + " = " + l + ";");
            entity.add_expr(
                temp + ".insert(" + temp + ".end(), "
              + r + ".begin(), " + r + ".end());");
            return temp;
        }
    }
    throw runtime_error("shit! shit! shit!");
}
}
