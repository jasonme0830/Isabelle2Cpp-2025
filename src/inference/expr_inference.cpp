#include "inference.hpp"

using namespace std;

namespace hol2cpp {
void TypeInference::analy_from_bottom(Expr &expr, const string &funcname) {
    if (typeid(expr) == typeid(IntegralExpr)) {
        if(expr.expr_type != nullptr) {
            return;
        }

        IntegralExpr &trans = reinterpret_cast<IntegralExpr &>(expr);
        trans.expr_type = normal_nat_type;
    }
    else if (typeid(expr) == typeid(VarExpr)) {
        if (expr.expr_type != nullptr) {
            return;
        }

        // variables
        VarExpr &trans = reinterpret_cast<VarExpr &>(expr);
        auto var_elem = find_if(the_arg_type_mapping.begin(), the_arg_type_mapping.end(),
                        [name = trans.name]
                        (auto &iter) {
                            return name == iter.first;
                        });

        if(var_elem != the_arg_type_mapping.end()) {
            expr.expr_type = var_elem->second.get()->clone();
            return;
        }

        // lambda param
        auto lambda_elem = find_if(the_lambda_ins_mapping.begin(), the_lambda_ins_mapping.end(),
                            [name = trans.name]
                            (auto &iter) {
                                return name == iter.first;
                            });

        if (lambda_elem != the_lambda_ins_mapping.end()) {
            auto lambda_index = lambda_elem->second;
            expr.expr_type = the_lambda_storge[lambda_index];  // shallow copy
            temp_vec.push_back(make_pair(
                                            ref(expr.expr_type),
                                            ref(the_lambda_storge[lambda_index])
                                        ));
            return;
        }

        //constructor like Nil that only appears in expr
        auto cons_elem = find_if(the_cons_type_mapping.begin(), the_cons_type_mapping.end(),
                            [name = trans.name]
                            (auto& iter) {
                                return name == iter.first;
                            });

        if (cons_elem != the_cons_type_mapping.end()) {
            expr.expr_type = cons_elem->second.get().decl_type->clone();
            return;
        }
        else {
            expr.expr_type = normal_unknown_type;
        }

    }
    else if (typeid(expr) == typeid(ConsExpr)) {
        ConsExpr &trans = reinterpret_cast<ConsExpr &>(expr);

        // mark the type of sub-exprs
        for (size_t i = 0; i < trans.args.size(); ++i) {
            analy_from_bottom(*trans.args[i], funcname);
        }

        // infer expr
        if (the_cons_type_mapping.find(trans.constructor) == the_cons_type_mapping.end()) {
            // it's a function apply
            auto funcdef_iter = the_func_type_mapping.find(trans.constructor); 

            if (funcdef_iter != the_func_type_mapping.end()) {
                // call functions which are from the FunctionDef
                FunctionDef &funcdef = funcdef_iter->second.get();
                trans.expr_type = funcdef.type->clone();

                function_apply(trans.expr_type, trans.args);
            }
            else {
                // call functions which are from the pattern's args
                auto elem = find_if(the_arg_type_mapping.begin(), the_arg_type_mapping.end(),
                            [name = trans.constructor]
                            (auto &iter) {
                                return name == iter.first;
                            }); // e.g. trans.constructor := f

                if (elem == the_arg_type_mapping.end()) {
                    string info = funcname + ": cannot find the definition of <" +  trans.constructor + ">\n";
                    throw info;
                }

                auto &functype = elem->second.get();
                trans.expr_type = functype->clone();

                function_apply(trans.expr_type, trans.args);
            }
        }
        else {
            // it's a datatype's constructor
            DatatypeDef &dtypedef = the_cons_type_mapping.find(trans.constructor)->second.get();
            auto component = find_if(dtypedef.components.begin(), dtypedef.components.end(),
                                    [cons = trans.constructor]
                                    (auto &com) {
                                        return com.constructor == cons;
                                    });

            if(typeid(*dtypedef.decl_type) == typeid(NormalType)) {
                trans.expr_type = dtypedef.decl_type->clone();
            }
            else {
                //datatype's decl_type is TemplateType
                if (ins_map_clear_flag) {
                    the_argument_type_ins_mapping.clear();
                }

                if (component->arguments.size() != trans.args.size()) {
                    string info = trans.constructor + ": need curring of datatype.\n";
                    throw info;
                }
                
                // map all type variables
                for (size_t i = 0; i < component->arguments.size(); ++i) {
                    // get current VarExpr (lambda args)
                    if (typeid(*trans.args[i]) == typeid(VarExpr)) {
                        VarExpr& arg = reinterpret_cast<VarExpr&>(*trans.args[i]);
                        current_lambda_argname = arg.name;
                    }

                    template_type_map(component->arguments[i], trans.args[i]->expr_type);
                }
                ins_map_clear_flag = false;

                trans.expr_type = dtypedef.decl_type->clone();
                TemplateType &trans_decl_type = reinterpret_cast<TemplateType &>(*trans.expr_type);

                for (size_t i = 0; i < trans_decl_type.args.size(); ++i) {
                    template_type_ins(trans_decl_type.args[i]);
                }

                // ins the lambda arg in datatype (y :: lambda_a # ys :: nat)
                for (size_t i = 0; i < trans.args.size(); ++i) {
                    if (typeid(*trans.args[i]) == typeid(VarExpr)) {
                        VarExpr &lambda_trans = reinterpret_cast<VarExpr &>(*trans.args[i]);
                        if (is_lambda_arg(lambda_trans.name)) {
                            template_type_ins(trans.args[i]->expr_type);
                        }
                    }
                }
                ins_map_clear_flag = true;
            }
        }
    }
    else if (typeid(expr) == typeid(BinaryOpExpr)) {
        BinaryOpExpr &trans = reinterpret_cast<BinaryOpExpr &>(expr);
        // mark type
        analy_from_bottom(*trans.lhs, funcname);
        analy_from_bottom(*trans.rhs, funcname);
        // infer expr
        if (trans.op.value == "=") {
            trans.expr_type = normal_bool_type;
        }
        else {
            trans.expr_type = trans.lhs->expr_type->clone();
        }
    }
    else if (typeid(expr) == typeid(ListExpr)) {
        ListExpr &trans = reinterpret_cast<ListExpr &>(expr);
        // mark type
        for (auto &arg: trans.exprs) {
            analy_from_bottom(*arg, funcname);
        }
        //infer expr type for 'a list
        auto iter = the_cons_type_mapping.find("Cons");

        if (iter == the_cons_type_mapping.end()) {
            throw string("cannot find the definition of <Cons>\n");
        }
        DatatypeDef &dtypedef = iter->second.get();

        trans.expr_type = dtypedef.decl_type->clone();
        TemplateType &tmpltype = reinterpret_cast<TemplateType &>(*trans.expr_type);
        if (trans.exprs.size() > 0) {
            tmpltype.args[0] = trans.exprs[0]->expr_type->clone();
        }
    }
    else if (typeid(expr) == typeid(SetExpr)) {
        SetExpr &trans = reinterpret_cast<SetExpr &>(expr);
        //mark type
        for (auto &arg: trans.exprs) {
            analy_from_bottom(*arg, funcname);
        }
        //infer expr type for 'a set
        auto iter = the_cons_type_mapping.find("setCons");

        if (iter == the_cons_type_mapping.end()) {
            throw string("cannot find the definition of <setCons>\n");
        }
        DatatypeDef &dtypedef = iter->second.get();

        trans.expr_type = dtypedef.decl_type->clone();
        TemplateType &tmpltype = reinterpret_cast<TemplateType &>(*trans.expr_type);
        if (trans.exprs.size() > 0) {
            tmpltype.args[0] = trans.exprs[0]->expr_type->clone();
        }
    }
    else if (typeid(expr) == typeid(LetinExpr)) {
        LetinExpr &trans = reinterpret_cast<LetinExpr &>(expr);
        analy_from_bottom(*trans.equation.expr, funcname);
        trans.equation.pattern->expr_type = trans.equation.expr->expr_type->clone();
        pattern_infer(trans.equation.pattern, funcname); 
        // VarExpr &pattern_trans = reinterpret_cast<VarExpr &>(*trans.equation.pattern);
        // the_arg_type_mapping.emplace(
        //                             pattern_trans.name,
        //                             ref(trans.equation.expr->expr_type)
        //                         );

        analy_from_bottom(*trans.expr, funcname);
        trans.expr_type = trans.expr->expr_type->clone();
    }
    else if (typeid(expr) == typeid(CaseExpr)) {
        CaseExpr &trans = reinterpret_cast<CaseExpr &>(expr);
        analy_from_bottom(*trans.expr, funcname);

        for (auto &equation: trans.equations) {
            equation.pattern->expr_type = trans.expr->expr_type->clone();

            pattern_infer(equation.pattern, funcname);
            analy_from_bottom(*equation.expr, funcname);
        }
        trans.expr_type = trans.equations[0].expr->expr_type->clone();
    }
    else if (typeid(expr) == typeid(LambdaExpr)) {
        LambdaExpr &trans = reinterpret_cast<LambdaExpr &>(expr);

        ++lambda_depth;
        for (auto &param: trans.parameters) {
            gen_new_lambda_arg();
            the_lambda_ins_mapping.emplace(
                                            param,
                                            lambda_counter - 1
                                        );

            trans.param_types.emplace(param, lambda_counter - 1);
        }

        analy_from_bottom(*trans.expr, funcname);

        Ptr<FuncType> lambda_type(new FuncType);
        for (size_t i = 0; i < trans.parameters.size(); ++i) {
            auto lambda_index = the_lambda_ins_mapping.find(trans.parameters[i])->second;
            auto &type = the_lambda_storge[lambda_index];
            lambda_type->types.push_back(type);
        }
        auto &type = trans.expr->expr_type;
        lambda_type->types.push_back(type);

        expr.expr_type = lambda_type;

        --lambda_depth;
        if(lambda_depth == 0) {
            the_lambda_ins_mapping.clear();
        }
    }
    else {
        cerr << "analy_from_bottom(): bad match.\n";
    }
}

void TypeInference::analy_lambda_expr(Expr &expr, const string &funcname) {
    if (typeid(expr) == typeid(IntegralExpr)) {
        return;
    }
    else if (typeid(expr) == typeid(VarExpr)) {
        return;
    }
    else if (typeid(expr) == typeid(ConsExpr)) {
        ConsExpr &trans = reinterpret_cast<ConsExpr &>(expr);

        for (size_t i = 0; i < trans.args.size(); ++i) {
            analy_lambda_expr(*trans.args[i], funcname);
        }

    }
    else if (typeid(expr) == typeid(BinaryOpExpr)) {
        BinaryOpExpr &trans = reinterpret_cast<BinaryOpExpr &>(expr);

        analy_lambda_expr(*trans.lhs, funcname);
        analy_lambda_expr(*trans.rhs, funcname);

    }
    else if (typeid(expr) == typeid(ListExpr)) {
        ListExpr &trans = reinterpret_cast<ListExpr &>(expr);

        for (auto &arg: trans.exprs) {
            analy_lambda_expr(*arg, funcname);
        }
    }
    else if (typeid(expr) == typeid(SetExpr)) {
        SetExpr &trans = reinterpret_cast<SetExpr &>(expr);

        for (auto &arg: trans.exprs) {
            analy_lambda_expr(*arg, funcname);
        }
    }
    else if (typeid(expr) == typeid(LetinExpr)) {
        LetinExpr &trans = reinterpret_cast<LetinExpr &>(expr);
        analy_lambda_expr(*trans.equation.expr, funcname);
        analy_lambda_expr(*trans.expr, funcname);
    }
    else if (typeid(expr) == typeid(CaseExpr)) {
        CaseExpr &trans = reinterpret_cast<CaseExpr &>(expr);
        analy_lambda_expr(*trans.expr, funcname);
    }
    else if (typeid(expr) == typeid(LambdaExpr)) {
        LambdaExpr &trans = reinterpret_cast<LambdaExpr &>(expr);

        ++lambda_depth;

        analy_lambda_expr(*trans.expr, funcname);

        Ptr<FuncType> lambda_type(new FuncType);
        for (size_t i = 0; i < trans.parameters.size(); ++i) {
            auto lambda_index = trans.param_types.find(trans.parameters[i])->second;
            auto &type = the_lambda_storge[lambda_index];
            lambda_type->types.push_back(type);
        }
        auto &type = trans.expr->expr_type;
        lambda_type->types.push_back(type);

        expr.expr_type = lambda_type;

        --lambda_depth;
    }
    else {
        cerr << "analy_lambda_expr(): bad match.\n";
    }
}
} // namespace hol2cpp
