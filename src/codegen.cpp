#include <cctype>
#include <exception>
#include <algorithm>
#include "ast.hpp"
#include "code.hpp"

using namespace std;

namespace
{
static
bool
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

static
string
get_argument_type(const string &type, size_t ind = 0)
{
    if (type.empty())
    {
        return type;
    }

    auto l = type.find('<');
    for (size_t i = 0; i < ind; ++i)
    {
        for (size_t cnt = 0;
            cnt == 0 and type[l] == ',';
            ++l)
        {
            if (type[l] == '<')
            {
                ++cnt;
            }
            else if (type[l] == '>')
            {
                --cnt;
            }
        }
        l += 2;
    }

    auto r = l;
    for (size_t cnt = 0;
        cnt == 0 and (type[r] == ',' or type[r] == '>');
        ++r)
    {
        if (type[r] == '<')
        {
            ++cnt;
        }
        else if (type[r] == '>')
        {
            --cnt;
        }
    }

    return type.substr(l, r - l);
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
    // as bool
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
        { "list",   "std::list" },
        { "pair",   "std::pair" }
    };

    static const map<string, string> mapping_header
    {
        { "set",    "set" },
        { "option", "optional" },
        { "list",   "list" },
        { "pair",   "utility" }
    };

    if (mapping_header.count(name))
    {
        entity.code().add_header(mapping_header.at(name));
    }

    auto res = mapping.count(name) ? mapping.at(name) : name;

    res += '<';
    for (size_t i = 0; i < args.size(); ++i)
    {
        if (i == 0)
        {
            res += args[i]->gen_typeinfo(entity);
        }
        else
        {
            res += ", " + args[i]->gen_typeinfo(entity);
        }
    }
    res += '>';

