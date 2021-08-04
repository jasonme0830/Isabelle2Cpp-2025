#include <regex>
#include <cctype>
#include <exception>
#include <algorithm>
#include "ast.hpp"
#include "code.hpp"

using namespace std;

namespace
{
bool is_number(const string &str)
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

string argument_type(const string &type, size_t ind = 0)
{
    if (type.empty())
    {
        return type;
    }

    auto l = type.find('<') + 1;
    for (size_t i = 0; i < ind; ++i)
    {
        for (size_t cnt = 1; cnt != 0; ++l)
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
        l = type.find('<', l) + 1;
    }

    auto r = l;
    for (size_t cnt = 1; cnt != 0 ; ++r)
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

    return type.substr(l, r - l - 1);
}

pair<string, string> split_type(const string &type)
{
    auto pos = type.find('<');
    if (pos == type.npos)
    {
        return { type.substr(0, pos), "" };
    }
    return { type.substr(0, pos), type.substr(pos) };
}

string main_of_type(const string &type)
{
    return split_type(type).first;
}

string rest_of_type(const string &type)
{
    return split_type(type).second;
}

string add_elem_for_type(const string &type)
{
    return main_of_type(type) + "Elem" + rest_of_type(type);
}
}

namespace hol2cpp
{
Type::~Type() = default;
Expr::~Expr() = default;
Declaration::~Declaration() = default;

// --- code generation ---

void
DataTypeDecl::codegen(Code &code)
const
{
    code.add_header("variant");

    auto name = decl_type->main_name();
    auto &data_type = code.entry_data_type(name);
    data_type.name() = name;
    auto self = decl_type->build_data_type(data_type);

    vector<vector<Ptr<Type>>> abstracts;
    for (size_t i = 0; i < components.size(); ++i)
    {
        abstracts.push_back({});
        data_type.entry_component();
        data_type.add_constructor(components[i].constructor);
        code.bind_cons(components[i].constructor, data_type);
        for (auto &type : components[i].arguments)
        {
            auto field_type = type->build_data_type(data_type);
            data_type.add_field_type(field_type);
            /**
             * FIXME:
             *  datatype 'a tlist = tNil | tCons 'a "int tlist"
             *
             * this should be also recursive
            */
            if (field_type == self)
            {
                data_type.is_recuisive() = true;
                code.add_header("memory");
            }
            abstracts.back().push_back(move(type));
        }
    }
    data_type.abstracts() = move(abstracts);
}

void
FuncDecl::codegen(Code &code)
const
{
    auto &entity = code.entry_func_entity(name);

    entity.name() = name;
    type->build_func_entity(entity);
    for (auto &equation : equations)
    {
        equation->build_func_entity(entity);
    }
}

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

// --- build data type ---

string
NormalType::build_data_type(DataType &)
const
{
    return name;
}

string
ArgumentType::build_data_type(DataType &type)
const
{
    return type.add_argument_type(name);
}

string
TemplateType::build_data_type(DataType &type)
const
{
    auto res = name + '<';
    for (size_t i = 0; i < args.size(); ++i)
    {
        if (i == 0)
        {
            res += args[i]->build_data_type(type);
        }
        else
        {
            res += ", " + args[i]->build_data_type(type);
        }
    }
    res += '>';
    return res;
}

string
FuncType::build_data_type(DataType &type)
const
{
    string res = "std::function<";
    res += result_type()->build_data_type(type) + '(';
    for (size_t i = 0; i < types.size() - 1; ++i)
    {
        if (i == 0)
        {
            res += types[i]->build_data_type(type);
        }
        else
        {
            res += ", " + types[i]->build_data_type(type);
        }
    }
    res += ")>";
    return res;
}

// --- get main name of type ---

string
Type::main_name()
const
{
    throw runtime_error("fff");
}

string
NormalType::main_name()
const
{
    return name;
}

string
TemplateType::main_name()
const
{
    return name;
}

// --- apply type arguments ---

string
NormalType::apply(std::function<std::string(std::string)> &)
const
{
    return name;
}

string
ArgumentType::apply(std::function<std::string(std::string)> &trans)
const
{
    return trans(name);
}

string
TemplateType::apply(std::function<std::string(std::string)> &trans)
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

    auto res = mapping.count(name) ? mapping.at(name) : name;

    res += '<';
    for (size_t i = 0; i < args.size(); ++i)
    {
        if (i == 0)
        {
            res += args[i]->apply(trans);
        }
        else
        {
            res += ", " + args[i]->apply(trans);
        }
    }
    res += '>';

    return res;
}

