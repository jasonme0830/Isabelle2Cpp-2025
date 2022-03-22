#include "codegen.hpp"
#include "../optimizer/optimizer.hpp"

using namespace std;

namespace hol2cpp {
string IntegralExpr::gen_expr(FuncEntity &, const TypeInfo &) const {
    return value;
}

string VarExpr::gen_expr(FuncEntity &func, const TypeInfo &typeinfo) const {
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
    else if (func.code().find_datatype_by_cons(name)) {
        return "$::$()"_fs.format(typeinfo.to_str(), name);
    }

    // for variables
    else {
        string var;
        auto &varrm_mapping = func.varrm_mapping();
        auto it = varrm_mapping.find(name);
        if (it != varrm_mapping.end()) {
            var = it->second;
        } else {
            func.unused_varrm_count().erase(name);
            var = name;
        }

        if (theOptimizer.option().list_move && movable && typeinfo.name == "std::list") {
            return "std::move($)"_fs.format(var);
        } else {
            return var;
        }
    }
}

string ConsExpr::gen_expr(FuncEntity &func, const TypeInfo &typeinfo) const {
    // for recursive calls
    if (constructor == func.name()) {
        string expr = constructor + '(';
        assert_true(func.args_size() == args.size());
        for (size_t i = 0; i < args.size(); ++i) {
            if (i == 0) {
                expr += args[i]->gen_expr(func, func.typeinfos()[i]);
            } else {
                expr += ", " + args[i]->gen_expr(func, func.typeinfos()[i]);
            }
        }
        return expr + ')';
    }

    // for common calls
    else if (auto other_func = func.code().find_func_entity(constructor)) {
        string expr = constructor + '(';
        assert_true(other_func->args_size() == args.size());
        for (size_t i = 0; i < args.size(); ++i) {
            if (i == 0) {
                expr += args[i]->gen_expr(func, other_func->typeinfos()[i]);
            } else {
                expr += ", " + args[i]->gen_expr(func, other_func->typeinfos()[i]);
            }
        }
        return expr + ')';
    }

    // for nat
    else if (constructor == "Suc") {
        assert_true(args.size() == 1);
        auto expr = args[0]->gen_expr(func, typeinfo);
        return "(" + expr + ") + 1";
    }

    // for option
    else if (constructor == "Some") {
        assert_true(args.size() == 1);
        if (typeinfo.empty()) {
            auto expr = args.front()->gen_expr(func, typeinfo);
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
            auto expr = args.front()->gen_expr(func, arg_type);
            return "std::make_optional<$>($)"_fs.format(
                arg_type.to_str(), expr
            );
        }
    }

    // for list
    else if (constructor == "Cons") {
        assert_true(args.size() == 2);
        auto x = args[0]->gen_expr(func, typeinfo[0]);
        auto xs = args[1]->gen_expr(func, typeinfo);
        if (xs == "{}") {
            if (typeinfo.empty()) {
                return "std::list<decltype($)>{$}"_fs.format(x, x);
            } else {
                return "${$};"_fs.format(typeinfo.to_str(), x);
            }
        } else {
            auto temp = func.gen_temp();
            func
                .add_expr("auto $ = $;", temp, xs)
                .add_expr("$.push_front($);", temp, x)
            ;

            if (theOptimizer.option().list_move) {
                return "std::move($)"_fs.format(temp);
            } else {
                return temp;
            }
        }
    } else if (constructor == "length") {
        assert_true(args.size() == 1);
        auto xs = args[0]->gen_expr(func, typeinfo);
        if (xs == "{}") {
            return "0";
        } else {
            return xs + ".size()";
        }
    } else if (constructor == "take") {
        assert_true(args.size() == 2);
        auto n = args[0]->gen_expr(func, TypeInfo("nat"));
        auto xs = unmove_expr(args[1]->gen_expr(func, typeinfo));
        return "decltype($){ $.begin(), std::next($.begin(), $) }"_fs.format(xs, xs, xs, n);
    } else if (constructor == "drop") {
        assert_true(args.size() == 2);
        auto n = args[0]->gen_expr(func, TypeInfo("nat"));
        auto xs = unmove_expr(args[1]->gen_expr(func, typeinfo));
        return "decltype($){ std::next($.begin(), $), $.end() }"_fs.format(xs, xs, n, xs);
    } else if (constructor == "append") {
        assert_true(args.size() == 2);
        auto l = args[0]->gen_expr(func, typeinfo);
        auto r = args[1]->gen_expr(func, typeinfo);
        if (l == "{}" && r == "{}") {
            return typeinfo.empty() ? "{}"s : (typeinfo.to_str() + "()");
        } else if (l == "{}") {
            return r;
        } else if (r == "{}") {
            return l;
        } else {
            auto temp0 = func.gen_temp();
            auto temp1 = func.gen_temp();
            func
                .add_expr("auto $ = $;", temp0, l)
                .add_expr("auto $ = $;", temp1, r)
                .add_expr("$.splice($.end(), $);", temp0, temp0, temp1);
            ;
            return temp0;
        }
    } else if (constructor == "upto") {
        auto elem_typeinfo = typeinfo.empty() ? TypeInfo() : typeinfo[0];

        auto start = func.gen_temp();
        func.add_expr("auto $ = $;", start, args[0]->gen_expr(func, elem_typeinfo));

        auto end = func.gen_temp();
        func.add_expr("auto $ = $;", end, args[1]->gen_expr(func, elem_typeinfo));

        auto res = func.gen_temp();
        if (typeinfo.empty()) {
            func.add_expr("std::list<decltype($)> $;", start, res);
        } else {
            func.add_expr("$ $;", typeinfo.to_str(), res);
        }

        auto i = func.gen_temp();
        func.add_expr("for (auto $ = $; $ <= $; ++$) {", i, start, i, end, i).add_indent()
            .add_expr("$.push_back($);", res, i).sub_indent()
            .add_expr("}")
        ;

        return res;
    } else if (constructor == "upt") {
        auto elem_typeinfo = typeinfo.empty() ? TypeInfo() : typeinfo[0];

        auto start = func.gen_temp();
        func.add_expr("auto $ = $;", start, args[0]->gen_expr(func, elem_typeinfo));

        auto end = func.gen_temp();
        func.add_expr("auto $ = $;", end, args[1]->gen_expr(func, elem_typeinfo));

        auto res = func.gen_temp();
        if (typeinfo.empty()) {
            func.add_expr("std::list<decltype($)> $;", start, res);
        } else {
            func.add_expr("$ $;", typeinfo.to_str(), res);
        }

        auto i = func.gen_temp();
        func.add_expr("for (auto $ = $; $ < $; ++$) {", i, start, i, end, i).add_indent()
            .add_expr("$.push_back($);", res, i).sub_indent()
            .add_expr("}")
        ;

        return res;
    }

    // for If-expression
    else if (constructor == "If") {
        assert_true(args.size() == 3);
        if (typeinfo.empty()) {
            auto texpr = args[1]->gen_expr(func, typeinfo);
            auto fexpr = args[2]->gen_expr(func, typeinfo);
            auto ce = args[0]->gen_expr(func, TypeInfo("bool"));
            return "$ ? $ : $"_fs.format(ce, texpr, fexpr);
        } else {
            auto ce = args[0]->gen_expr(func, TypeInfo("bool"));
            auto res = func.gen_temp();
            func
                .add_expr("$ $;", typeinfo.to_str(), res)
                .add_expr("if ($) {", ce).add_indent()
                    .add_expr("$ = $;", res, args[1]->gen_expr(func, typeinfo)).sub_indent()
                .add_expr("} else {").add_indent()
                    .add_expr("$ = $;", res, args[2]->gen_expr(func, typeinfo)).sub_indent()
                .add_expr("}")
            ;
            return res;
        }
    }

    // for pairs
    else if (constructor == "Pair") {
        assert_true(args.size() == 2);
        return "std::make_pair(" + args[0]->gen_expr(func, typeinfo.arguments.front())
            + ", " + args[1]->gen_expr(func, typeinfo[1]) + ")"
        ;
    }

    // for user-defined datatypes
    else if (auto datatype = func.code().find_datatype(typeinfo.name)) {
        std::string res = "$::$("_fs.format(typeinfo.to_str(), constructor);

        function trans = [&](const string &arg_type) {
            return typeinfo[datatype->find_argument_type(arg_type)];
        };

        vector<string> arguments;
        auto &abstracts = datatype->abstracts()[datatype->pos_of_cons(constructor)];
        for (size_t i = 0; i < abstracts.size(); ++i) {
            arguments.push_back(args[i]->gen_expr(func, abstracts[i]->apply(trans)));
        }

        auto temp = func.gen_temp();
        func.add_expr("auto $ = $::$(", temp, typeinfo.to_str(), constructor).add_indent();
        for (size_t i = 0; i < arguments.size(); ++i) {
            if (i == 0) {
                func.add_expr(arguments[i]);
            } else {
                func.app_last_stmt(", " + arguments[i]);
            }
        }
        func.sub_indent().add_expr(");");
        return temp;
    }

    // for ShortDef
    else if (auto short_def = func.code().get_short_def(constructor)) {
        assert_true(short_def->parameters.size() == args.size());
        for (size_t i = 0; i < args.size(); ++i) {
            func.add_expr("auto $ = $;", short_def->parameters[i], args[i]->gen_expr(func, TypeInfo()));
        }
        return short_def->expr->gen_expr(func, typeinfo);
    }

    // else as the common call without determined function
    else {
        string expr = constructor + '(';
        for (size_t i = 0; i < args.size(); ++i) {
            if (i == 0) {
                expr += args[i]->gen_expr(func, TypeInfo());
            } else {
                expr += ", " + args[i]->gen_expr(func, TypeInfo());
            }
        }
        return expr + ')';
    }
}

string ListExpr::gen_expr(FuncEntity &func, const TypeInfo &typeinfo) const {
    if (exprs.empty()) {
        return typeinfo.empty() ? "{}"s : (typeinfo.to_str() + "()");
    }

    string res;
    if (typeinfo.empty()) {
        res = "std::list<decltype($)>{"_fs.format(exprs.front()->gen_expr(func, typeinfo));
    } else {
        res = typeinfo.to_str() + '{';
    }

    for (size_t i = 0; i < exprs.size(); ++i) {
        if (i == 0) {
            res += exprs[i]->gen_expr(func, typeinfo[0]);
        } else {
            res += ", " + exprs[i]->gen_expr(func, typeinfo[0]);
        }
    }
    return res + '}';
}

string SetExpr::gen_expr(FuncEntity &func, const TypeInfo &typeinfo) const {
    if (exprs.empty()) {
        return typeinfo.empty() ? "{}"s : (typeinfo.to_str() + "()");
    }

    string res;
    if (typeinfo.empty()) {
        res = "std::set<decltype($)>{"_fs.format(exprs.front()->gen_expr(func, typeinfo));
    } else {
        res = typeinfo.to_str() + '{';
    }

    for (size_t i = 0; i < exprs.size(); ++i) {
        if (i == 0) {
            res += exprs[i]->gen_expr(func, typeinfo);
        } else {
            res += ", " + exprs[i]->gen_expr(func, typeinfo);
        }
    }
    return res + '}';
}

string BinaryOpExpr::gen_expr(FuncEntity &func, const TypeInfo &typeinfo) const {
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
            auto l = lhs->gen_expr(func, typeinfo);
            auto r = rhs->gen_expr(func, typeinfo);

            auto lv = func.gen_temp();
            auto rv = func.gen_temp();
            func
                .add_expr("auto $ = $;", lv, l)
                .add_expr("auto $ = $;", rv, r)
                .add_expr("$.merge($);", lv, rv);
            ;
            return rv;
        }
        case Token::Type::Union: {
            auto l = lhs->gen_expr(func, typeinfo);
            auto r = rhs->gen_expr(func, typeinfo);

            auto lv = func.gen_temp();
            auto rv = func.gen_temp();
            func
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
                rhs->gen_expr(func, typeinfo), lhs->gen_expr(func, typeinfo.arguments.front())
            );
        case Token::Type::NotIn:
            return "!$.count($)"_fs.format(
                rhs->gen_expr(func, typeinfo), lhs->gen_expr(func, typeinfo.arguments.front())
            );