    return res;
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
    for (size_t i = 0; i < types.size() - 1; ++i)
    {
        entity.add_type(types[i]->gen_typeinfo(entity));
    }
    entity.add_type(result_type()->gen_typeinfo(entity));
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
Expr::gen_pattern(FuncEntity &entity, const string &prev)
const
{
    throw runtime_error("cannot be pattern");
}

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
        entity.add_pattern("if (" + prev + ".has_value()) {");
    }
    else
    {
        postfix = false;
        entity.add_pattern("auto " + name + " = " + prev + ";");
    }

    if (postfix)
    {
        entity.add_indent();
            entity.add_pattern("break;");
        entity.sub_indent();
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
        entity.add_indent();
            entity.add_pattern("break;");
        entity.sub_indent();
        entity.add_pattern("}");

        args[0]->gen_pattern(entity, prev + ".front()");
        entity.add_pattern(prev + ".pop_front();");
        args[1]->gen_pattern(entity, prev);
    }
    else if (constructor == "Some")
    {
        entity.add_pattern("if (!" + prev + ".has_value()) {");
        entity.add_indent();
            entity.add_pattern("break;");
        entity.sub_indent();
        entity.add_pattern("}");

        args[0]->gen_pattern(entity, prev + ".value()");
    }
    else if (constructor == "Pair")
    {
        assert(args.size() == 2);
        args[0]->gen_pattern(entity, prev + ".first");
        args[1]->gen_pattern(entity, prev + ".second");
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
        VarExpr("Nil").gen_pattern(entity, prev);
    }
    else
    {
        entity.add_pattern("if (" + prev + ".size() != " + to_string(exprs.size()) + ") {");
        entity.add_indent();
            entity.add_pattern("break;");
        entity.sub_indent();
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
    else if (name == "None")
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
    else if (constructor == "Some")
    {
        assert(args.size() == 1);
        if (type.empty())
        {
            auto expr = args.front()->gen_expr(entity, type);
            if (expr == "{}")
            {
                // if receive Some Nil,
                // it will return wrong result
                // becasue it has not enough typeinfo
                return expr;
            }
            else
            {
                return "std::make_optional<decltype(" + expr + ")>(" + expr + ")";
            }
        }
        else
        {
            auto arg_type = get_argument_type(type);
            auto expr = args.front()->gen_expr(entity, arg_type);
            return "std::make_optional<" + arg_type + ">(" + expr + ")";
        }
    }
    else if (constructor == "If")
    {
        assert(args.size() == 3);
        if (type.empty())
        {
            auto texpr = args[1]->gen_expr(entity, type);
            auto fexpr = args[2]->gen_expr(entity, type);
            auto ce = args[0]->gen_expr(entity, "bool");
            return ce + "? " + texpr + " : " + fexpr;
        }
        else
        {
            auto ce = args[0]->gen_expr(entity, "bool");
            auto res = entity.gen_temp();
            entity.add_expr(type + " " + res + ";");
            entity.add_expr("if (" + ce + ") {");
            entity.add_indent();
                entity.add_expr(res + " = " + args[1]->gen_expr(entity, type) + ";");
            entity.sub_indent();
            entity.add_expr("} else {");
            entity.add_indent();
                entity.add_expr(res + " = " + args[2]->gen_expr(entity, type) + ";");
            entity.sub_indent();
            entity.add_expr("}");
            return res;
        }
    }
    else if (constructor == "Pair")
    {
        assert(args.size() == 2);
        return "std::make_pair(" + args[0]->gen_expr(entity, get_argument_type(type, 0))
            + ", " + args[1]->gen_expr(entity, get_argument_type(type, 1)) + ")";
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

string
SetExpr::gen_expr(FuncEntity &entity, const string &type)
const
{
    if (exprs.empty())
    {
        return type.empty() ? "{}"s : (type + "()");
    }

    string res;
    if (type.empty())
    {
        res = "std::set<decltype(" +
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

string
BinaryOpExpr::gen_expr(FuncEntity &entity, const string &type)
const
{
    string l, r;

    switch (op)
    {
    case BOp::LogicAnd:
        l = lhs->gen_expr(entity, type);
        r = rhs->gen_expr(entity, type);
        return '(' + l + ") and (" + r + ')';
    case BOp::LogicOr:
        l = lhs->gen_expr(entity, type);
        r = rhs->gen_expr(entity, type);
        return '(' + l + ") or (" + r + ')';
    case BOp::LogicEq:
        l = lhs->gen_expr(entity, type);
        r = rhs->gen_expr(entity, type);
        return '(' + l + ") = (" + r + ')';
    case BOp::LogicNoteq:
        l = lhs->gen_expr(entity, type);
        r = rhs->gen_expr(entity, type);
        return '(' + l + ") != (" + r + ')';

    case BOp::OrderLe:
        l = lhs->gen_expr(entity, type);
        r = rhs->gen_expr(entity, type);
        return '(' + l + ") <= (" + r + ')';
    case BOp::OrderLt:
        l = lhs->gen_expr(entity, type);
        r = rhs->gen_expr(entity, type);
        return '(' + l + ") < (" + r + ')';
    case BOp::OrderGe:
        l = lhs->gen_expr(entity, type);
        r = rhs->gen_expr(entity, type);
        return '(' + l + ") >= (" + r + ')';
    case BOp::OrderGt:
        l = lhs->gen_expr(entity, type);
        r = rhs->gen_expr(entity, type);
        return '(' + l + ") > (" + r + ')';

    case BOp::SetInter:
    {
        l = lhs->gen_expr(entity, type);
        r = rhs->gen_expr(entity, type);

        auto lv = entity.gen_temp();
        auto rv = entity.gen_temp();
        entity.add_expr("auto " + lv + " = " + l + ";");
        entity.add_expr("auto " + rv + " = " + r + ";");

        auto res = entity.gen_temp();
        entity.add_expr("decltype(" + lv + ") " + res + ";");

        auto term = entity.gen_temp();
        entity.add_expr("for (auto " + term + " : " + rv + ") {");
        entity.add_indent();
            entity.add_expr("if (lv.count(" + term + ")) {");
            entity.add_indent();
                entity.add_expr(res + ".insert(" + term + ");");
            entity.sub_indent();
            entity.add_expr("}");
        entity.sub_indent();
        entity.add_expr("}");
        return res;
    }
    case BOp::SetUnion:
    {
        l = lhs->gen_expr(entity, type);
        r = rhs->gen_expr(entity, type);

        auto lv = entity.gen_temp();
        auto rv = entity.gen_temp();
        entity.add_expr("auto " + lv + " = " + l + ";");
        entity.add_expr("auto " + rv + " = " + r + ";");

        auto term = entity.gen_temp();
        entity.add_expr("for (auto " + term + " : " + rv + ") {");
        entity.add_indent();
            entity.add_expr(lv + ".insert(" + term + ");");
        entity.sub_indent();
        entity.add_expr("}");
        return lv;
    }
    case BOp::SetSubseteq:
        break;
    case BOp::SetSubset:
        break;
    case BOp::SetSupseteq:
        break;
    case BOp::SetSupset:
        break;
    case BOp::SetIn:
        l = lhs->gen_expr(entity, get_argument_type(type));
        r = rhs->gen_expr(entity, type);
        return r + ".count(" + l + ")";
    case BOp::SetNotin:
        l = lhs->gen_expr(entity, get_argument_type(type));
        r = rhs->gen_expr(entity, type);
        return "!" + r + ".count(" + l + ")";

    case BOp::NumAdd:
        l = lhs->gen_expr(entity, type);
        r = rhs->gen_expr(entity, type);
        return '(' + l + ") + (" + r + ')';
    case BOp::NumSub:
        l = lhs->gen_expr(entity, type);
        r = rhs->gen_expr(entity, type);
        return '(' + l + ") - (" + r + ')';
    case BOp::NumMul:
        l = lhs->gen_expr(entity, type);
        r = rhs->gen_expr(entity, type);
        return '(' + l + ") * (" + r + ')';
    case BOp::NumDiv:
        l = lhs->gen_expr(entity, type);
        r = rhs->gen_expr(entity, type);
        return '(' + l + ") / (" + r + ')';
    case BOp::NumMod:
        l = lhs->gen_expr(entity, type);
        r = rhs->gen_expr(entity, type);
        return '(' + l + ") % (" + r + ')';
    case BOp::NumPow:
        l = lhs->gen_expr(entity, type);
        r = rhs->gen_expr(entity, type);
        entity.code().add_header("cmath");
        return "std::pow(" + l + ", " + r + ')';

    case BOp::ListApp:
        l = lhs->gen_expr(entity, type);
        r = rhs->gen_expr(entity, type);
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
