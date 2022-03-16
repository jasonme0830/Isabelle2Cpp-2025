#include "codegen.hpp"

using namespace std;

namespace hol2cpp {
string IntegralExpr::gen_expr(FuncEntity &, const TypeInfo &) const {
    return value;
}

string VarExpr::gen_expr(FuncEntity &entity, const TypeInfo &typeinfo) const {
    // for bool
    if (name == "True") {
        return "true";
    } else if (name == "False") {
        return "false";
    }

    // for list
    else if (name == "Nil") {
        return typeinfo.empty() ? "{}"s : (typeinfo.to_str() + "()");
    }

    // for option
    else if (name == "None") {
        return typeinfo.empty() ? "{}"s : (typeinfo.to_str() + "()");
    }

    // for user-defined datatypes
    else if (entity.code().find_data_type_by_cons(name)) {
        return "$::$()"_fs.format(typeinfo.to_str(), name);
    }

    // for variables
    else {
        auto &varrm_mapping = entity.varrm_mapping();
        auto it = varrm_mapping.find(name);
        if (it != varrm_mapping.end()) {
            return it->second;
        } else {
            entity.unused_varrm_count().erase(name);
            return name;
        }
    }
}

string ConsExpr::gen_expr(FuncEntity &entity, const TypeInfo &typeinfo) const {
    // for tail calls
    if (constructor == entity.name()) {
        string expr = constructor + '(';
        assert(entity.args_size() == args.size());
        for (size_t i = 0; i < args.size(); ++i) {
            if (i == 0) {
                expr += args[i]->gen_expr(entity, entity.typeinfos()[i]);
            } else {
                expr += ", " + args[i]->gen_expr(entity, entity.typeinfos()[i]);
            }
        }
        return expr + ')';
    }

    // for common calls
    else if (auto func = entity.code().find_func_entity(constructor)) {
        string expr = constructor + '(';
        assert(func->args_size() == args.size());
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
    }

    // for nat
    else if (constructor == "Suc") {
        assert(args.size() == 1);
        auto expr = args[0]->gen_expr(entity, typeinfo);
        return "(" + expr + ") + 1";
    }

    // for option
    else if (constructor == "Some") {
        assert(args.size() == 1);
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

    // for list
    else if (constructor == "Cons") {
        assert(args.size() == 2);
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
        assert(args.size() == 1);
        auto xs = args[0]->gen_expr(entity, typeinfo);
        if (xs == "{}") {
            return "0";
        } else {
            return xs + ".size()";
        }
    } else if (constructor == "take") {
        assert(args.size() == 2);
        auto n = args[0]->gen_expr(entity, TypeInfo("nat"));
        auto xs = args[1]->gen_expr(entity, TypeInfo());
        return "decltype($){ $.begin(), std::next($.begin(), $) }"_fs.format(xs, xs, xs, n);
    } else if (constructor == "drop") {
        assert(args.size() == 2);
        auto n = args[0]->gen_expr(entity, TypeInfo("nat"));
        auto xs = args[1]->gen_expr(entity, TypeInfo());
        return "decltype($){ std::next($.begin(), $), $.end() }"_fs.format(xs, xs, n, xs);
    } else if (constructor == "append") {
        assert(args.size() == 2);
        auto l = args[0]->gen_expr(entity, typeinfo);
        auto r = args[1]->gen_expr(entity, typeinfo);
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
                .add_expr("$.splice($.end(), $);", temp0, temp0, temp1);
            ;
            return temp0;
        }
    } else if (constructor == "upto") {
        auto elem_typeinfo = typeinfo.empty() ? TypeInfo() : typeinfo[0];

        auto start = entity.gen_temp();
        entity.add_expr("auto $ = $;", start, args[0]->gen_expr(entity, elem_typeinfo));

        auto end = entity.gen_temp();
        entity.add_expr("auto $ = $;", end, args[1]->gen_expr(entity, elem_typeinfo));

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
    } else if (constructor == "upt") {
        auto elem_typeinfo = typeinfo.empty() ? TypeInfo() : typeinfo[0];

        auto start = entity.gen_temp();
        entity.add_expr("auto $ = $;", start, args[0]->gen_expr(entity, elem_typeinfo));

        auto end = entity.gen_temp();
        entity.add_expr("auto $ = $;", end, args[1]->gen_expr(entity, elem_typeinfo));

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

    // for If-expression
    else if (constructor == "If") {
        assert(args.size() == 3);
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

    // for pairs
    else if (constructor == "Pair") {
        assert(args.size() == 2);
        return "std::make_pair(" + args[0]->gen_expr(entity, typeinfo.arguments.front())
            + ", " + args[1]->gen_expr(entity, typeinfo[1]) + ")"
        ;
    }

    // for user-defined datatypes
    else if (auto data_type = entity.code().find_data_type(typeinfo.name)) {
        std::string res = "$::$("_fs.format(typeinfo.to_str(), constructor);

        function trans = [&](const string &arg_type) {
            return typeinfo[data_type->find_argument_type(arg_type)];
        };

        vector<string> arguments;
        auto &abstracts = data_type->abstracts()[data_type->pos_of_cons(constructor)];
        for (size_t i = 0; i < abstracts.size(); ++i) {
            arguments.push_back(args[i]->gen_expr(entity, abstracts[i]->apply(trans)));
        }

        auto temp = entity.gen_temp();
        entity.add_expr("auto $ = $::$(", temp, typeinfo.to_str(), constructor).add_indent();
        for (size_t i = 0; i < arguments.size(); ++i) {
            if (i == 0) {
                entity.add_expr(arguments[i]);
            } else {
                entity.app_last_stmt(", " + arguments[i]);
            }
        }
        entity.sub_indent().add_expr(");");
        return temp;
    }

    // for ShortDef
    else if (auto short_def = entity.code().get_short_def(constructor)) {
        assert(short_def->parameters.size() == args.size());
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
                .add_expr("$.merge($);", lv, rv);
            ;
            return rv;
        }
        case Token::Type::Union: {
            auto l = lhs->gen_expr(entity, typeinfo);
            auto r = rhs->gen_expr(entity, typeinfo);

            auto lv = entity.gen_temp();
            auto rv = entity.gen_temp();
            entity
                .add_expr("auto $ = $;", lv, l)
                .add_expr("auto $ = $;", rv, r)
                .add_expr("$.merge($);", lv, rv);
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

        default: {
            assert(bop_mapping.count(op.type));

            return "$ $ $"_fs.format(
                enclose_expr(lhs->gen_expr(entity, TypeInfo())),
                bop_mapping.at(op.type),
                enclose_expr(rhs->gen_expr(entity, TypeInfo()))
            );
        }
    }
    throw CodegenError("failed call BinaryOpExpr::gen_expr(...): unsupported binary operator " + op.value);
}

string CaseExpr::gen_expr(FuncEntity &entity, const TypeInfo &typeinfo) const {
    auto temp0 = entity.gen_temp();
    entity.add_expr("auto $ = ([&] {", temp0).add_indent();

    auto e = expr->gen_expr(entity, TypeInfo());
    auto temp1 = entity.gen_temp();
    entity.add_expr("auto $ = $;\n", temp1, e);

    for (size_t i = 0; i < equations.size(); ++i) {
        if (i) {
            entity.app_last_stmt("\n");
        }

        auto condition_cnt = entity.condition_count();
        entity.add_expr("// $", equations[i].raw_str);
        equations[i].pattern->gen_pattern(entity, temp1);
        entity.add_expr("return $;", equations[i].expr->gen_expr(entity, typeinfo));
        entity.close_sub_equation(condition_cnt);
    }

    if (entity.statements().back().back().back() == '}') {
        entity.app_last_stmt(" else { // auto-generated for -Wreturn-type");
        entity.add_indent().add_expr("std::abort();").sub_indent().add_expr("}");
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
    assert(!typeinfo.empty());
    assert(typeinfo.is_function());
    assert(typeinfo.args_size() == parameters.size());

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
    entity.add_expr("std::function $ = [=] ($) {", temp, params).add_indent()
        .add_expr("return $;", expr->gen_expr(entity, typeinfo[-1])).sub_indent()
        .add_expr("};")
    ;

    return temp;
}
} // namespace hol2cpp