#include "../utility/config.hpp"
#include "codegen.hpp"

using namespace std;

namespace hol2cpp {
void
Expr::gen_pattern(FuncEntity&, const string&) const
{
  throw CodegenError("failed call Expr::gen_pattern(...)");
}

void
IntegralExpr::gen_pattern(FuncEntity& func, const string& prev) const
{
  func.add_pattern_cond("$ == $", prev, value);
}

void
VarExpr::gen_pattern(FuncEntity& func, const string& prev) const
{
  if (name == func.name()) {
    assert_true(func.args_size() == 0);
  }

  else if (name == "True") {
    func.add_pattern_cond("$ == true", prev);
  } else if (name == "False") {
    func.add_pattern_cond("$ == false", prev);
  }

  else if (name == "Nil") {
    func.add_pattern_cond("$.empty()", prev);
  }
  
  else if (name == "Zero"){
    func.add_pattern_cond("$ == $", prev, "0");
  }

  else if (name == "None") {
    func.add_pattern_cond("!$.has_value()", prev);
  }

  // for constructors
  else if (auto datatype = func.code().find_datatype_by_cons(name)) {
    func.add_pattern_cond("$.is_$()", prev, name);
  }

  // for variables
  else {
      func.decl_variable(name, prev);
  }
}

void
ConsExpr::gen_pattern(FuncEntity& func, const string& prev) const
{
  if (constructor == func.name()) {
    if (args.size() == func.args_size()) {
      for (size_t i = 0; i < args.size(); ++i) {
        args[i]->gen_pattern(func, "arg" + to_string(i + 1));
      }
    } else { // for uncurry and will only be called once at the first pattern
      assert_true(theConfig.uncurry());
      assert_true(args.size() < func.args_size());
      assert_true(func.statements().back().empty());
    }
  }

  else if (constructor == "Suc") {
    func.add_pattern_cond("$ != 0", prev);
    args[0]->gen_pattern(func, enclose_if_needed(prev) + " - 1");
  }

  else if (constructor == "Some") {
    func.add_pattern_cond("$.has_value()", prev);
    args[0]->gen_pattern(func, prev + ".value()");
  }

  else if (constructor == "Pair") {
    assert_true(args.size() == 2);
    args[0]->gen_pattern(func, prev + ".first");
    args[1]->gen_pattern(func, prev + ".second");
  }

  // for List
  else if (constructor == "Cons") {
    // if (!theConfig.move_list()) 
    // if(func.func_recu_class() != 1)
    // if(func.func_gen_mode() == 0)
    if(!theConfig.close_moveStd())
    {
      func.add_pattern_cond("!$.empty()", prev);
      args[0]->gen_pattern(func, prev + ".front()");

      auto type = expr_type->gen_typeinfo(func).to_str();
      if (dynamic_cast<VarExpr*>(args[1].get())) {
        if (theConfig.use_deque()) {
          args[1]->gen_pattern(
            func, "$($.begin() + 1, $.end())"_fs.format(type, prev, prev));
        } else {
          args[1]->gen_pattern(
            func,
            "$(std::next($.begin()), $.end())"_fs.format(type, prev, prev));
        }
      } else {
        auto temp = func.gen_temp();

        if (theConfig.use_deque()) {
          func.add_pattern(
            "$ $($.begin() + 1, $.end());", type, temp, prev, prev);
        } else {
          func.add_pattern(
            "$ $(std::next($.begin()), $.end());", type, temp, prev, prev);
        }

        args[1]->gen_pattern(func, temp);
      }
    } else {
      auto n = 1;
      auto rhs = args[1].get();
      while (auto cons_expr = dynamic_cast<ConsExpr*>(rhs)) {
        n += 1;
        rhs = cons_expr->args[1].get();
      }

      // the last rhs can only be Nil or variable
      auto var_expr = dynamic_cast<VarExpr*>(rhs);
      if (var_expr->name == "Nil") {
        func.add_pattern_cond("$.size() == $", prev, n);
      } else {
        func.add_pattern_cond("$.size() >= $", prev, n);
      }

      args[0]->gen_pattern(func, prev + ".front()");

      rhs = args[1].get();
      for (int i = 1; i < n; ++i) {
        auto cons_expr = reinterpret_cast<ConsExpr*>(rhs);

        if (theConfig.use_deque()) {
          cons_expr->args[0]->gen_pattern(func, "$[$]"_fs.format(prev, i));
        } else {
          cons_expr->args[0]->gen_pattern(
            func, "*std::next($.begin(), $)"_fs.format(prev, i));
        }

        rhs = cons_expr->args[1].get();
      }

      if (var_expr->name != "Nil") {
        if (theConfig.use_deque()) {
          func.add_delay_statement(
            "$.erase($.begin(), $.begin() + $);", prev, prev, prev, n);
        } else {
          func.add_delay_statement(
            "$.erase($.begin(), std::next($.begin(), $));",
            prev,
            prev,
            prev,
            n);
        }

        // var_expr->gen_pattern(func, "std::move($)"_fs.format(prev));
        var_expr->gen_pattern(func, "$"_fs.format(prev));
      }
    }
  }

  // for Datatype
  else if (auto datatype = func.code().find_datatype_by_cons(constructor)) {
    func.add_pattern_cond("$.is_$()", prev, constructor);
    auto pos = datatype->pos_of_cons(constructor);
    auto& self = datatype->self();
    auto& components = datatype->components();
    for (size_t i = 0; i < args.size(); ++i) {
      switch (func.func_gen_mode())
      {
      case 0:
        args[i]->gen_pattern(func,"$.as_$().p$()"_fs.format(prev, constructor, i + 1));
        break;
      case 1:
        args[i]->gen_pattern(func,"$.as_$().p$()"_fs.format(prev, constructor, i + 1));
        break;
      case 2:
        if(components[pos][i] == self){
          args[i]->gen_pattern(func,"std::move(*$.as_$().p$_)"_fs.format(prev, constructor, i + 1));
        }else{
          args[i]->gen_pattern(func,"std::move($.as_$().p$_)"_fs.format(prev, constructor, i + 1));
        }
        break;
      default:
        args[i]->gen_pattern(func,"$.as_$().p$()"_fs.format(prev, constructor, i + 1));
        break;
      }
    }
  } 
  
  else {
    throw CodegenError("failed call VarExpr::gen_pattern(...): no such name: " +
                       constructor);
  }
}

void
ListExpr::gen_pattern(FuncEntity& func, const string& prev) const
{
  if (exprs.empty()) {
    VarExpr("Nil").gen_pattern(func, prev);
  } else {
    func.add_pattern_cond("$.size() == $", prev, exprs.size());
    for (std::size_t i = 0; i < exprs.size(); ++i) {
      if (theConfig.use_deque()) {
        exprs[i]->gen_pattern(func, "$[$]"_fs.format(prev, i));
      } else {
        exprs[i]->gen_pattern(func,
                              "*std::next($.begin(), $)"_fs.format(prev, i));
      }
    }
  }
}

void
SetExpr::gen_pattern(FuncEntity& func, const string& prev) const
{
  assert_true(exprs.empty());
  func.add_pattern_cond("$.empty()", prev);
}

void
BinaryOpExpr::gen_pattern(FuncEntity& func, const string& prev) const
{
  switch (op.type) {
    case Token::Type::Sharp: {
      ConsExpr cons_expr("Cons");
      cons_expr.args.push_back(move(this->lhs));
      cons_expr.args.push_back(move(this->rhs));
      cons_expr.gen_pattern(func, prev);

      lhs = move(cons_expr.args[0]);
      rhs = move(cons_expr.args[1]);
    } break;

    default:
      throw CodegenError("failed call BinaryOpExpr::gen_pattern(): unsupported "
                         "binary operator " +
                         op.value);
  }
}
} // namespace hol2cpp