string
FuncType::apply(std::function<std::string(std::string)> &trans)
const
{
    string type = "std::function<";
    type += result_type()->apply(trans) + '(';
    for (size_t i = 0; i < types.size() - 1; ++i)
    {
        if (i == 0)
        {
            type += types[i]->apply(trans);
        }
        else
        {
            type += ", " + types[i]->apply(trans);
        }
    }
    type += ")>";
    return type;
}

// --- build func entity ---

void
FuncType::build_func_entity(FuncEntity &entity)
const
{
    for (size_t i = 0; i < types.size() - 1; ++i)
    {
        entity.add_type(types[i]->gen_typeinfo(entity));
    }
    entity.add_type(result_type()->gen_typeinfo(entity));
}

void
Equation::build_func_entity(FuncEntity &entity)
const
{
    entity.entry_euqation();
    pattern->gen_pattern(entity, "");
    entity.add_expr("return " + expr->gen_expr(entity, entity.result_type()) + ";");
}

// --- generate pattern ---

void
Expr::gen_pattern(FuncEntity &, const string &)
const
{
    throw runtime_error("cannot be pattern");
}

void
VarExpr::gen_pattern(FuncEntity &entity, const string &prev)
const
{
    if (is_number(name))
    {
        entity.add_pattern_cond(prev + " != " + name);
    }
    else if (name == "True")
    {
        entity.add_pattern_cond(prev + "!= true");
    }
    else if (name == "False")
    {
        entity.add_pattern_cond(prev + "!= false");
    }
    else if (name == "Nil")
    {
        entity.add_pattern_cond("!" + prev + ".empty()");
    }
    else if (name == "None")
    {
        entity.add_pattern_cond(prev + ".has_value()");
    }
    else if (auto data_type = entity.code().find_data_type_by_cons(name))
    {
        string dot = data_type->is_recuisive() ? "->"s : "."s;
        entity.add_pattern_cond(prev + dot + "cons != " + data_type->name() + "Cons::" + name);
    }
    else
    {
        static std::regex arg_regex(R"(arg[1-9][0-9]*)");
        if (std::regex_match(prev, arg_regex))
        {
            entity.varrm_mapping()[name] = prev;
        }
        else
        {
            entity.add_pattern("auto " + name + " = " + prev);
        }
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
        entity.add_pattern_cond(prev + " == 0");
        args[0]->gen_pattern(entity, "(" + prev + ") - 1");
    }
    else if (constructor == "Cons")
    {
        entity.add_pattern_cond(prev + ".empty()");
        args[0]->gen_pattern(entity, prev + ".front()");
        args[1]->gen_pattern(entity, "decltype(" + prev + "){std::next(" + prev + ".begin()), " + prev + ".end()}");
    }
    else if (constructor == "Some")
    {
        entity.add_pattern_cond("!" + prev + ".has_value()");
        args[0]->gen_pattern(entity, prev + ".value()");
    }
    else if (constructor == "Pair")
    {
        assert(args.size() == 2);
        args[0]->gen_pattern(entity, prev + ".first");
        args[1]->gen_pattern(entity, prev + ".second");
    }
    else if (auto data_type = entity.code().find_data_type_by_cons(constructor))
    {
        string dot = data_type->is_recuisive() ? "->"s : "."s;
        entity.add_pattern_cond(prev + dot + "cons != " + data_type->name() + "Cons::" + constructor);
        auto pos = data_type->pos_of_cons(constructor);
        for (size_t i = 0; i < args.size(); ++i)
        {
            args[i]->gen_pattern(entity, prev + dot + "get_c" + to_string(pos + 1) + "().p" + to_string(i + 1));
        }
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
        entity.add_pattern_cond(prev + ".size() != " + to_string(exprs.size()));

        for (const auto &expr : exprs)
        {
            expr->gen_pattern(entity, prev + ".front()");
            entity.add_pattern(prev + ".pop_front()");
        }
    }
}

void
SetExpr::gen_pattern(FuncEntity &entity, const string &prev)
const
{
    assert(exprs.empty());
    entity.add_pattern_cond("!" + prev + ".empty()");
}

void
BinaryOpExpr::gen_pattern(FuncEntity &, const string &)
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
    else if (auto data_type = entity.code().find_data_type_by_cons(name))
    {
        if (data_type->is_recuisive())
        {
            return "std::make_shared<" + add_elem_for_type(type) + ">(" + data_type->name() + "Cons::" + name + ")";
        }
        else
        {
            return type + "(" + data_type->name() + "Cons::" + name + ")";
        }
    }
    else
    {
        auto &varrm_mapping = entity.varrm_mapping();
        auto it = varrm_mapping.find(name);
        if (it != varrm_mapping.end())
        {
            return it->second;
        }
        else
        {
            return name;
        }
    }
}

