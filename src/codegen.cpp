#include "ast.hpp"
#include "code.hpp"
#include "error.hpp"
#include "format.hpp"

#include <regex>
#include <cctype>
#include <iostream>
#include <algorithm>

#undef ERROR
#define ERROR CodegenError

using namespace std;

namespace hol2cpp {
static const map<string, string> local_ntype_mapping {
    { "nat", "std::uint64_t" },
    { "int", "std::int64_t" }
};

static const map<string, string> local_ttype_mapping {
    { "set",    "std::set" },
    { "option", "std::optional" },
    { "list",   "std::list" },
    { "pair",   "std::pair" }
};

Type::~Type() = default;
Expr::~Expr() = default;
Definition::~Definition() = default;

// --- code generation ---
/**
 * TODO: distinguish ShortDef with others
*/
void Theory::codegen(Code &code) const {
    size_t datatype_cnt = 0, fun_cnt = 0, predefined = 0;
    for (size_t i = 0; i < definitions.size(); ++i) {
        auto decl = definitions[i].get();
        if (decl) try {
            if (decl->is_predefined()) {
                ++predefined;
            } else {
                decl->codegen(code);
            }

            if (decl->is_datatype_decl()) {
                ++datatype_cnt;
            } else {
                ++fun_cnt;
            }
        } catch (const exception &e) {
            string name;
            if (auto datatype_decl = dynamic_cast<DataTypeDef *>(decl)) {
                name = "datatype " + datatype_decl->decl_type->main_name();
                code.pop_datatype();
            } else {
                auto fun_decl = static_cast<FunctionDef *>(decl);
                name = "function " + fun_decl->name;
                code.pop_function();
            }
            "$ at No.$ definition of $:\n`$\n\n"_fs.outf(
                cerr, info::light_red("codegen error"), i + 1, name, e.what()
            );
        }
    }

    "$\n`scanned $ definitions;\n"_fs.outf(
        cout, info::light_green("Result:"), definitions.size()
    );
    "`generated $ definitions ($ predefined):\n"_fs.outf(
        cout, datatype_cnt + fun_cnt, predefined
    );
    "``$ datatypes and $ functions.\n"_fs.outf(
        cout, datatype_cnt, fun_cnt
    );
}

void DataTypeDef::codegen(Code &code) const {
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

void FunctionDef::codegen(Code &code) const {
    auto &entity = code.entry_func_entity(name);

    entity.name() = name;
    type->build_func_entity(entity);
    for (auto &equation : equations) {
        equation.build_func_entity(entity);
    }
}

void ShortDef::codegen(Code &code) const {
    code.add_short_def(name, make_unique<ShortDef>(name, move(parameters), move(expr)));
}

// --- generate typeinfo ---
TypeInfo NormalType::gen_typeinfo(FuncEntity &entity) const {
    if (local_ntype_mapping.count(name)) {
        entity.code().add_header("cstdint");
        return TypeInfo(local_ntype_mapping.at(name));
    } else {
        // e.g. bool
        return TypeInfo(name);
    }
}

TypeInfo ArgumentType::gen_typeinfo(FuncEntity &entity) const {
    return TypeInfo(entity.add_argument_type(name));
}

TypeInfo TemplateType::gen_typeinfo(FuncEntity &entity) const {
    static const map<string, string> mapping_header {
        { "set",    "set" },
        { "option", "optional" },
        { "list",   "list" },
        { "pair",   "utility" }
    };

    if (mapping_header.count(name)) {
        entity.code().add_header(mapping_header.at(name));
    }

    TypeInfo res(local_ttype_mapping.count(name) ? local_ttype_mapping.at(name) : name);
    for (auto &arg : args) {
        res.arguments.push_back(arg->gen_typeinfo(entity));
    }
    return res;
}

TypeInfo FuncType::gen_typeinfo(FuncEntity &entity) const {
    entity.code().add_header("functional");

    TypeInfo res("std::function");
    for (auto &arg : types) {
        res.arguments.push_back(arg->gen_typeinfo(entity));
    }
    return res;
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
    throw CodegenError("failed call of Type::main_name()");
}

string NormalType::main_name() const {
    return name;
}

string TemplateType::main_name() const {
    return name;
}

// --- apply type arguments ---
TypeInfo NormalType::apply(function<TypeInfo(const string &)> &) const {
    return TypeInfo(name);
}

TypeInfo ArgumentType::apply(function<TypeInfo(const string &)> &trans) const {
    return trans(name);
}

TypeInfo TemplateType::apply(function<TypeInfo(const string &)> &trans) const {
    static const map<string, string> mapping_header {
        { "set",    "set" },
        { "option", "optional" },
        { "list",   "list" },
        { "pair",   "utility" }
    };

    TypeInfo res(local_ttype_mapping.count(name) ? local_ttype_mapping.at(name) : name);
    for (auto &arg : args) {
        res.arguments.push_back(arg->apply(trans));
    }
    return res;
}

TypeInfo FuncType::apply(function<TypeInfo(const string &)> &trans) const {
    TypeInfo res("std::function");
    for (auto &type : types) {
        res.arguments.push_back(type->apply(trans));
    }
    return res;
}

// --- build func entity ---
void FuncType::build_func_entity(FuncEntity &entity) const {
    for (auto &type : types) {
        entity.add_typeinfo(type->gen_typeinfo(entity));
    }
}

void Equation::build_func_entity(FuncEntity &entity) const {
    entity.entry_euqation();
    pattern->gen_pattern(entity, "");
    entity.add_expr("return $;", expr->gen_expr(entity, entity.result_typeinfo()));
}

// --- generate pattern ---
void Expr::gen_pattern(FuncEntity &, const string &) const {
    throw CodegenError("failed call Expr::gen_pattern(...)");
}

void IntegralExpr::gen_pattern(FuncEntity &entity, const string &prev) const {
    entity.add_pattern_cond("$ != $", prev, value);
}

void VarExpr::gen_pattern(FuncEntity &entity, const string &prev) const {
    if (name == entity.name()) {
        assertt(entity.args_size() == 0);
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
        assertt(args.size() == entity.args_size());
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
        assertt(args.size() == 2);
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
        throw CodegenError("failed call VarExpr::gen_pattern(...): no such name: " + constructor);
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
    assertt(exprs.empty());
    entity.add_pattern_cond("!$.empty()", prev);
}

void BinaryOpExpr::gen_pattern(FuncEntity &entity, const string &prev) const {
    switch (op.type)
    {
        case Token::Type::Sharp: {
            ConsExpr cons_expr("Cons");
            cons_expr.args.push_back(move(this->lhs));
            cons_expr.args.push_back(move(this->rhs));
            cons_expr.gen_pattern(entity, prev);

            lhs = move(cons_expr.args[0]);
            rhs = move(cons_expr.args[1]);
        }
            break;

        default:
            throw CodegenError("failed call BinaryOpExpr::gen_pattern(): unsupported binary operator " + op.value);
    }
}

// --- generate expression ---
string IntegralExpr::gen_expr(FuncEntity &, const TypeInfo &) const {
    return value;
}

string VarExpr::gen_expr(FuncEntity &entity, const TypeInfo &typeinfo) const {
    if (name == "True") {
        return "true";
    } else if (name == "False") {
        return "false";
    } else if (name == "Nil") {
        return typeinfo.empty() ? "{}"s : (typeinfo.to_str() + "()");
    } else if (name == "None") {
        return typeinfo.empty() ? "{}"s : (typeinfo.to_str() + "()");
    } else if (auto data_type = entity.code().find_data_type_by_cons(name)) {
        if (data_type->is_recuisive()) {
            return "std::make_shared<$>($Cons::$)"_fs.format(
                typeinfo.to_str_with(typeinfo.name + "Elem"), data_type->name(), name
            );
        } else {
            return "$($Cons::$)"_fs.format(
                typeinfo.to_str(), data_type->name(), name
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

string ConsExpr::gen_expr(FuncEntity &entity, const TypeInfo &typeinfo) const {
    if (constructor == entity.name()) {
        string expr = constructor + '(';
        assertt(entity.args_size() == args.size());
        for (size_t i = 0; i < args.size(); ++i) {
            if (i == 0) {
                expr += args[i]->gen_expr(entity, entity.typeinfos()[i]);
            } else {
                expr += ", " + args[i]->gen_expr(entity, entity.typeinfos()[i]);
            }
        }
        return expr + ')';
    } else if (auto func = entity.code().find_func_entity(constructor)) {
        string expr = constructor + '(';
        assertt(func->args_size() == args.size());
        for (size_t i = 0; i < args.size(); ++i) {
            if (i == 0) {
                // expr += args[i]->gen_expr(entity, func->typeinfos()[i]);
                expr += args[i]->gen_expr(entity, TypeInfo());
            } else {
                // expr += ", " + args[i]->gen_expr(entity, func->typeinfos()[i]);
                expr += ", " + args[i]->gen_expr(entity, TypeInfo());
            }
        }
        return expr + ')';
    } else if (constructor == "Suc") {
        assertt(args.size() == 1);
        auto expr = args[0]->gen_expr(entity, typeinfo);
        return "(" + expr + ") + 1";
    } else if (constructor == "Some") {
        assertt(args.size() == 1);
        if (typeinfo.empty()) {
            auto expr = args.front()->gen_expr(entity, typeinfo);
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
            auto arg_type = typeinfo.arguments.front();
            auto expr = args.front()->gen_expr(entity, arg_type);
            return "std::make_optional<$>($)"_fs.format(
                arg_type.to_str(), expr
            );
        }
    }

    // for List
    else if (constructor == "Cons") {
        assertt(args.size() == 2);
        auto x = args[0]->gen_expr(entity, typeinfo.arguments.front());
        auto xs = args[1]->gen_expr(entity, typeinfo);
        if (xs == "{}") {
            if (typeinfo.empty()) {
                return "std::list<decltype($)>{$}"_fs.format(x, x);
            } else {
                return "${$};"_fs.format(typeinfo.to_str(), x);
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
        assertt(args.size() == 1);
        auto xs = args[0]->gen_expr(entity, typeinfo);
        if (xs == "{}") {
            return "0";
        } else {
            return xs + ".size()";
        }
    } else if (constructor == "take") {
        assertt(args.size() == 2);
        auto n = args[0]->gen_expr(entity, TypeInfo("nat"));
        auto xs = args[1]->gen_expr(entity, TypeInfo());
        return "decltype($){ $.begin(), std::next($.begin(), $) }"_fs.format(xs, xs, xs, n);
    } else if (constructor == "drop") {
        assertt(args.size() == 2);
        auto n = args[0]->gen_expr(entity, TypeInfo("nat"));
        auto xs = args[1]->gen_expr(entity, TypeInfo());
        return "decltype($){ std::next($.begin(), $), $.end() }"_fs.format(xs, xs, n, xs);
    }

    // for If-expression
    else if (constructor == "If") {
        assertt(args.size() == 3);
        if (typeinfo.empty()) {
            auto texpr = args[1]->gen_expr(entity, typeinfo);
            auto fexpr = args[2]->gen_expr(entity, typeinfo);
            auto ce = args[0]->gen_expr(entity, TypeInfo("bool"));
            return "$ ? $ : $"_fs.format(ce, texpr, fexpr);
        } else {
            auto ce = args[0]->gen_expr(entity, TypeInfo("bool"));
            auto res = entity.gen_temp();
            entity
                .add_expr("$ $;", typeinfo.to_str(), res)
                .add_expr("if ($) {", ce).add_indent()
                    .add_expr("$ = $;", res, args[1]->gen_expr(entity, typeinfo)).sub_indent()
                .add_expr("} else {").add_indent()
                    .add_expr("$ = $;", res, args[2]->gen_expr(entity, typeinfo)).sub_indent()
                .add_expr("}")
            ;
            return res;
        }
    }

    else if (constructor == "Pair") {
        assertt(args.size() == 2);
        return "std::make_pair(" + args[0]->gen_expr(entity, typeinfo.arguments.front())
            + ", " + args[1]->gen_expr(entity, typeinfo[1]) + ")"
        ;
    }

    // For Datatype
    else if (auto data_type = entity.code().find_data_type(typeinfo.name)) {
        auto dot = data_type->is_recuisive() ? "->"s : "."s;

        auto temp = entity.gen_temp();
        if (data_type->is_recuisive()) {
            entity.add_expr("$ $ = std::make_shared<$>($Cons::$);",
                typeinfo.to_str(), temp, typeinfo.to_str_with(typeinfo.name + "Elem"), data_type->name(), constructor
            );
        } else {
            entity.add_expr("$ ${$Cons::$};",
                typeinfo.to_str(), temp, data_type->name(), constructor
            );
        }

        auto &abstracts = data_type->abstracts()[data_type->pos_of_cons(constructor)];
        if (abstracts.empty()) {
            return temp;
        }

        string stmt = temp + dot + "set_c" + to_string(data_type->pos_of_cons(constructor) + 1) + "(";

        function trans = [&](const string &arg_type) {
            return typeinfo[data_type->find_argument_type(arg_type)];
        };

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

    // for ShortDef
    else if (auto short_def = entity.code().get_short_def(constructor)) {
        assertt(short_def->parameters.size() == args.size());
        for (size_t i = 0; i < args.size(); ++i) {
            entity.add_expr("auto $ = $;", short_def->parameters[i], args[i]->gen_expr(entity, TypeInfo()));
        }
        return short_def->expr->gen_expr(entity, typeinfo);
    }

    // else as the common call without determined function
    else {
        string expr = constructor + '(';
        for (size_t i = 0; i < args.size(); ++i) {
            if (i == 0) {
                expr += args[i]->gen_expr(entity, TypeInfo());
            } else {
                expr += ", " + args[i]->gen_expr(entity, TypeInfo());
            }
        }
        return expr + ')';
    }
}

string ListExpr::gen_expr(FuncEntity &entity, const TypeInfo &typeinfo) const {
    if (exprs.empty()) {
        return typeinfo.empty() ? "{}"s : (typeinfo.to_str() + "()");
    }

    string res;
    if (typeinfo.empty()) {
        res = "std::list<decltype($)>{"_fs.format(exprs.front()->gen_expr(entity, typeinfo));
    } else {
        res = typeinfo.to_str() + '{';
    }

    for (size_t i = 0; i < exprs.size(); ++i) {
        if (i == 0) {
            res += exprs[i]->gen_expr(entity, typeinfo);
        } else {
            res += ", " + exprs[i]->gen_expr(entity, typeinfo);
        }
    }
    return res + '}';
}

string SetExpr::gen_expr(FuncEntity &entity, const TypeInfo &typeinfo) const {
    if (exprs.empty()) {
        return typeinfo.empty() ? "{}"s : (typeinfo.to_str() + "()");
    }

    string res;
    if (typeinfo.empty()) {
        res = "std::set<decltype($)>{"_fs.format(exprs.front()->gen_expr(entity, typeinfo));
    } else {
        res = typeinfo.to_str() + '{';
    }

    for (size_t i = 0; i < exprs.size(); ++i) {
        if (i == 0) {
            res += exprs[i]->gen_expr(entity, typeinfo);
        } else {
            res += ", " + exprs[i]->gen_expr(entity, typeinfo);
        }
    }
    return res + '}';
}

string BinaryOpExpr::gen_expr(FuncEntity &entity, const TypeInfo &typeinfo) const {
    static const map<Token::Type, string> bop_mapping {
        { Token::Type::And, "&&" },
        { Token::Type::Or, "||" },
        { Token::Type::Equiv, "==" },
        { Token::Type::NotEq, "!=" },

        { Token::Type::Le, "<=" },
        { Token::Type::Lt, "<" },
        { Token::Type::Ge, ">=" },
        { Token::Type::Gt, ">" },

        { Token::Type::Add, "+" },
        { Token::Type::Sub, "-" },
        { Token::Type::Mul, "*" },
        { Token::Type::Div, "/" },
        { Token::Type::Mod, "%" },
    };

    switch (op.type) {
        case Token::Type::Inter: {
            auto l = lhs->gen_expr(entity, typeinfo);
            auto r = rhs->gen_expr(entity, typeinfo);

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
        case Token::Type::Union: {
            auto l = lhs->gen_expr(entity, typeinfo);
            auto r = rhs->gen_expr(entity, typeinfo);

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
        case Token::Type::SubsetEq:
            break;
        case Token::Type::Subset:
            break;
        case Token::Type::SupsetEq:
            break;
        case Token::Type::Supset:
            break;
        case Token::Type::In:
            return "$.count($)"_fs.format(
                rhs->gen_expr(entity, typeinfo), lhs->gen_expr(entity, typeinfo.arguments.front())
            );
        case Token::Type::NotIn:
            return "!$.count($)"_fs.format(
                rhs->gen_expr(entity, typeinfo), lhs->gen_expr(entity, typeinfo.arguments.front())
            );

        case Token::Type::Pow:
            entity.code().add_header("cmath");
            return "std::pow($, $)"_fs.format(
                lhs->gen_expr(entity, typeinfo), rhs->gen_expr(entity, typeinfo)
            );

        case Token::Type::At: {
            auto l = lhs->gen_expr(entity, typeinfo);
            auto r = rhs->gen_expr(entity, typeinfo);
            if (l == "{}" && r == "{}") {
                return typeinfo.empty() ? "{}"s : (typeinfo.to_str() + "()");
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
        case Token::Type::Sharp: {
            ConsExpr cons_expr("Cons");
            cons_expr.args.push_back(move(this->lhs));
            cons_expr.args.push_back(move(this->rhs));
            auto res = cons_expr.gen_expr(entity, typeinfo);

            lhs = move(cons_expr.args[0]);
            rhs = move(cons_expr.args[1]);

            return res;
        }
        case Token::Type::Doot: {
            auto elem_typeinfo = typeinfo.empty() ? TypeInfo() : typeinfo[0];

            auto start = entity.gen_temp();
            entity.add_expr("auto $ = $;", start, lhs->gen_expr(entity, elem_typeinfo));

            auto end = entity.gen_temp();
            entity.add_expr("auto $ = $;", end, rhs->gen_expr(entity, elem_typeinfo));

            auto res = entity.gen_temp();
            if (typeinfo.empty()) {
                entity.add_expr("std::list<decltype($)> $;", start, res);
            } else {
                entity.add_expr("$ $;", typeinfo.to_str(), res);
            }

            auto i = entity.gen_temp();
            entity.add_expr("for (auto $ = $; $ <= $; ++$) {", i, start, i, end, i).add_indent()
                .add_expr("$.push_back($);", res, i).sub_indent()
                .add_expr("}")
            ;

            return res;
        }
        case Token::Type::DootLt: {
            auto elem_typeinfo = typeinfo.empty() ? TypeInfo() : typeinfo[0];

            auto start = entity.gen_temp();
            entity.add_expr("auto $ = $;", start, lhs->gen_expr(entity, elem_typeinfo));

            auto end = entity.gen_temp();
            entity.add_expr("auto $ = $;", end, rhs->gen_expr(entity, elem_typeinfo));

            auto res = entity.gen_temp();
            if (typeinfo.empty()) {
                entity.add_expr("std::list<decltype($)> $;", start, res);
            } else {
                entity.add_expr("$ $;", typeinfo.to_str(), res);
            }

            auto i = entity.gen_temp();
            entity.add_expr("for (auto $ = $; $ < $; ++$) {", i, start, i, end, i).add_indent()
                .add_expr("$.push_back($);", res, i).sub_indent()
                .add_expr("}")
            ;

            return res;
        }

        /**
         * WARN: BE CAREFUL HERE!
        */
        case Token::Type::Equiv:
            if (auto val_expr = dynamic_cast<VarExpr *>(lhs.get())) {
                if (val_expr->name == "Nil") {
                    return "$.empty()"_fs.format(rhs->gen_expr(entity, TypeInfo()));
                }
            }

            if (auto val_expr = dynamic_cast<VarExpr *>(rhs.get())) {
                if (val_expr->name == "Nil") {
                    return "$.empty()"_fs.format(lhs->gen_expr(entity, TypeInfo()));
                }
            }

        default:
            assertt(bop_mapping.count(op.type));

            return "($) $ ($)"_fs.format(
                lhs->gen_expr(entity, TypeInfo()), bop_mapping.at(op.type), rhs->gen_expr(entity, TypeInfo())
            );
    }
    throw CodegenError("failed call BinaryOpExpr::gen_expr(...): unsupported binary operator " + op.value);
}

string CaseExpr::gen_expr(FuncEntity &entity, const TypeInfo &typeinfo) const {
    auto temp0 = entity.gen_temp();
    entity.add_expr("auto $ = ([&] {", temp0).add_indent();

    auto e = expr->gen_expr(entity, TypeInfo());
    auto temp1 = entity.gen_temp();
    entity.add_expr("auto $ = $;", temp1, e);

    for (auto &equation : equations) {
        entity.add_expr("for(;;) {").add_indent();
        equation.pattern->gen_pattern(entity, temp1);
        entity.add_expr("return $;", equation.expr->gen_expr(entity, typeinfo)).sub_indent()
            .add_expr("}")
        ;
    }

    entity.sub_indent()
        .add_expr("})();")
    ;

    return temp0;
}

string LetinExpr::gen_expr(FuncEntity &entity, const TypeInfo &typeinfo) const {
    auto temp = entity.gen_temp();
    entity.add_expr("auto $ = $;", temp, equation.expr->gen_expr(entity, TypeInfo()));
    equation.pattern->gen_pattern(entity, temp);
    return expr->gen_expr(entity, typeinfo);
}

string LambdaExpr::gen_expr(FuncEntity &entity, const TypeInfo &typeinfo) const {
    assertt(!typeinfo.empty());
    assertt(typeinfo.is_function());
    assertt(typeinfo.args_size() == parameters.size());

    entity.code().add_header("functional");

    string params;
    for (size_t i = 0; i < parameters.size(); ++i) {
        if (i == 0) {
            params += typeinfo[i].to_str_as_arg() + parameters[i];
        } else {
            params += ", " + typeinfo[i].to_str_as_arg() + parameters[i];
        }
    }

    auto temp = entity.gen_temp();
    entity.add_expr("std::function $ = [&] ($) {", temp, params).add_indent()
        .add_expr("return $;", expr->gen_expr(entity, typeinfo[-1])).sub_indent()
        .add_expr("};")
    ;

    return temp;
}

// --- others ---
Type *FuncType::result_type() const {
    assertt(!types.empty());
    return types.back().get();
}

bool DataTypeDef::is_predefined() const {
    return local_ntype_mapping.count(decl_type->main_name()) || local_ttype_mapping.count(decl_type->main_name());
}

bool FunctionDef::is_predefined() const {
    static set<string> predefined_func {
        "length", "take", "drop"
    };
    return predefined_func.count(name);
}
} // namespace hol2cpp
