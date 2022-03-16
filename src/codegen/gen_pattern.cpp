#include "codegen.hpp"

#include <regex>

using namespace std;

namespace hol2cpp {
void Expr::gen_pattern(FuncEntity &, const string &) const {
    throw CodegenError("failed call Expr::gen_pattern(...)");
}

void IntegralExpr::gen_pattern(FuncEntity &entity, const string &prev) const {
    entity.add_pattern_cond("$ == $", prev, value);
}

void VarExpr::gen_pattern(FuncEntity &entity, const string &prev) const {
    if (name == entity.name()) {
        assert(entity.args_size() == 0);
    } else if (name == "True") {
        entity.add_pattern_cond("$ == true", prev);
    } else if (name == "False") {
        entity.add_pattern_cond("$ == false", prev);
    } else if (name == "Nil") {
        entity.add_pattern_cond("$.empty()", prev);
    } else if (name == "None") {
        entity.add_pattern_cond("!$.has_value()", prev);
    }

    // for constructors
    else if (auto data_type = entity.code().find_data_type_by_cons(name)) {
        entity.add_pattern_cond("$.is_$()", prev, name);
    }

    // for variables
    else {
        static std::regex arg_regex(R"(arg[1-9][0-9]*)");
        if (std::regex_match(prev, arg_regex)) {
            entity.varrm_mapping()[name] = prev;
        } else {
            entity.unused_varrm_count()[name] = entity.statements().back().size();
            // entity.add_pattern("auto &&$ = $;", name, prev);
            entity.add_pattern("auto $ = $;", name, prev);
        }
    }
}

void ConsExpr::gen_pattern(FuncEntity &entity, const string &prev) const {
    if (constructor == entity.name()) {
        assert(args.size() == entity.args_size());
        for (size_t i = 0; i < args.size(); ++i) {
            args[i]->gen_pattern(entity, "arg" + to_string(i + 1));
        }
    } else if (constructor == "Suc") {
        entity.add_pattern_cond("$ != 0", prev);
        args[0]->gen_pattern(entity, enclose_expr(prev) + " - 1");
    } else if (constructor == "Some") {
        entity.add_pattern_cond("$.has_value()", prev);
        args[0]->gen_pattern(entity, prev + ".value()");
    } else if (constructor == "Pair") {
        assert(args.size() == 2);
        args[0]->gen_pattern(entity, prev + ".first");
        args[1]->gen_pattern(entity, prev + ".second");
    }

    // for List
    else if (constructor == "Cons") {
        entity.add_pattern_cond("!$.empty()", prev);
        args[0]->gen_pattern(entity, prev + ".front()");

        if (dynamic_cast<VarExpr *>(args[1].get())) {
            args[1]->gen_pattern(entity, "decltype(" + prev + "){std::next(" + prev + ".begin()), " + prev + ".end()}");
        } else {
            auto temp = entity.gen_temp();
            entity.add_pattern("decltype($) $(std::next($).begin(), $.end());", prev, temp, prev, prev);
            args[1]->gen_pattern(entity, temp);
        }
    }

    // for Datatype
    else if (auto data_type = entity.code().find_data_type_by_cons(constructor)) {
        entity.add_pattern_cond("$.is_$()", prev, constructor);
        auto pos = data_type->pos_of_cons(constructor);
        for (size_t i = 0; i < args.size(); ++i) {
            args[i]->gen_pattern(entity, "$.as_$().p$()"_fs.format(prev, constructor, i + 1));
        }
    } else {
        throw CodegenError("failed call VarExpr::gen_pattern(...): no such name: " + constructor);
    }
}

void ListExpr::gen_pattern(FuncEntity &entity, const string &prev) const {
    if (exprs.empty()) {
        VarExpr("Nil").gen_pattern(entity, prev);
    } else {
        entity.add_pattern_cond("$.size() == $", prev, exprs.size());
        for (std::size_t i = 0; i < exprs.size(); ++i) {
            exprs[i]->gen_pattern(entity, "*std::next($.begin(), $)"_fs.format(prev, i));
        }
    }
}

void SetExpr::gen_pattern(FuncEntity &entity, const string &prev) const {
    assert(exprs.empty());
    entity.add_pattern_cond("$.empty()", prev);
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
} // namespace hol2cpp