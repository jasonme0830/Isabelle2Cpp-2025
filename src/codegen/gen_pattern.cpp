#include "codegen.hpp"
#include "../optimizer/optimizer.hpp"

#include <regex>

using namespace std;

namespace hol2cpp {
void Expr::gen_pattern(FuncEntity &, const string &) const {
    throw CodegenError("failed call Expr::gen_pattern(...)");
}

void IntegralExpr::gen_pattern(FuncEntity &func, const string &prev) const {
    func.add_pattern_cond("$ == $", prev, value);
}

void VarExpr::gen_pattern(FuncEntity &func, const string &prev) const {
    if (name == func.name()) {
        assert_true(func.args_size() == 0);
    } else if (name == "True") {
        func.add_pattern_cond("$ == true", prev);
    } else if (name == "False") {
        func.add_pattern_cond("$ == false", prev);
    } else if (name == "Nil") {
        func.add_pattern_cond("$.empty()", prev);
    } else if (name == "None") {
        func.add_pattern_cond("!$.has_value()", prev);
    }

    // for constructors
    else if (auto datatype = func.code().find_datatype_by_cons(name)) {
        func.add_pattern_cond("$.is_$()", prev, name);
    }

    // for variables
    else {
        static std::regex arg_regex(R"(arg[1-9][0-9]*)");
        if (std::regex_match(prev, arg_regex)) {
            func.varrm_mapping()[name] = prev;
        } else {
            func.unused_varrm_count()[name] = func.delay_declarations().size();
            func.add_delay_declaration("auto $ = $;", name, prev);
        }
    }
}

void ConsExpr::gen_pattern(FuncEntity &func, const string &prev) const {
    if (constructor == func.name()) {
        assert_true(args.size() == func.args_size());
        for (size_t i = 0; i < args.size(); ++i) {
            args[i]->gen_pattern(func, "arg" + to_string(i + 1));
        }
    } else if (constructor == "Suc") {
        func.add_pattern_cond("$ != 0", prev);
        args[0]->gen_pattern(func, enclose_expr(prev) + " - 1");
    } else if (constructor == "Some") {
        func.add_pattern_cond("$.has_value()", prev);
        args[0]->gen_pattern(func, prev + ".value()");
    } else if (constructor == "Pair") {
        assert_true(args.size() == 2);
        args[0]->gen_pattern(func, prev + ".first");
        args[1]->gen_pattern(func, prev + ".second");
    }

    // for List
    else if (constructor == "Cons") {
        if (!theOptimizer.option().move_list) {
            func.add_pattern_cond("!$.empty()", prev);
            args[0]->gen_pattern(func, prev + ".front()");

            if (dynamic_cast<VarExpr *>(args[1].get())) {
                args[1]->gen_pattern(func, "decltype(" + prev + "){std::next(" + prev + ".begin()), " + prev + ".end()}");
            } else {
                auto temp = func.gen_temp();
                func.add_pattern("decltype($) $(std::next($).begin(), $.end());", prev, temp, prev, prev);
                args[1]->gen_pattern(func, temp);
            }
        } else {
            auto n = 1;
            auto rhs = args[1].get();
            while (auto cons_expr = dynamic_cast<ConsExpr *>(rhs)) {
                n += 1;
                rhs = cons_expr->args[1].get();
            }

            // the last rhs can only be Nil or variable
            auto var_expr = dynamic_cast<VarExpr *>(rhs);
            if (var_expr->name == "Nil") {
                func.add_pattern_cond("$.size() == $", prev, n);
            } else {
                func.add_pattern_cond("$.size() >= $", prev, n);
            }

            args[0]->gen_pattern(func, prev + ".front()");

            rhs = args[1].get();
            for (int i = 1; i < n; ++i) {
                auto cons_expr = reinterpret_cast<ConsExpr *>(rhs);
                cons_expr->args[0]->gen_pattern(func, "*std::next($.begin(), $)"_fs.format(prev, i));
                rhs = cons_expr->args[1].get();
            }

            if (var_expr->name != "Nil") {
                func.add_delay_declaration("$.erase($.begin(), std::next($.begin(), $));", prev, prev, prev, n);
                var_expr->gen_pattern(func, "std::move($)"_fs.format(prev));
            }
        }
    }

    // for Datatype
    else if (auto datatype = func.code().find_datatype_by_cons(constructor)) {
        func.add_pattern_cond("$.is_$()", prev, constructor);
        auto pos = datatype->pos_of_cons(constructor);
        for (size_t i = 0; i < args.size(); ++i) {
            args[i]->gen_pattern(func, "$.as_$().p$()"_fs.format(prev, constructor, i + 1));
        }
    } else {
        throw CodegenError("failed call VarExpr::gen_pattern(...): no such name: " + constructor);
    }
}

void ListExpr::gen_pattern(FuncEntity &func, const string &prev) const {
    if (exprs.empty()) {
        VarExpr("Nil").gen_pattern(func, prev);
    } else {
        func.add_pattern_cond("$.size() == $", prev, exprs.size());
        for (std::size_t i = 0; i < exprs.size(); ++i) {
            exprs[i]->gen_pattern(func, "*std::next($.begin(), $)"_fs.format(prev, i));
        }
    }
}

void SetExpr::gen_pattern(FuncEntity &func, const string &prev) const {
    assert_true(exprs.empty());
    func.add_pattern_cond("$.empty()", prev);
}

void BinaryOpExpr::gen_pattern(FuncEntity &func, const string &prev) const {
    switch (op.type)
    {
        case Token::Type::Sharp: {
            ConsExpr cons_expr("Cons");
            cons_expr.args.push_back(move(this->lhs));
            cons_expr.args.push_back(move(this->rhs));
            cons_expr.gen_pattern(func, prev);

            lhs = move(cons_expr.args[0]);
            rhs = move(cons_expr.args[1]);
        }
            break;

        default:
            throw CodegenError("failed call BinaryOpExpr::gen_pattern(): unsupported binary operator " + op.value);
    }
}
} // namespace hol2cpp