        case Token::Type::Pow:
            func.code().add_header("cmath");
            return "std::pow($, $)"_fs.format(
                lhs->gen_expr(func, typeinfo), rhs->gen_expr(func, typeinfo)
            );

        /**
         * WARN: BE CAREFUL HERE!
        */
        case Token::Type::Equiv:
            if (auto val_expr = dynamic_cast<VarExpr *>(lhs.get())) {
                if (val_expr->name == "Nil") {
                    return "$.empty()"_fs.format(rhs->gen_expr(func, TypeInfo()));
                }
            }

            if (auto val_expr = dynamic_cast<VarExpr *>(rhs.get())) {
                if (val_expr->name == "Nil") {
                    return "$.empty()"_fs.format(lhs->gen_expr(func, TypeInfo()));
                }
            }

        default: {
            assert_true(bop_mapping.count(op.type));

            return "$ $ $"_fs.format(
                enclose_expr(lhs->gen_expr(func, TypeInfo())),
                bop_mapping.at(op.type),
                enclose_expr(rhs->gen_expr(func, TypeInfo()))
            );
        }
    }
    throw CodegenError("failed call BinaryOpExpr::gen_expr(...): unsupported binary operator " + op.value);
}

string CaseExpr::gen_expr(FuncEntity &func, const TypeInfo &typeinfo) const {
    auto temp0 = func.gen_temp();
    func.add_expr("auto $ = ([&] {", temp0).add_indent();

    auto e = expr->gen_expr(func, TypeInfo());
    auto temp1 = func.gen_temp();
    func.add_expr("auto $ = $;\n", temp1, e);

    for (size_t i = 0; i < equations.size(); ++i) {
        if (i) {
            func.app_last_stmt("\n");
        }

        auto condition_cnt = func.condition_count();
        func.add_expr("// $", equations[i].raw_str);
        equations[i].pattern->gen_pattern(func, temp1);
        func.add_expr("return $;", equations[i].expr->gen_expr(func, typeinfo));
        func.close_sub_equation(condition_cnt);
    }

    if (func.statements().back().back().back() == '}') {
        func.app_last_stmt(" else { // auto-generated for -Wreturn-type");
        func.add_indent().add_expr("std::abort();").sub_indent().add_expr("}");
    }

    func.sub_indent()
        .add_expr("})();")
    ;

    return temp0;
}

string LetinExpr::gen_expr(FuncEntity &func, const TypeInfo &typeinfo) const {
    auto temp = func.gen_temp();
    func.add_expr("auto $ = $;", temp, equation.expr->gen_expr(func, TypeInfo()));
    equation.pattern->gen_pattern(func, temp);
    return expr->gen_expr(func, typeinfo);
}

string LambdaExpr::gen_expr(FuncEntity &func, const TypeInfo &typeinfo) const {
    assert_true(!typeinfo.empty());
    assert_true(typeinfo.is_function());
    assert_true(typeinfo.args_size() == parameters.size());

    func.code().add_header("functional");

    string params;
    for (size_t i = 0; i < parameters.size(); ++i) {
        if (i == 0) {
            params += typeinfo[i].to_str_as_arg() + parameters[i];
        } else {
            params += ", " + typeinfo[i].to_str_as_arg() + parameters[i];
        }
    }

    auto temp = func.gen_temp();
    func.add_expr("std::function $ = [=] ($) {", temp, params).add_indent()
        .add_expr("return $;", expr->gen_expr(func, typeinfo[-1])).sub_indent()
        .add_expr("};")
    ;

    return temp;
}
} // namespace hol2cpp
