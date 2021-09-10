#include "ast.hpp"
#include "code.hpp"
#include "format.hpp"

#include <regex>
#include <cctype>
#include <exception>
#include <algorithm>

using namespace std;

namespace {
bool is_number(const string &str) {
    for (auto ch : str) {
        if (!isdigit(ch)) {
            return false;
        }
    }
    return true;
}

string argument_type(const string &type, size_t ind = 0) {
    if (type.empty()) {
        return type;
    }

    auto l = type.find('<') + 1;
    for (size_t i = 0; i < ind; ++i) {
        for (size_t cnt = 1; cnt != 0; ++l) {
            if (type[l] == '<') {
                ++cnt;
            } else if (type[l] == '>') {
                --cnt;
            }
        }
        l = type.find('<', l) + 1;
    }

    auto r = l;
    for (size_t cnt = 1; cnt != 0 ; ++r) {
        if (type[r] == '<') {
            ++cnt;
        } else if (type[r] == '>') {
            --cnt;
        }
    }

    return type.substr(l, r - l - 1);
}

pair<string, string> split_type(const string &type) {
    auto pos = type.find('<');
    if (pos == type.npos) {
        return { type.substr(0, pos), "" };
    } else {
        return { type.substr(0, pos), type.substr(pos) };
    }
}

string main_of_type(const string &type) {
    return split_type(type).first;
}

string rest_of_type(const string &type) {
    return split_type(type).second;
}

string add_elem_for_type(const string &type) {
    return main_of_type(type) + "Elem" + rest_of_type(type);
}
} // namespace

