#include "codegen.hpp"
#include "../optimizer/optimizer.hpp"

using namespace std;

namespace hol2cpp {
string IntegralExpr::gen_expr_impl(FuncEntity &, const TypeInfo &) const {
    return value;
}

string VarExpr::gen_expr_impl(FuncEntity &func, const TypeInfo &typeinfo) const {
    // for `bool`
    if (name == "True") {
        return "true";
    } else if (name == "False") {
        return "false";
    }

    // for `list` and `option`
    else if (name == "Nil" || name == "None") {
        return typeinfo.to_str() + "()";
    }

    // for user-defined datatypes
    else if (func.code().find_datatype_by_cons(name)) {
        typeinfo.avoid_lack(func, name);

        // assume the typeinfo exits
        return "$::$()"_fs.format(typeinfo.to_str(), name);
    }

    // for variables
    else {
        auto var = func.get_variable(name);
        if (movable && typeinfo.movable()) {
            // movable is true only when move-list is enable
            return "std::move($)"_fs.format(var); // for move-list
        } else {
            return var;
        }
    }
}

string ConsExpr::gen_expr_impl(FuncEntity &func, const TypeInfo &typeinfo) const {
    // for recursive calls
    if (constructor == func.name()) {
        typeinfo.avoid_lack(func, constructor);
        assert_true(func.args_size() == args.size());

        return constructor + enclose(join(
            args, [&](const auto &arg) {
                return arg->gen_expr(func);
            }
        ));
    }

    // for nat
    else if (constructor == "Suc") {
        assert_true(args.size() == 1);
        auto expr = args[0]->gen_expr(func);
        return enclose_if_needed(expr) + "+ 1";
    }

    // for option
    else if (constructor == "Some") {
        assert_true(args.size() == 1);
        return "std::make_optional<$>($)"_fs.format(
            typeinfo[0].to_str(), args[0]->gen_expr(func)
        );
    }

    // for list
    else if (constructor == "Cons") {
        assert_true(args.size() == 2);
        auto x = args[0]->gen_expr(func);
        auto xs = args[1]->gen_expr(func);

        auto temp = func.gen_temp();
        func
            .add_expr("auto $ = $;", temp, xs)
            .add_expr("$.push_front($);", temp, x)
        ;

        if (theOptimizer.option().move_list) {
            return "std::move($)"_fs.format(temp);
        } else {
            return temp;
        }
    } else if (constructor == "length") {
        assert_true(args.size() == 1);
        return unmove_expr(args[0]->gen_expr(func)) + ".size()";
    } else if (constructor == "take") {
        assert_true(args.size() == 2);

        auto n = args[0]->gen_expr(func);
        auto xs = unmove_expr(args[1]->gen_expr(func));

        if (theOptimizer.option().use_deque) {
            return "$($.begin(), $.begin() + $)"_fs.format(
                typeinfo.to_str(), xs, xs, n
            );
        } else {
            return "$($.begin(), std::next($.begin(), $))"_fs.format(
                typeinfo.to_str(), xs, xs, n
            );
        }
    } else if (constructor == "drop") {
        assert_true(args.size() == 2);

        auto n = args[0]->gen_expr(func);
        auto xs = unmove_expr(args[1]->gen_expr(func));

        if (theOptimizer.option().use_deque) {
            return "$($.begin() + $, $.end())"_fs.format(typeinfo.to_str(), xs, n, xs);
        } else {
            return "$(std::next($.begin(), $), $.end())"_fs.format(typeinfo.to_str(), xs, n, xs);
        }
    } else if (constructor == "append") {
        assert_true(args.size() == 2);

        auto l = args[0]->gen_expr(func);
        auto r = args[1]->gen_expr(func);

        auto temp0 = func.gen_temp();
        auto temp1 = func.gen_temp();
        func
            .add_expr("auto $ = $;", temp0, l)
            .add_expr("auto $ = $;", temp1, r)
        ;

        if (theOptimizer.option().use_deque) {
            func.add_expr("$.insert($.end(), $.begin(), $.end());", temp0, temp0, temp1, temp1);
        } else {
            func.add_expr("$.splice($.end(), $);", temp0, temp0, temp1);
        }
        return temp0;
    } else if (constructor == "nth") {
        assert_true(args.size() == 2);

        auto l = args[0]->gen_expr(func);
        auto r = args[1]->gen_expr(func);

        auto temp0 = func.gen_temp();
        func.add_expr("auto $ = $;", temp0, l);

        if (theOptimizer.option().use_deque) {
            return "$[$]"_fs.format(temp0, r);
        } else {
            return "*std::next($.begin(), $)"_fs.format(temp0, r);
        }
    } else if (constructor == "upto") {
        auto start = func.gen_temp();
        func.add_expr("auto $ = $;", start, args[0]->gen_expr(func));

        auto end = func.gen_temp();
        func.add_expr("auto $ = $;", end, args[1]->gen_expr(func));

        auto res = func.gen_temp();
        func.add_expr("$ $;", typeinfo.to_str(), res);

        auto i = func.gen_temp();
        func.add_expr("for (auto $ = $; $ <= $; ++$) {", i, start, i, end, i).add_indent()
            .add_expr("$.push_back($);", res, i).sub_indent()
            .add_expr("}")
        ;

        return res;
    } else if (constructor == "upt") {
        auto start = func.gen_temp();
        func.add_expr("auto $ = $;", start, args[0]->gen_expr(func));

        auto end = func.gen_temp();
        func.add_expr("auto $ = $;", end, args[1]->gen_expr(func));

        auto res = func.gen_temp();
        func.add_expr("$ $;", typeinfo.to_str(), res);

        auto i = func.gen_temp();
        func.add_expr("for (auto $ = $; $ < $; ++$) {", i, start, i, end, i).add_indent()
            .add_expr("$.push_back($);", res, i).sub_indent()
            .add_expr("}")
        ;

        return res;
    } else if (constructor == "set") {
        assert_true(args.size() == 1);
        func.code().add_header("set");

        auto temp = func.gen_temp();
        auto list = args[0]->gen_expr(func);
        func.add_expr("auto $ = $;", temp, list);

        auto set_type = typeinfo.replace_with("std::set");
        return "$($.begin(), $.end())"_fs.format(set_type.to_str(), temp, temp);
    }

    // for If-expression
    else if (constructor == "If") {
        assert_true(args.size() == 3);

        auto cond = args[0]->gen_expr(func);
        auto res = func.gen_temp();

        func
            .add_expr("$ $;", typeinfo.to_str(), res)
            .add_expr("if ($) {", cond).add_indent()
                .add_expr("$ = $;", res, args[1]->gen_expr(func)).sub_indent()
            .add_expr("} else {").add_indent()
                .add_expr("$ = $;", res, args[2]->gen_expr(func)).sub_indent()
            .add_expr("}")
        ;

        return res;
    }

    // for pairs
    else if (constructor == "Pair") {
        assert_true(args.size() == 2);
        return "std::make_pair(" + args[0]->gen_expr(func)
            + ", " + args[1]->gen_expr(func) + ")"
        ;
    } else if (constructor == "fst") {
        assert_true(args.size() == 1);
        return "$.first"_fs.format(args[0]->gen_expr(func));
    }else if (constructor == "snd") {
        assert_true(args.size() == 1);
        return "$.second"_fs.format(args[0]->gen_expr(func));
    }

    // for user-defined datatypes
    else if (auto datatype = func.code().find_datatype_by_cons(constructor)) {
        typeinfo.avoid_lack(func, constructor);

        vector<string> arguments;
        for (auto &arg : args) {
            arguments.push_back(arg->gen_expr(func));
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

    // for common calls
    else if (auto other_func = func.code().find_func_entity(constructor)) {
        typeinfo.avoid_lack(func, constructor);
        assert_true(other_func->args_size() == args.size());

        return constructor + enclose(
            join(args, [&](auto &arg) {
                return arg->gen_expr(func);
            })
        );
    }

    // for ShortDef
    else if (auto short_def = func.code().get_short_def(constructor)) {
        assert_true(short_def->parameters.size() == args.size());
        for (size_t i = 0; i < args.size(); ++i) {
            func.add_expr("auto $ = $;", short_def->parameters[i], args[i]->gen_expr(func));
        }
        return short_def->expr->gen_expr(func);
    }

    // else as the common call without determined function
    else {
        // todo @xubo support currying
        auto foo = func.get_variable(constructor);
        return foo + enclose(join(
            args,
            [&](auto &expr) {
                return expr->gen_expr(func);
            }
        ));
    }
}

string ListExpr::gen_expr_impl(FuncEntity &func, const TypeInfo &typeinfo) const {
    return typeinfo.to_str() + enclose<'{', '}'>(join(
        exprs, [&](const auto &expr) {
            return expr->gen_expr(func);
        }
    ));
}

string SetExpr::gen_expr_impl(FuncEntity &func, const TypeInfo &typeinfo) const {
    return typeinfo.to_str() + enclose<'{', '}'>(join(
        exprs, [&](const auto &expr) {
            return expr->gen_expr(func);
        }
    ));
}

string BinaryOpExpr::gen_expr_impl(FuncEntity &func, const TypeInfo &typeinfo) const {
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
            auto l = lhs->gen_expr(func);
            auto r = rhs->gen_expr(func);

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
            auto l = lhs->gen_expr(func);
            auto r = rhs->gen_expr(func);

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
                rhs->gen_expr(func), lhs->gen_expr(func)
            );
        case Token::Type::NotIn:
            return "!$.count($)"_fs.format(
                rhs->gen_expr(func), lhs->gen_expr(func)
            );

        case Token::Type::Pow:
            func.code().add_header("cmath");
            return "std::pow($, $)"_fs.format(
                lhs->gen_expr(func), rhs->gen_expr(func)
            );

        /**
         * WARN: BE CAREFUL HERE!
         * `== Nil` is special
        */
        case Token::Type::Equiv:
            if (auto val_expr = dynamic_cast<VarExpr *>(lhs.get())) {
                if (val_expr->name == "Nil") {
                    return "$.empty()"_fs.format(unmove_expr(rhs->gen_expr(func)));
                }
            }

            if (auto val_expr = dynamic_cast<VarExpr *>(rhs.get())) {
                if (val_expr->name == "Nil") {
                    return "$.empty()"_fs.format(unmove_expr(lhs->gen_expr(func)));
                }
            }

        default: {
            assert_true(bop_mapping.count(op.type));

            return "$ $ $"_fs.format(
                enclose_if_needed(lhs->gen_expr(func)),
                bop_mapping.at(op.type),
                enclose_if_needed(rhs->gen_expr(func))
            );
        }
    }
    throw CodegenError("failed call BinaryOpExpr::gen_expr(...): unsupported binary operator " + op.value);
}

string CaseExpr::gen_expr_impl(FuncEntity &func, const TypeInfo &typeinfo) const {
    auto temp0 = func.gen_temp();
    func.add_expr("auto $ = ([&] {", temp0).add_indent();

    auto e = expr->gen_expr(func);
    auto temp1 = func.gen_temp();
    func.add_expr("auto $ = $;\n", temp1, e);

    auto is_last_equation = func.is_last_equation(); // for reduce-cond
    for (size_t i = 0; i < equations.size(); ++i) {
        func.is_last_equation(i == equations.size() - 1); // for reduce-cond

        if (i != 0) {
            func.app_last_stmt("\n");
        }
        auto condition_cnt = func.condition_count();
        func.add_expr("// $", equations[i].raw_str);

        equations[i].pattern->gen_pattern(func, temp1);
        func.close_pattern(); // for reduce-cond

        func.add_expr("return $;", equations[i].expr->gen_expr(func));
        func.close_sub_equation(condition_cnt);
    }
    func.is_last_equation(is_last_equation); // for reduce-cond

    if (func.statements().back().back().back() == '}') {
        func.app_last_stmt(" else { // auto-generated for -Wreturn-type");
        func.add_indent().add_expr("std::abort();").sub_indent().add_expr("}");
    }

    func.sub_indent()
        .add_expr("})();")
    ;

    return temp0;
}

string LetinExpr::gen_expr_impl(FuncEntity &func, const TypeInfo &typeinfo) const {
    auto temp = func.gen_temp();
    func.add_expr("auto $ = $;", temp, equation.expr->gen_expr(func));
    equation.pattern->gen_pattern(func, temp);
    func.close_pattern(); // for reduce-cond
    return expr->gen_expr(func);
}

string LambdaExpr::gen_expr_impl(FuncEntity &func, const TypeInfo &typeinfo) const {
    assert_true(!typeinfo.lack());
    assert_true(typeinfo.is_function());

    // todo @xubo support currying
    assert_true(typeinfo.args_size() == parameters.size());

    func.code().add_header("functional");

    string params;
    for (size_t i = 0; i < parameters.size(); ++i) {
        if (i != 0) {
            params += ", ";
        }
        params += typeinfo[i].to_str_as_arg() + parameters[i];
    }

    auto temp = func.gen_temp();
    func.add_expr("std::function $ = [=] ($) {", temp, params).add_indent()
        .add_expr("return $;", expr->gen_expr(func)).sub_indent()
        .add_expr("};")
    ;

    return temp;
}
} // namespace hol2cpp
