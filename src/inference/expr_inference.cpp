#include "inference.hpp"

using namespace std;

namespace hol2cpp {
void
TypeInference::analy_from_bottom(Expr& expr,
                                 const string& funcname,
                                 LambdaMap& lambda_types_map,
                                 LambdaMap& return_lambda_types_map)
{
  if (typeid(expr) == typeid(IntegralExpr)) {
    if (expr.expr_type != nullptr) {
      return;
    }

    IntegralExpr& trans = reinterpret_cast<IntegralExpr&>(expr);
    trans.expr_type = normal_nat_type;
  } else if (typeid(expr) == typeid(VarExpr)) {
    if (expr.expr_type != nullptr) {
      return;
    }

    // variables
    VarExpr& trans = reinterpret_cast<VarExpr&>(expr);
    auto var_elem =
      find_if(the_arg_type_mapping.begin(),
              the_arg_type_mapping.end(),
              [name = trans.name](auto& iter) { return name == iter.first; });

    if (var_elem != the_arg_type_mapping.end()) {
      expr.expr_type = var_elem->second.get()->clone();
      return;
    }

    // lambda param
    auto lambda_elem =
      find_if(lambda_types_map.begin(),
              lambda_types_map.end(),
              [name = trans.name](auto& iter) { return name == iter.first; });

    if (lambda_elem != lambda_types_map.end()) {
      expr.expr_type = gen_new_lambda_arg(); // gen lambda arg here
      lambda_elem->second = ref(expr.expr_type);
      return;
    }

    // constructor like Nil that only appears in expr
    auto cons_elem =
      find_if(the_dtypedef_mapping.begin(),
              the_dtypedef_mapping.end(),
              [name = trans.name](auto& iter) { return name == iter.first; });

    // functions like id in fold that only appears in expr
    auto func_elem =
      find_if(the_func_type_mapping.begin(),
              the_func_type_mapping.end(),
              [name = trans.name](auto& iter) { return name == iter.first; });

    if (cons_elem != the_dtypedef_mapping.end()) {
      expr.expr_type = cons_elem->second.get().decl_type->clone();
      type_mangling(expr.expr_type);
      return;
    } else if (func_elem != the_func_type_mapping.end()) {
      expr.expr_type = func_elem->second.get().type->clone();
      //?
      return;
    } else {
      expr.expr_type = normal_unknown_type;
    }
  } else if (typeid(expr) == typeid(ConsExpr)) {
    ConsExpr& trans = reinterpret_cast<ConsExpr&>(expr);

    // mark the type of sub-exprs
    for (size_t i = 0; i < trans.args.size(); ++i) {
      analy_from_bottom(
        *trans.args[i], funcname, lambda_types_map, return_lambda_types_map);
    }

    // infer expr's type
    if (the_dtypedef_mapping.find(trans.constructor) ==
        the_dtypedef_mapping.end()) {
      // it's a function apply
      auto funcdef_iter = the_func_type_mapping.find(trans.constructor);

      if (funcdef_iter != the_func_type_mapping.end()) {
        // call functions which are from the FunctionDef
        FunctionDef& funcdef = funcdef_iter->second.get();
        trans.expr_type = funcdef.type->clone();
        type_mangling(trans.expr_type);

        trans.expr_type = function_apply(trans.expr_type, trans.args);
      } else {
        // call functions which are from the pattern's args
        auto elem = find_if(the_arg_type_mapping.begin(),
                            the_arg_type_mapping.end(),
                            [name = trans.constructor](auto& iter) {
                              return name == iter.first;
                            }); // e.g. trans.constructor := f

        if (elem == the_arg_type_mapping.end()) {
          string info = funcname + ": cannot find the definition of <" +
                        trans.constructor + ">\n";
          throw info;
        }

        auto& functype = elem->second.get();
        trans.expr_type = functype->clone();
        // type_mangling(trans.expr_type);

        trans.expr_type = function_apply(trans.expr_type, trans.args);
      }
    } else {
      // it's a datatype's constructor
      DatatypeDef dtypedef =
        the_dtypedef_mapping.find(trans.constructor)->second.get().clone();
      dtypedef_mangling(dtypedef);

      // components has been mangled
      auto component = find_if(dtypedef.components.begin(),
                               dtypedef.components.end(),
                               [cons = trans.constructor](auto& com) {
                                 return com.constructor == cons;
                               });

      if (typeid(*dtypedef.decl_type) == typeid(NormalType)) {
        trans.expr_type = dtypedef.decl_type->clone();
      } else {
        // datatype's decl_type is TemplateType
        auto cons_iter = the_cons_type_mapping.find(trans.constructor);
        trans.expr_type = cons_iter->second->clone();
        type_mangling(trans.expr_type);

        trans.expr_type = function_apply(trans.expr_type, trans.args);
      }
    }
  } else if (typeid(expr) == typeid(BinaryOpExpr)) {
    BinaryOpExpr& trans = reinterpret_cast<BinaryOpExpr&>(expr);

    analy_from_bottom(
      *trans.lhs, funcname, lambda_types_map, return_lambda_types_map);
    analy_from_bottom(
      *trans.rhs, funcname, lambda_types_map, return_lambda_types_map);

    if (typeid(*trans.lhs) == typeid(VarExpr) &&
        typeid(*trans.rhs) == typeid(VarExpr)) {
      VarExpr& trans_lhs = dynamic_cast<VarExpr&>(*trans.lhs);
      VarExpr& trans_rhs = dynamic_cast<VarExpr&>(*trans.rhs);

      if (the_dtypedef_mapping.find(trans_lhs.name) !=
            the_dtypedef_mapping.end() ||
          the_dtypedef_mapping.find(trans_rhs.name) !=
            the_dtypedef_mapping.end()) {
        if (!is_mangled(trans_lhs.expr_type) &&
            is_mangled(trans_rhs.expr_type)) {
          template_type_map(trans_rhs.expr_type, trans_lhs.expr_type);
          template_type_ins(trans_rhs.expr_type);
        } else if (is_mangled(trans_lhs.expr_type) &&
                   !is_mangled(trans_rhs.expr_type)) {
          template_type_map(trans_lhs.expr_type, trans_rhs.expr_type);
          template_type_ins(trans_lhs.expr_type);
        } else {
          // do nothing
        }
      }
    }

    if (trans.op.value == "=") {
      trans.expr_type = normal_bool_type;
    } else {
      trans.expr_type = trans.lhs->expr_type->clone();
    }
  } else if (typeid(expr) == typeid(ListExpr)) {
    ListExpr& trans = reinterpret_cast<ListExpr&>(expr);
    // mark type
    for (auto& arg : trans.exprs) {
      analy_from_bottom(
        *arg, funcname, lambda_types_map, return_lambda_types_map);
    }
    // infer expr type for 'a list
    auto iter = the_dtypedef_mapping.find("Cons");

    if (iter == the_dtypedef_mapping.end()) {
      throw string("cannot find the definition of <Cons>\n");
    }
    DatatypeDef& dtypedef = iter->second.get();

    trans.expr_type = dtypedef.decl_type->clone();
    TemplateType& tmpltype = reinterpret_cast<TemplateType&>(*trans.expr_type);
    if (trans.exprs.size() > 0) {
      tmpltype.args[0] = trans.exprs[0]->expr_type->clone();
    }
  } else if (typeid(expr) == typeid(SetExpr)) {
    SetExpr& trans = reinterpret_cast<SetExpr&>(expr);
    // mark type
    for (auto& arg : trans.exprs) {
      analy_from_bottom(
        *arg, funcname, lambda_types_map, return_lambda_types_map);
    }
    // infer expr type for 'a set
    auto iter = the_dtypedef_mapping.find("setCons");

    if (iter == the_dtypedef_mapping.end()) {
      throw string("cannot find the definition of <setCons>\n");
    }
    DatatypeDef& dtypedef = iter->second.get();

    trans.expr_type = dtypedef.decl_type->clone();
    TemplateType& tmpltype = reinterpret_cast<TemplateType&>(*trans.expr_type);
    if (trans.exprs.size() > 0) {
      tmpltype.args[0] = trans.exprs[0]->expr_type->clone();
    }
  } else if (typeid(expr) == typeid(LetinExpr)) {
    LetinExpr& trans = reinterpret_cast<LetinExpr&>(expr);
    analy_from_bottom(*trans.equation.expr,
                      funcname,
                      lambda_types_map,
                      return_lambda_types_map);
    trans.equation.pattern->expr_type = trans.equation.expr->expr_type->clone();
    pattern_infer(trans.equation.pattern, funcname);
    // VarExpr &pattern_trans = reinterpret_cast<VarExpr
    // &>(*trans.equation.pattern); the_arg_type_mapping.emplace(
    //                             pattern_trans.name,
    //                             ref(trans.equation.expr->expr_type)
    //                         );

    analy_from_bottom(
      *trans.expr, funcname, lambda_types_map, return_lambda_types_map);
    trans.expr_type = trans.expr->expr_type->clone();
  } else if (typeid(expr) == typeid(CaseExpr)) {
    CaseExpr& trans = reinterpret_cast<CaseExpr&>(expr);
    analy_from_bottom(
      *trans.expr, funcname, lambda_types_map, return_lambda_types_map);

    for (auto& equation : trans.equations) {
      equation.pattern->expr_type = trans.expr->expr_type->clone();

      pattern_infer(equation.pattern, funcname);
      analy_from_bottom(
        *equation.expr, funcname, lambda_types_map, return_lambda_types_map);
    }
    trans.expr_type = trans.equations[0].expr->expr_type->clone();
  } else if (typeid(expr) == typeid(LambdaExpr)) {
    LambdaExpr& trans = reinterpret_cast<LambdaExpr&>(expr);

    for (auto& elem : lambda_types_map) {
      trans.lambda_types_map.insert(elem);
    }
    for (auto& param : trans.parameters) {
      trans.lambda_storage.push_back(gen_new_lambda_arg());
      trans.lambda_types_map.emplace(param, ref(trans.lambda_storage.back()));
    }

    LambdaMap return_map{};
    analy_from_bottom(
      *trans.expr, funcname, trans.lambda_types_map, return_map);

    LambdaMap temp_map = return_map;
    for (auto& elem : trans.lambda_types_map) {
      auto iter = return_map.find(elem.first);
      if (iter == return_map.end()) {
        temp_map.insert(elem);
      }
    }
    trans.lambda_types_map = temp_map;

    Ptr<FuncType> lambda_type(new FuncType);
    for (size_t i = 0; i < trans.parameters.size(); ++i) {
      auto& type =
        trans.lambda_types_map.find(trans.parameters[i])->second.get();
      lambda_type->types.push_back(type);
    }
    auto& type = trans.expr->expr_type;
    lambda_type->types.push_back(type);

    expr.expr_type = lambda_type;

    // return_lambda_types_map = trans.lambda_types_map;
    for (auto& elem : trans.lambda_types_map) {
      return_lambda_types_map.insert(elem);
    }
  } else {
    cerr << "analy_from_bottom(): bad match.\n";
  }
}

void
TypeInference::analy_lambda_expr(Expr& expr, const string& funcname)
{
  if (typeid(expr) == typeid(IntegralExpr)) {
    return;
  } else if (typeid(expr) == typeid(VarExpr)) {
    return;
  } else if (typeid(expr) == typeid(ConsExpr)) {
    ConsExpr& trans = reinterpret_cast<ConsExpr&>(expr);

    for (size_t i = 0; i < trans.args.size(); ++i) {
      analy_lambda_expr(*trans.args[i], funcname);
    }

  } else if (typeid(expr) == typeid(BinaryOpExpr)) {
    BinaryOpExpr& trans = reinterpret_cast<BinaryOpExpr&>(expr);

    analy_lambda_expr(*trans.lhs, funcname);
    analy_lambda_expr(*trans.rhs, funcname);

  } else if (typeid(expr) == typeid(ListExpr)) {
    ListExpr& trans = reinterpret_cast<ListExpr&>(expr);

    for (auto& arg : trans.exprs) {
      analy_lambda_expr(*arg, funcname);
    }
  } else if (typeid(expr) == typeid(SetExpr)) {
    SetExpr& trans = reinterpret_cast<SetExpr&>(expr);

    for (auto& arg : trans.exprs) {
      analy_lambda_expr(*arg, funcname);
    }
  } else if (typeid(expr) == typeid(LetinExpr)) {
    LetinExpr& trans = reinterpret_cast<LetinExpr&>(expr);
    analy_lambda_expr(*trans.equation.expr, funcname);
    analy_lambda_expr(*trans.expr, funcname);
  } else if (typeid(expr) == typeid(CaseExpr)) {
    CaseExpr& trans = reinterpret_cast<CaseExpr&>(expr);
    analy_lambda_expr(*trans.expr, funcname);
  } else if (typeid(expr) == typeid(LambdaExpr)) {
    LambdaExpr& trans = reinterpret_cast<LambdaExpr&>(expr);

    analy_lambda_expr(*trans.expr, funcname);

    Ptr<FuncType> lambda_type(new FuncType);
    for (size_t i = 0; i < trans.parameters.size(); ++i) {
      auto& type =
        trans.lambda_types_map.find(trans.parameters[i])->second.get();
      lambda_type->types.push_back(type);
    }
    auto& type = trans.expr->expr_type;
    lambda_type->types.push_back(type);

    expr.expr_type = lambda_type;
  } else {
    cerr << "analy_lambda_expr(): bad match.\n";
  }
}

void
TypeInference::analy_from_top(Expr& expr)
{
  if (typeid(expr) == typeid(IntegralExpr)) {
    return;
  } else if (typeid(expr) == typeid(VarExpr)) {
    return;
  } else if (typeid(expr) == typeid(ConsExpr)) {
    ConsExpr& trans = reinterpret_cast<ConsExpr&>(expr);

    if (the_dtypedef_mapping.find(trans.constructor) ==
        the_dtypedef_mapping.end()) {
      // it's a function apply
      Ptr<Type> type;
      if (the_arg_type_mapping.find(trans.constructor) !=
          the_arg_type_mapping.end()) {
        type =
          the_arg_type_mapping.find(trans.constructor)->second.get()->clone();
      } else {
        type = the_func_type_mapping.find(trans.constructor)
                 ->second.get()
                 .type->clone();
        type_mangling(type);
      }

      auto& functype = dynamic_cast<FuncType&>(*type);
      if (typeid(*trans.expr_type) == typeid(FuncType)) {
        auto& trans_functype = dynamic_cast<FuncType&>(*trans.expr_type);

        for (size_t i = 0; i < trans_functype.types.size(); ++i) {
          template_type_map(functype.types[i], trans_functype.types[i]);
          template_type_ins(type);
        }

        for (size_t i = 0; i < trans.args.size(); ++i) {
          if (is_mangled(functype.types[i])) {
            break;
          }
          if (typeid(*trans.args[i]) == typeid(IntegralExpr) ||
              typeid(*trans.args[i]) == typeid(VarExpr) ||
              typeid(*trans.args[i]) == typeid(LambdaExpr)) {
            if (is_mangled(trans.args[i]->expr_type)) {
              trans.args[i]->expr_type = functype.types[i]->clone();
              analy_from_top(*trans.args[i]);
            }
          } else {
            trans.args[i]->expr_type = functype.types[i]->clone();
            analy_from_top(*trans.args[i]);
          }
        }
      } else {
        template_type_map(functype.types[functype.types.size() - 1],
                          trans.expr_type);
        template_type_ins(type);

        for (size_t i = 0; i < trans.args.size(); ++i) {
          if (is_mangled(functype.types[i])) {
            break;
          }
          if (typeid(*trans.args[i]) == typeid(IntegralExpr) ||
              typeid(*trans.args[i]) == typeid(VarExpr) ||
              typeid(*trans.args[i]) == typeid(LambdaExpr)) {
            if (is_mangled(trans.args[i]->expr_type)) {
              trans.args[i]->expr_type = functype.types[i]->clone();
              analy_from_top(*trans.args[i]);
            }
          } else {
            trans.args[i]->expr_type = functype.types[i]->clone();
            analy_from_top(*trans.args[i]);
          }
        }
      }
    } else {
      // it's a datatype's constructor
      auto type =
        the_cons_type_mapping.find(trans.constructor)->second->clone();
      type_mangling(type);
      auto& dtype_funcform = dynamic_cast<FuncType&>(*type);

      if (typeid(*dtype_funcform.types[dtype_funcform.types.size() - 1]) ==
          typeid(NormalType)) {
        return;
      } else {
        template_type_map(dtype_funcform.types[dtype_funcform.types.size() - 1],
                          trans.expr_type);
        template_type_ins(type);

        for (size_t i = 0; i < trans.args.size(); ++i) {
          if (is_mangled(trans.args[i]->expr_type)) {
            trans.args[i]->expr_type = dtype_funcform.types[i]->clone();
            analy_from_top(*trans.args[i]);
          }
        }
      }
    }
  } else if (typeid(expr) == typeid(BinaryOpExpr)) {
    BinaryOpExpr& trans = reinterpret_cast<BinaryOpExpr&>(expr);

    if (trans.op.value == "=") {
      return;
    }

    if (is_mangled(trans.lhs->expr_type)) {
      trans.lhs->expr_type = trans.expr_type;
      analy_from_top(*trans.lhs);
    }
    if (is_mangled(trans.rhs->expr_type)) {
      trans.rhs->expr_type = trans.expr_type;
      analy_from_top(*trans.lhs);
    }
  } else if (typeid(expr) == typeid(ListExpr)) {
    ListExpr& trans = reinterpret_cast<ListExpr&>(expr);
    TemplateType& tmpltype = reinterpret_cast<TemplateType&>(*trans.expr_type);

    // mark type
    for (auto& arg : trans.exprs) {
      if (is_mangled(arg->expr_type)) {
        arg->expr_type = tmpltype.args[0];
      }
      analy_from_top(*arg);
    }
  } else if (typeid(expr) == typeid(SetExpr)) {
    SetExpr& trans = reinterpret_cast<SetExpr&>(expr);
    TemplateType& tmpltype = reinterpret_cast<TemplateType&>(*trans.expr_type);

    // mark type
    for (auto& arg : trans.exprs) {
      if (is_mangled(arg->expr_type)) {
        arg->expr_type = tmpltype.args[0];
      }
      analy_from_top(*arg);
    }
  } else if (typeid(expr) == typeid(LetinExpr)) {
    LetinExpr& trans = reinterpret_cast<LetinExpr&>(expr);

    trans.expr->expr_type = trans.expr_type;
    analy_from_top(*trans.expr);
  } else if (typeid(expr) == typeid(CaseExpr)) {
    CaseExpr& trans = reinterpret_cast<CaseExpr&>(expr);
    for (auto& equation : trans.equations) {
      equation.expr->expr_type = trans.expr_type;
      analy_from_top(*equation.expr);
    }
  } else if (typeid(expr) == typeid(LambdaExpr)) {
    return;
  } else {
    cerr << "analy_from_top(): bad match.\n";
  }
}
} // namespace hol2cpp
