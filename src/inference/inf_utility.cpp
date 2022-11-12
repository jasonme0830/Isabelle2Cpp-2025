#include "inference.hpp"

using namespace std;

namespace hol2cpp {
void TypeInference::template_type_ins(Ptr<Type> &type) {
    if (typeid(*type) == typeid(ArgumentType)) {
        ArgumentType &argmttype = reinterpret_cast<ArgumentType &>(*type);

        auto ins_type_iter = the_argument_type_ins_mapping.find(argmttype.name);
        
        if (ins_type_iter != the_argument_type_ins_mapping.end()) {
            auto ins_type = ins_type_iter->second.get()->clone();
            type = ins_type;
        }
    }
    else if (typeid(*type) == typeid(TemplateType)) {
        TemplateType &tmpltype = reinterpret_cast<TemplateType &>(*type);

        for(size_t i = 0; i < tmpltype.args.size(); ++i) {
            template_type_ins(tmpltype.args[i]);
        }
    }
    else if (typeid(*type) == typeid(FuncType)) {
        FuncType &functype = reinterpret_cast<FuncType &>(*type);

        for (size_t i = 0; i < functype.types.size(); ++i) {
            template_type_ins(functype.types[i]);
        }
    }
    else return;
}

void TypeInference::template_type_ins_lambda(Ptr<Expr> &lambda_expr) {
    LambdaExpr &lambda_trans = reinterpret_cast<LambdaExpr &>(*lambda_expr);

    for (auto &elem : lambda_trans.lambda_types_map) {
        template_type_ins(elem.second.get());
    }

    template_type_ins(lambda_trans.expr->expr_type);
}

void TypeInference::template_type_map(Ptr<Type> &com_type, Ptr<Type> &ins_type) {
    if (typeid(*com_type) == typeid(ArgumentType)) {
        ArgumentType &com_type_trans = reinterpret_cast<ArgumentType &>(*com_type);
        the_argument_type_ins_mapping.emplace(
                                                com_type_trans.name, 
                                                ins_type->clone()
                                            );
    }
    else if (typeid(*com_type) == typeid(TemplateType)) {
        TemplateType &com_type_trans = reinterpret_cast<TemplateType &>(*com_type);
        TemplateType &ins_type_trans = reinterpret_cast<TemplateType &>(*ins_type);

        for (size_t i = 0; i < com_type_trans.args.size(); ++i) {
            template_type_map(com_type_trans.args[i], ins_type_trans.args[i]);
        }
    }
    else if (typeid(*com_type) == typeid(FuncType)) {
        FuncType &com_type_trans = reinterpret_cast<FuncType &>(*com_type);
        FuncType &ins_type_trans = reinterpret_cast<FuncType &>(*ins_type);

        for (size_t i = 0; i < com_type_trans.types.size(); ++i) {
            template_type_map(com_type_trans.types[i], ins_type_trans.types[i]);
        }
    }
    else return;
}

void TypeInference::template_type_map_lambda(Ptr<Type> &com_type, Ptr<Type> &ins_type) {
    ins_type = com_type->clone();
}

Ptr<Type> TypeInference::function_apply(Ptr<Type> &functype, std::vector<Ptr<Expr>> &params) {  
    Ptr<Type> return_type = functype->clone();
    auto return_type_trans = reinterpret_cast<FuncType *>(return_type.get());

    for (size_t i = 0; i < params.size(); ++i) {
        if ((!is_mangled(return_type_trans->types[0]) && is_mangled(params[i]->expr_type)) || 
            (is_lambda(params[i]->expr_type))) { function_apply_aux(params[i]->expr_type, return_type_trans->types[0]); }
        else                                   { function_apply_aux(return_type_trans->types[0], params[i]->expr_type); }

        function_update(return_type_trans->types);

        for (size_t j = 0; j < params.size(); ++j) { 
            if (typeid(*params[j]) == typeid(LambdaExpr)) { template_type_ins_lambda(params[j]); }
            else                                          { template_type_ins(params[j]->expr_type); }
        }

        ins_map_clear_flag = true;
    }
    
    if (return_type_trans->types.size() == 1) { 
        return return_type_trans->types[0];
    }
    else {
        return return_type;
    }
}

void TypeInference::function_apply_aux(Ptr<Type> &func_param_type, Ptr<Type> &ins_param_type) {
    if (ins_map_clear_flag) {
        the_argument_type_ins_mapping.clear();
    }

    if (typeid(*func_param_type) == typeid(NormalType)) {
        return;
    }
    else if (typeid(*func_param_type) == typeid(ArgumentType)) {
        ArgumentType &func_param_type_trans = reinterpret_cast<ArgumentType &>(*func_param_type);
        the_argument_type_ins_mapping.emplace(
                                                func_param_type_trans.name, 
                                                ins_param_type->clone()
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
        //FunctionType, ProductType is TemplateType of ('a, 'b)Pair now
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

    vector<Ptr<Type>> temp;

    for (size_t i = 1; i < args.size(); ++i) { temp.push_back(args[i]); }
    for (size_t i = 0; i < temp.size(); ++i) { template_type_ins(temp[i]); }

    args = temp; 
}

Ptr<Type> TypeInference::gen_new_lambda_arg() {
    string lambda("lambda@");
    lambda += (static_cast<char>('a' + lambda_counter));
    ++lambda_counter;

    Ptr<Type> ptr = make_unique<ArgumentType>(lambda);
    return ptr;
}

void TypeInference::type_mangling(Ptr<Type> &type) {
    type_mangling_aux(type);

    ++argument_counter;
}

void TypeInference::type_mangling_aux(Ptr<Type> &type) {
    if (typeid(*type) == typeid(NormalType)) {
        return;
    }
    else if (typeid(*type) == typeid(ArgumentType)) {
        ArgumentType &trans = dynamic_cast<ArgumentType &>(*type);

        string suffix = string("#") + to_string(argument_counter);
        trans.name += suffix;
    }
    else if (typeid(*type) == typeid(TemplateType)) {
        TemplateType &trans = dynamic_cast<TemplateType &>(*type);
        for (auto &elem: trans.args) {
            type_mangling_aux(elem);
        }
    }
    else {
        // FuncType
        FuncType &trans = dynamic_cast<FuncType &>(*type);
        for (auto &elem: trans.types) {
            type_mangling_aux(elem);
        }
    }
}

void TypeInference::dtypedef_mangling(DatatypeDef &dtypedef) {
    type_mangling_aux(dtypedef.decl_type);

    for (auto &component: dtypedef.components) {
        for (auto &argument: component.arguments) {
            type_mangling_aux(argument);
        }
    }
    
    ++argument_counter;
}

bool TypeInference::is_mangled(Ptr<Type> &type) {
    if (typeid(*type) == typeid(NormalType)) {
        return false;
    }
    else if (typeid(*type) == typeid(ArgumentType)) {
        ArgumentType &trans = dynamic_cast<ArgumentType &>(*type);

        return (trans.name.find('#') != string::npos) ? true : false;
    }
    else if (typeid(*type) == typeid(TemplateType)) {
        TemplateType &trans = dynamic_cast<TemplateType &>(*type);
        for (auto &elem: trans.args) {
            if (is_mangled(elem)) 
                return true;
        }
        return false;
    }
    else {
        // FuncType
        FuncType &trans = dynamic_cast<FuncType &>(*type);
        for (auto &elem: trans.types) {
            if (is_mangled(elem)) 
                return true;
        }
        return false;
    }
}

bool TypeInference::is_lambda(Ptr<Type> &type) {
    if (typeid(*type) == typeid(NormalType)) {
        return false;
    }
    else if (typeid(*type) == typeid(ArgumentType)) {
        ArgumentType &trans = dynamic_cast<ArgumentType &>(*type);

        return (trans.name.find('@') != string::npos) ? true : false;
    }
    else if (typeid(*type) == typeid(TemplateType)) {
        TemplateType &trans = dynamic_cast<TemplateType &>(*type);
        for (auto &elem: trans.args) {
            if (is_lambda(elem)) 
                return true;
        }
        return false;
    }
    else {
        // FuncType
        FuncType &trans = dynamic_cast<FuncType &>(*type);
        for (auto &elem: trans.types) {
            if (is_lambda(elem)) 
                return true;
        }
        return false;
    }
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