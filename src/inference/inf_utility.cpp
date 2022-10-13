#include "inference.hpp"

using namespace std;

namespace hol2cpp {
void TypeInference::template_type_ins(Ptr<Type> &type) {
    if (typeid(*type) == typeid(TemplateType)) {
        TemplateType &tmpltype = reinterpret_cast<TemplateType &>(*type);

        for(size_t i = 0; i < tmpltype.args.size(); ++i) {
            template_type_ins(tmpltype.args[i]);
        }
    }
    else if (typeid(*type) == typeid(ArgumentType)) {
        ArgumentType &argmttype = reinterpret_cast<ArgumentType &>(*type);

        auto ins_type_iter = theArgumentTypeInsMapping.find(argmttype.name);
        
        if (ins_type_iter != theArgumentTypeInsMapping.end()) {
            auto ins_type = ins_type_iter->second.get();
            type = ins_type;
        }
    }
    else return;
}

void TypeInference::template_type_map(Ptr<Type> &com_type, Ptr<Type> &ins_type) {
    if (is_lambda_arg(current_lambda_argname)) {
        ins_type = com_type->clone();
        auto iter = theLambdaInsMapping.find(current_lambda_argname);
        theLambdaStorge[iter->second] = ins_type;
        return;
    }
    
    if (typeid(*com_type) == typeid(ArgumentType)) {
        ArgumentType &com_type_trans = reinterpret_cast<ArgumentType &>(*com_type);
        theArgumentTypeInsMapping.emplace(com_type_trans.name, ref(ins_type));
    }
    else if (typeid(*com_type) == typeid(TemplateType)) {
        TemplateType &com_type_trans = reinterpret_cast<TemplateType &>(*com_type);
        TemplateType &ins_type_trans = reinterpret_cast<TemplateType &>(*ins_type);

        for (size_t i = 0; i < com_type_trans.args.size(); ++i) {
            template_type_map(com_type_trans.args[i], ins_type_trans.args[i]);
        }
    }
    else return;
}

void TypeInference::function_apply(Ptr<Type> &functype, std::vector<Ptr<Expr>> &params) {  
    FuncType &functype_trans = reinterpret_cast<FuncType &>(*functype);
    auto argnum = functype_trans.types.size();

    

    for (size_t i = 0; i < argnum - 1; ++i) {
        function_apply_aux(functype_trans.types[0], params[i]->expr_type);
        function_update(functype_trans.types);

        for (auto &elem : theLambdaInsMapping) {
            template_type_ins(theLambdaStorge[elem.second]);
        }

        ins_map_clear_flag = true;
    }

    for (auto& elem : temp_vec) {
        elem.first.get() = elem.second.get();
    }
    
    functype = functype_trans.types[0];
}

void TypeInference::function_apply_aux(Ptr<Type> &func_param_type, Ptr<Type> &ins_param_type) {
    if (ins_map_clear_flag) {
        theArgumentTypeInsMapping.clear();
    }

    if (typeid(*func_param_type) == typeid(NormalType)) {
        return;
    }
    else if (typeid(*func_param_type) == typeid(ArgumentType)) {
        ArgumentType &func_param_type_trans = reinterpret_cast<ArgumentType &>(*func_param_type);
        theArgumentTypeInsMapping.emplace(
                                            func_param_type_trans.name, 
                                            ref(ins_param_type)
                                        );
    }
    else if (typeid(*func_param_type) == typeid(TemplateType)) {
        TemplateType &func_param_type_trans = reinterpret_cast<TemplateType &>(*func_param_type);
        TemplateType &ins_param_type_trans = reinterpret_cast<TemplateType &>(*ins_param_type);
        for (size_t i = 0; i < func_param_type_trans.args.size(); ++i) {
            function_apply_aux(func_param_type_trans.args[i], ins_param_type_trans.args[i]);
        }
    }
    else { 
        //FunctionType(ProductType?)
        FuncType &func_param_type_trans = reinterpret_cast<FuncType &>(*func_param_type);
        FuncType &ins_param_type_trans = reinterpret_cast<FuncType &>(*ins_param_type);
        for (size_t i = 0; i < func_param_type_trans.types.size(); ++i) {
            function_apply_aux(func_param_type_trans.types[i], ins_param_type_trans.types[i]);
        }
    }            
    ins_map_clear_flag = false;
}

void TypeInference::function_update(vector<Ptr<Type>> &args) {
    if (args.size() == 1) return;

    vector<reference_wrapper<Ptr<Type>>> tem_vec;
    for (size_t i = 1; i < args.size(); ++i) {
        tem_vec.push_back(ref(args[i]));
    }

    for (size_t i = 0; i < tem_vec.size(); ++i) {
        template_type_ins(tem_vec[i]);
    }

    for (size_t i = 0; i < tem_vec.size(); ++i) {
        args[i] = tem_vec[i].get();
    }
}

void TypeInference::gen_new_lambda_arg() {
    string lambda("lambda_");
    lambda += (static_cast<char>('a' + lambda_counter));
    ++lambda_counter;

    Ptr<Type> ptr = make_unique<ArgumentType>(lambda);
    theLambdaStorge.push_back(move(ptr));
}

bool TypeInference::is_lambda_arg(const std::string &argname) {
    return theLambdaInsMapping.find(argname) == theLambdaInsMapping.end() ? false : true;
}

Ptr<Type> TypeInference::analy_if_expr(Expr &ifexpr, const string &funcname) {
    ConsExpr &ifexpr_trans = reinterpret_cast<ConsExpr &>(ifexpr);
    for (size_t i = 1; i < ifexpr_trans.args.size(); ++i) {
        if (typeid(*ifexpr_trans.args[i]) != typeid(ConsExpr)) {
            return ifexpr_trans.args[i]->expr_type;
        }
        else {
            ConsExpr &arg_trans = reinterpret_cast<ConsExpr &>(*ifexpr_trans.args[i]);
            if (arg_trans.constructor == "If") {
                for (size_t j = 1; j < arg_trans.args.size(); ++j) {
                    analy_if_expr(*arg_trans.args[j], funcname);
                }
            }
            else {
                return arg_trans.expr_type;
            }
        }
    }
    return nullptr;
}

void TypeInference::print_theory() {
    for (auto &def: thy.definitions) {
        if (typeid(*def) == typeid(FunctionDef)) {
            auto &def_trans = reinterpret_cast<FunctionDef &>(*def);
            cout << def_trans.name << endl;
            for (auto &equation: def_trans.equations) {
                auto &expr = equation.expr;
                expr->print_expr();
                cout << endl;
            }
        }
    }
}
} // namespace hol2cpp