string
ConsExpr::gen_expr(FuncEntity &entity, const string &type)
const
{
    if (constructor == entity.name())
    {
        string expr = constructor + '(';
        for (size_t i = 0; i < args.size(); ++i)
        {
            if (i == 0)
            {
                expr += args[i]->gen_expr(entity, entity.types()[i]);
            }
            else
            {
                expr += ", " + args[i]->gen_expr(entity, entity.types()[i]);
            }
        }
        return expr + ')';
    }
    else if (auto func = entity.code().find_func_entity(constructor))
    {
        string expr = constructor + '(';
        for (size_t i = 0; i < args.size(); ++i)
        {
            if (i == 0)
            {
                expr += args[i]->gen_expr(entity, func->types()[i]);
            }
            else
            {
                expr += ", " + args[i]->gen_expr(entity, func->types()[i]);
            }
        }
        return expr + ')';
    }
    else if (constructor == "Suc")
    {
        assert(args.size() == 1);
        auto expr = args[0]->gen_expr(entity, type);
        return "(" + expr + ") + 1";
    }
    else if (constructor == "Cons")
    {
        assert(args.size() == 2);
        auto x = args[0]->gen_expr(entity, argument_type(type));
        auto xs = args[1]->gen_expr(entity, type);
        if (xs == "{}")
        {
            if (type.empty())
            {
                return "std::list<decltype(" + x + ")>{" + x + "}";
            }
            else
            {
                return type + "{" + x + "}";
            }
        }
        else
        {
            auto temp = entity.gen_temp();
            entity.add_expr("auto " + temp + " = " + xs + ";");
            entity.add_expr(temp + ".push_front(" + x + ");");
            return temp;
        }
    }
    else if (constructor == "Some")
    {
        assert(args.size() == 1);
        if (type.empty())
        {
            auto expr = args.front()->gen_expr(entity, type);
            if (expr == "{}")
            {
                // if receive Some Nil or Some None,
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
            auto arg_type = argument_type(type);
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
        return "std::make_pair(" + args[0]->gen_expr(entity, argument_type(type, 0))
            + ", " + args[1]->gen_expr(entity, argument_type(type, 1)) + ")"
        ;
    }
    else if (auto data_type = entity.code().find_data_type(main_of_type(type)))
    {
        auto dot = data_type->is_recuisive() ? "->"s : "."s;

        auto temp = entity.gen_temp();
        if (data_type->is_recuisive())
        {
            entity.add_expr(type + " " + temp + " = std::make_shared<" + add_elem_for_type(type) + ">(" + data_type->name() + "Cons::" + constructor + ");");
        }
        else
        {
            entity.add_expr(type + " " + temp + "{" + data_type->name() + "Cons::" + constructor +"};");
        }

        string stmt = temp + dot + "set_c" + to_string(data_type->pos_of_cons(constructor) + 1) + "(";

        function trans = [&](string arg_type)
        {
            return argument_type(type, data_type->find_argument_type(arg_type));
        };

        auto &abstracts = data_type->abstracts()[data_type->pos_of_cons(constructor)];
        for (size_t i = 0; i < abstracts.size(); ++i)
        {
            if (i == 0)
            {
                stmt += args[i]->gen_expr(entity, abstracts[i]->apply(trans));
            }
            else
            {
                stmt += ", " + args[i]->gen_expr(entity, abstracts[i]->apply(trans));
            }
        }
        entity.add_expr(stmt + ");");
        return temp;
    }
    // else as the common call without determined function
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
        return '(' + l + ") && (" + r + ')';
    case BOp::LogicOr:
        l = lhs->gen_expr(entity, type);
        r = rhs->gen_expr(entity, type);
        return '(' + l + ") || (" + r + ')';
    case BOp::LogicEq:
        l = lhs->gen_expr(entity, type);
        r = rhs->gen_expr(entity, type);
        return '(' + l + ") == (" + r + ')';
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
            entity.add_expr("if (" + lv + ".count(" + term + ")) {");
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
        l = lhs->gen_expr(entity, argument_type(type));
        r = rhs->gen_expr(entity, type);
        return r + ".count(" + l + ")";
    case BOp::SetNotin:
        l = lhs->gen_expr(entity, argument_type(type));
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
        if (l == "{}" && r == "{}")
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
            auto temp0 = entity.gen_temp();
            auto temp1 = entity.gen_temp();
            entity.add_expr("auto " + temp0 + " = " + l + ";");
            entity.add_expr("auto " + temp1 + " = " + r + ";");
            entity.add_expr(
                temp0 + ".insert(" + temp0 + ".end(), "
              + temp1 + ".begin(), " + temp1 + ".end());");
            return temp0;
        }
    }
    throw runtime_error("shit! shit! shit!");
}
}