namespace hol2cpp {
Type::~Type() = default;
Expr::~Expr() = default;
Declaration::~Declaration() = default;

// --- code generation ---
void DataTypeDecl::codegen(Code &code) const {
    code.add_header("variant");

    auto name = decl_type->main_name();
    auto &data_type = code.entry_data_type(name);
    data_type.name() = name;
    auto self = decl_type->build_data_type(data_type);

    vector<vector<Ptr<Type>>> abstracts;
    for (size_t i = 0; i < components.size(); ++i) {
        abstracts.push_back({});
        data_type.entry_component();
        data_type.add_constructor(components[i].constructor);
        code.bind_cons(components[i].constructor, data_type);

        for (auto &type : components[i].arguments) {
            auto field_type = type->build_data_type(data_type);
            data_type.add_field_type(field_type);
            if (field_type == self) {
                data_type.is_recuisive() = true;
                code.add_header("memory");
            }
            abstracts.back().push_back(move(type));
        }
    }
    data_type.abstracts() = move(abstracts);
}

void FuncDecl::codegen(Code &code) const {
    auto &entity = code.entry_func_entity(name);

    entity.name() = name;
    type->build_func_entity(entity);
    for (auto &equation : equations) {
        equation.build_func_entity(entity);
    }
}

// --- generate typeinfo ---
string NormalType::gen_typeinfo(FuncEntity &entity) const {
    static const map<string, string> mapping {
        { "nat", "std::uint64_t" },
        { "int", "std::int64_t" }
    };

    if (mapping.count(name)) {
        entity.code().add_header("cstdint");
        return mapping.at(name);
    } else {
        // e.g. bool
        return name;
    }
}

string ArgumentType::gen_typeinfo(FuncEntity &entity) const {
    return entity.add_argument_type(name);
}

string TemplateType::gen_typeinfo(FuncEntity &entity) const {
    static const map<string, string> mapping {
        { "set",    "std::set" },
        { "option", "std::optional" },
        { "list",   "std::list" },
        { "pair",   "std::pair" }
    };

    static const map<string, string> mapping_header {
        { "set",    "set" },
        { "option", "optional" },
        { "list",   "list" },
        { "pair",   "utility" }
    };

    if (mapping_header.count(name)) {
        entity.code().add_header(mapping_header.at(name));
    }

    auto res = mapping.count(name) ? mapping.at(name) : name;

    res += '<';
    for (size_t i = 0; i < args.size(); ++i) {
        if (i == 0) {
            res += args[i]->gen_typeinfo(entity);
        } else {
            res += ", " + args[i]->gen_typeinfo(entity);
        }
    }
    res += '>';

    return res;
}

string FuncType::gen_typeinfo(FuncEntity &entity) const {
    entity.code().add_header("functional");

    string type = "std::function<" + result_type()->gen_typeinfo(entity) + '(';
    for (size_t i = 0; i < types.size() - 1; ++i) {
        if (i == 0) {
            type += types[i]->gen_typeinfo(entity);
        } else {
            type += ", " + types[i]->gen_typeinfo(entity);
        }
    }
    return type + ")>";
}

// --- build data type ---
string NormalType::build_data_type(DataType &) const {
    return name;
}

string ArgumentType::build_data_type(DataType &type) const {
    return type.add_argument_type(name);
}

string TemplateType::build_data_type(DataType &type) const {
    auto res = name + '<';
    for (size_t i = 0; i < args.size(); ++i) {
        if (i == 0) {
            res += args[i]->build_data_type(type);
        } else {
            res += ", " + args[i]->build_data_type(type);
        }
    }
    return res + '>';
}

string FuncType::build_data_type(DataType &type) const {
    string res = "std::function<" + result_type()->build_data_type(type) + '(';
    for (size_t i = 0; i < types.size() - 1; ++i) {
        if (i == 0) {
            res += types[i]->build_data_type(type);
        } else {
            res += ", " + types[i]->build_data_type(type);
        }
    }
    return res + ")>";
}

// --- get main name of type ---
string Type::main_name() const {
    throw runtime_error("fff");
}

string NormalType::main_name() const {
    return name;
}

string TemplateType::main_name() const {
    return name;
}

// --- apply type arguments ---
string NormalType::apply(std::function<std::string(std::string)> &) const {
    return name;
}

string ArgumentType::apply(std::function<std::string(std::string)> &trans) const {
    return trans(name);
}

string TemplateType::apply(std::function<std::string(std::string)> &trans) const {
    static const map<string, string> mapping {
        { "set",    "std::set" },
        { "option", "std::optional" },
        { "list",   "std::list" },
        { "pair",   "std::pair" }
    };

    static const map<string, string> mapping_header {
        { "set",    "set" },
        { "option", "optional" },
        { "list",   "list" },
        { "pair",   "utility" }
    };

    auto res = mapping.count(name) ? mapping.at(name) : name;

    res += '<';
    for (size_t i = 0; i < args.size(); ++i) {
        if (i == 0) {
            res += args[i]->apply(trans);
        } else {
            res += ", " + args[i]->apply(trans);
        }
    }
    return res + '>';
}

string FuncType::apply(std::function<std::string(std::string)> &trans) const {
    string type = "std::function<" + result_type()->apply(trans) + '(';
    for (size_t i = 0; i < types.size() - 1; ++i) {
        if (i == 0) {
            type += types[i]->apply(trans);
        } else {
            type += ", " + types[i]->apply(trans);
        }
    }
    return type + ")>";
}

// --- build func entity ---
void FuncType::build_func_entity(FuncEntity &entity) const {
    for (size_t i = 0; i < types.size() - 1; ++i) {
        entity.add_type(types[i]->gen_typeinfo(entity));
    }
    entity.add_type(result_type()->gen_typeinfo(entity));
}

void Equation::build_func_entity(FuncEntity &entity) const {
    entity.entry_euqation();
    pattern->gen_pattern(entity, "");
    entity.add_expr("return $;", expr->gen_expr(entity, entity.result_type()));
}

// --- generate pattern ---
void Expr::gen_pattern(FuncEntity &, const string &) const {
    throw runtime_error("cannot be pattern");
}

void VarExpr::gen_pattern(FuncEntity &entity, const string &prev) const {
    if (is_number(name)) {
        entity.add_pattern_cond("$ != $", prev, name);
    } else if (name == "True") {
        entity.add_pattern_cond("$ != true", prev);
    } else if (name == "False") {
        entity.add_pattern_cond("$ != false", prev);
    } else if (name == "Nil") {
        entity.add_pattern_cond("!$.empty()", prev);
    } else if (name == "None") {
        entity.add_pattern_cond("$.has_value()", prev);
    } else if (auto data_type = entity.code().find_data_type_by_cons(name)) {
        string dot = data_type->is_recuisive() ? "->"s : "."s;
        entity.add_pattern_cond("$$cons != $Cons::$", prev, dot, data_type->name(), name);
    } else {
        static std::regex arg_regex(R"(arg[1-9][0-9]*)");
        if (std::regex_match(prev, arg_regex)) {
            entity.varrm_mapping()[name] = prev;
        } else {
            entity.add_pattern("auto $ = $", name, prev);
        }
    }
}

void ConsExpr::gen_pattern(FuncEntity &entity, const string &prev) const {
    if (constructor == entity.name()) {
        assert(args.size() == entity.types().size() - 1);
        for (size_t i = 0; i < args.size(); ++i) {
            args[i]->gen_pattern(entity, "arg" + to_string(i + 1));
        }
    } else if (constructor == "Suc") {
        entity.add_pattern_cond("$ == 0", prev);
        args[0]->gen_pattern(entity, "(" + prev + ") - 1");
    } else if (constructor == "Some") {
        entity.add_pattern_cond("!$.has_value()", prev);
        args[0]->gen_pattern(entity, prev + ".value()");
    } else if (constructor == "Pair") {
        assert(args.size() == 2);
        args[0]->gen_pattern(entity, prev + ".first");
        args[1]->gen_pattern(entity, prev + ".second");
    }

    // for List
    else if (constructor == "Cons") {
        entity.add_pattern_cond("$.empty()", prev);
        args[0]->gen_pattern(entity, prev + ".front()");
        args[1]->gen_pattern(entity, "decltype(" + prev + "){std::next(" + prev + ".begin()), " + prev + ".end()}");
    }

    // for Datatype
    else if (auto data_type = entity.code().find_data_type_by_cons(constructor)) {
        string dot = data_type->is_recuisive() ? "->"s : "."s;
        entity.add_pattern_cond("$$cons != $Cons::$", prev, dot, data_type->name(), constructor);
        auto pos = data_type->pos_of_cons(constructor);
        for (size_t i = 0; i < args.size(); ++i) {
            args[i]->gen_pattern(entity, "$$get_c$().p$"_fs.format(prev, dot, pos + 1, i + 1));
        }
    } else {
        throw std::runtime_error("no such name: " + constructor);
    }
}

void ListExpr::gen_pattern(FuncEntity &entity, const string &prev) const {
    if (exprs.empty()) {
        VarExpr("Nil").gen_pattern(entity, prev);
    } else {
        entity.add_pattern_cond("$.size() != $", prev, exprs.size());
        for (std::size_t i = 0; i < exprs.size(); ++i) {
            exprs[i]->gen_pattern(entity, "*std::next($.begin(), $)"_fs.format(prev, i));
        }
    }
}

void SetExpr::gen_pattern(FuncEntity &entity, const string &prev) const {
    assert(exprs.empty());
    entity.add_pattern_cond("!$.empty()", prev);
}

void BinaryOpExpr::gen_pattern(FuncEntity &, const string &) const {
    switch (op) {
        default:
            throw runtime_error("pattern should be consturctor");
    }
}

// --- generate expression ---
string VarExpr::gen_expr(FuncEntity &entity, const string &type) const {
    if (is_number(name)) {
        return name;
    } else if (name == "True") {
        return "true";
    } else if (name == "False") {
        return "false";
    } else if (name == "Nil") {
        return type.empty() ? "{}"s : (type + "()");
    } else if (name == "None") {
        return type.empty() ? "{}"s : (type + "()");
    } else if (auto data_type = entity.code().find_data_type_by_cons(name)) {
        if (data_type->is_recuisive()) {
            return "std::make_shared<$>($Cons::$)"_fs.format(
                add_elem_for_type(type), data_type->name(), name
            );
        } else {
            return "$($Cons::$)"_fs.format(
                type, data_type->name(), name
            );
        }
    } else {
        auto &varrm_mapping = entity.varrm_mapping();
        auto it = varrm_mapping.find(name);
        if (it != varrm_mapping.end()) {
            return it->second;
        } else {
            return name;
        }
    }
}

string ConsExpr::gen_expr(FuncEntity &entity, const string &type) const {
    if (constructor == entity.name()) {
        string expr = constructor + '(';
        for (size_t i = 0; i < args.size(); ++i) {
            if (i == 0) {
                expr += args[i]->gen_expr(entity, entity.types()[i]);
            } else {
                expr += ", " + args[i]->gen_expr(entity, entity.types()[i]);
            }
        }
        return expr + ')';
    } else if (auto func = entity.code().find_func_entity(constructor)) {
        string expr = constructor + '(';
        for (size_t i = 0; i < args.size(); ++i) {
            if (i == 0) {
                expr += args[i]->gen_expr(entity, func->types()[i]);
            } else {
                expr += ", " + args[i]->gen_expr(entity, func->types()[i]);
            }
        }
        return expr + ')';
    } else if (constructor == "Suc") {
        assert(args.size() == 1);
        auto expr = args[0]->gen_expr(entity, type);
        return "(" + expr + ") + 1";
    } else if (constructor == "Some") {
        assert(args.size() == 1);
        if (type.empty()) {
            auto expr = args.front()->gen_expr(entity, type);
            if (expr == "{}") {
                // if receive Some Nil or Some None,
                // it will return wrong result
                // becasue it has not enough typeinfo
                return expr;
            } else {
                return "std::make_optional<decltype($)>($)"_fs.format(
                    expr, expr
                );
            }
        } else {
            auto arg_type = argument_type(type);
            auto expr = args.front()->gen_expr(entity, arg_type);
            return "std::make_optional<$>($)"_fs.format(
                arg_type, expr
            );
        }
    }

    // for List
    else if (constructor == "Cons") {
        assert(args.size() == 2);
        auto x = args[0]->gen_expr(entity, argument_type(type));
        auto xs = args[1]->gen_expr(entity, type);
        if (xs == "{}") {
            if (type.empty()) {
                return "std::list<decltype($)>{$}"_fs.format(x, x);
            } else {
                return "${$};"_fs.format(type, x);
            }
        } else {
            auto temp = entity.gen_temp();
            entity
                .add_expr("auto $ = $;", temp, xs)
                .add_expr("$.push_front($);", temp, x)
            ;
            return temp;
        }
    } else if (constructor == "length") {
        assert(args.size() == 1);
        auto xs = args[0]->gen_expr(entity, type);
        if (xs == "{}") {
            return "0";
        } else {
            return xs + ".size()";
        }
    } else if (constructor == "take") {
        assert(args.size() == 2);
        auto n = args[0]->gen_expr(entity, "nat");
        auto xs = args[1]->gen_expr(entity, "");
        return "decltype($){ $.begin(), std::next($.begin(), $) }"_fs.format(xs, xs, xs, n);
    } else if (constructor == "drop") {
        assert(args.size() == 2);
        auto n = args[0]->gen_expr(entity, "nat");
        auto xs = args[1]->gen_expr(entity, "");
        return "decltype($){ std::next($.begin(), $), $.end() }"_fs.format(xs, xs, n, xs);
    }

    // for If-expression
    else if (constructor == "If") {
        assert(args.size() == 3);
        if (type.empty()) {
            auto texpr = args[1]->gen_expr(entity, type);
            auto fexpr = args[2]->gen_expr(entity, type);
            auto ce = args[0]->gen_expr(entity, "bool");
            return "$ ? $ : $"_fs.format(ce, texpr, fexpr);
        } else {
            auto ce = args[0]->gen_expr(entity, "bool");
            auto res = entity.gen_temp();
            entity
                .add_expr("$ $;", type, res)
                .add_expr("if ($) {", ce).add_indent()
                    .add_expr("$ = $;", res, args[1]->gen_expr(entity, type)).sub_indent()
                .add_expr("} else {").add_indent()
                    .add_expr("$ = $;", res, args[2]->gen_expr(entity, type)).sub_indent()
                .add_expr("}")
            ;
            return res;
        }
    }

    else if (constructor == "Pair") {
        assert(args.size() == 2);
        return "std::make_pair(" + args[0]->gen_expr(entity, argument_type(type, 0))
            + ", " + args[1]->gen_expr(entity, argument_type(type, 1)) + ")"
        ;
    }

    else if (auto data_type = entity.code().find_data_type(main_of_type(type))) {
        auto dot = data_type->is_recuisive() ? "->"s : "."s;

        auto temp = entity.gen_temp();
        if (data_type->is_recuisive()) {
            entity.add_expr("$ $ = std::make_shared<$>($Cons::$);",
                type, temp, add_elem_for_type(type), data_type->name(), constructor
            );
        } else {
            entity.add_expr("$ ${$Cons::$};",
                type, temp, data_type->name(), constructor
            );
        }

        string stmt = temp + dot + "set_c" + to_string(data_type->pos_of_cons(constructor) + 1) + "(";

        function trans = [&](string arg_type) {
            return argument_type(type, data_type->find_argument_type(arg_type));
        };

        auto &abstracts = data_type->abstracts()[data_type->pos_of_cons(constructor)];
        for (size_t i = 0; i < abstracts.size(); ++i) {
            if (i == 0) {
                stmt += args[i]->gen_expr(entity, abstracts[i]->apply(trans));
            } else {
                stmt += ", " + args[i]->gen_expr(entity, abstracts[i]->apply(trans));
            }
        }
        entity.add_expr(stmt + ");");
        return temp;
    }

    // else as the common call without determined function
    else {
        string expr = constructor + '(';
        for (size_t i = 0; i < args.size(); ++i) {
            if (i == 0) {
                expr += args[i]->gen_expr(entity, "");
            } else {
                expr += ", " + args[i]->gen_expr(entity, "");
            }
        }
        return expr + ')';
    }
}

string ListExpr::gen_expr(FuncEntity &entity, const string &type) const {
    if (exprs.empty()) {
        return type.empty() ? "{}"s : (type + "()");
    }

    string res;
    if (type.empty()) {
        res = "std::list<decltype($)>{"_fs.format(exprs.front()->gen_expr(entity, type));
    } else {
        res = type + '{';
    }

    for (size_t i = 0; i < exprs.size(); ++i) {
        if (i == 0) {
            res += exprs[i]->gen_expr(entity, type);
        } else {
            res += ", " + exprs[i]->gen_expr(entity, type);
        }
    }
    return res + '}';
}

string SetExpr::gen_expr(FuncEntity &entity, const string &type) const {
    if (exprs.empty()) {
        return type.empty() ? "{}"s : (type + "()");
    }

    string res;
    if (type.empty()) {
        res = "std::set<decltype($)>{"_fs.format(exprs.front()->gen_expr(entity, type));
    } else {
        res = type + '{';
    }

    for (size_t i = 0; i < exprs.size(); ++i) {
        if (i == 0) {
            res += exprs[i]->gen_expr(entity, type);
        } else {
            res += ", " + exprs[i]->gen_expr(entity, type);
        }
    }
    return res + '}';
}

string BinaryOpExpr::gen_expr(FuncEntity &entity, const string &type) const {
    static const map<BOp, string> bop_mapping {
        { BOp::LogicAnd, "&&" },
        { BOp::LogicOr, "||" },
        { BOp::LogicEq, "==" },
        { BOp::LogicNoteq, "!=" },

        { BOp::OrderLe, "<=" },
        { BOp::OrderLt, "<" },
        { BOp::OrderGe, ">=" },
        { BOp::OrderGt, ">" },

        { BOp::NumAdd, "+" },
        { BOp::NumSub, "-" },
        { BOp::NumMul, "*" },
        { BOp::NumDiv, "/" },
        { BOp::NumMod, "%" },
    };

    switch (op) {
        case BOp::SetInter: {
            auto l = lhs->gen_expr(entity, type);
            auto r = rhs->gen_expr(entity, type);

            auto lv = entity.gen_temp();
            auto rv = entity.gen_temp();
            entity
                .add_expr("auto $ = $;", lv, l)
                .add_expr("auto $ = $;", rv, r)
            ;

            auto res = entity.gen_temp();
            auto term = entity.gen_temp();

            entity
                .add_expr("decltype($) $;", lv, res)
                .add_expr("for (auto $ : $) {", term, rv).add_indent()
                    .add_expr("if ($.count($)) {", lv, term).add_indent()
                        .add_expr("$.insert($);", res, term).sub_indent()
                    .add_expr("}").sub_indent()
                .add_expr("}")
            ;
            return res;
        }
        case BOp::SetUnion: {
            auto l = lhs->gen_expr(entity, type);
            auto r = rhs->gen_expr(entity, type);

            auto lv = entity.gen_temp();
            auto rv = entity.gen_temp();
            entity
                .add_expr("auto $ = $;", lv, l)
                .add_expr("auto $ = $;", rv, r)
            ;

            auto term = entity.gen_temp();
            entity
                .add_expr("for (auto $ : $) {", term, rv).add_indent()
                    .add_expr("$.insert($);", lv, term).sub_indent()
                .add_expr("}")
            ;
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
            return "$.count($)"_fs.format(
                rhs->gen_expr(entity, type), lhs->gen_expr(entity, argument_type(type))
            );
        case BOp::SetNotin:
            return "!$.count($)"_fs.format(
                rhs->gen_expr(entity, type), lhs->gen_expr(entity, argument_type(type))
            );

        case BOp::NumPow:
            entity.code().add_header("cmath");
            return "std::pow($, $)"_fs.format(
                lhs->gen_expr(entity, type), rhs->gen_expr(entity, type)
            );

        case BOp::ListApp: {
            auto l = lhs->gen_expr(entity, type);
            auto r = rhs->gen_expr(entity, type);
            if (l == "{}" && r == "{}") {
                return type.empty() ? "{}"s : (type + "()");
            } else if (l == "{}") {
                return r;
            } else if (r == "{}") {
                return l;
            } else {
                auto temp0 = entity.gen_temp();
                auto temp1 = entity.gen_temp();
                entity
                    .add_expr("auto $ = $;", temp0, l)
                    .add_expr("auto $ = $;", temp1, r)
                    .add_expr("$.insert($.end(), $.begin(), $.end());", temp0, temp0, temp1, temp1)
                ;
                return temp0;
            }
        }

        default:
            assert(bop_mapping.count(op));

            return "($) $ ($)"_fs.format(
                lhs->gen_expr(entity, type), bop_mapping.at(op), rhs->gen_expr(entity, type)
            );
    }
    throw runtime_error("Implementation error at line $ in file $!"_fs.format(__LINE__, __FILE__));
}
} // namespace hol2cpp
