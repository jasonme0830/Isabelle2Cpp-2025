#include "inference.hpp"

using namespace std;

namespace hol2cpp {
void TypeInference::pattern_infer(Ptr<Expr> &expr, const string &funcname) {
    if (typeid(*expr) == typeid(VarExpr)) {
        VarExpr &arg_trans = reinterpret_cast<VarExpr &>(*expr);
        theArgTypeMapping.emplace(
                                    arg_trans.name, 
                                    ref(arg_trans.expr_type)
                                );
    }
    else if (typeid(*expr) == typeid(ConsExpr)) {
        ConsExpr &expr_trans = reinterpret_cast<ConsExpr &>(*expr);
        pattern_consexpr_infer(expr_trans, funcname);
        ins_map_clear_flag = true;
    }
    else if (typeid(*expr) == typeid(ListExpr)) {
        pattern_list_or_set_infer(*expr, funcname);
    }
    else if (typeid(*expr) == typeid(SetExpr)) {
        pattern_list_or_set_infer(*expr, funcname);
    }
    else {
        cerr << "pattern_infer(): error map_funcdef.\n";
    }
    // Let-in-expr and case-expr won't appear at function's pattern(s)
    // IntegeralExpr has been given NormalType by the function arglist (nat, int, <N word>?)
}

void TypeInference::pattern_consexpr_infer(ConsExpr &consexpr, const string &funcname) {
    // all ConsExpr in pattern is datatype's constructor (Cons x xs) :: nat list
    DatatypeDef &dtypedef = (theConsTypeMapping.find(consexpr.constructor))->second.get();
    auto component = find_if(dtypedef.components.begin(), dtypedef.components.end(), 
                            [cons = consexpr.constructor]
                            (auto &com) {
                                return com.constructor == cons;
                            });

    /** 
     * judge the decl_type of dtypedef : NormalType or TemplateType
     * NormalType such like : datatype normallist :: nNil | nCons nat normallist
    */
    if(typeid(*dtypedef.decl_type) == typeid(NormalType)) {
        for (size_t i = 0; i < consexpr.args.size(); ++i) {
            consexpr.args[i]->expr_type = component->arguments[i]->clone();

            VarExpr& consexpr_arg_trans = reinterpret_cast<VarExpr&>(*consexpr.args[i]);
            theArgTypeMapping.emplace(
                                    consexpr_arg_trans.name,
                                    ref(consexpr_arg_trans.expr_type)
                                );  
        }
    } 
    else {
        //TemplateType
        
        // trans_expr_type includes the institution type(s)
        TemplateType &trans_expr_type = reinterpret_cast<TemplateType &>(*consexpr.expr_type); 
        // trans_decl_type includes all argumentType args
        TemplateType &trans_decl_type = reinterpret_cast<TemplateType &>(*dtypedef.decl_type); 

        if (ins_map_clear_flag) {
            theArgumentTypeInsMapping.clear();
        }

        for (size_t i = 0; i < trans_decl_type.args.size(); ++i) {
            try {
                ArgumentType &argument_type_trans = dynamic_cast<ArgumentType &>(*trans_decl_type.args[i]);
                theArgumentTypeInsMapping.emplace(
                                                    argument_type_trans.name, 
                                                    ref(trans_expr_type.args[i])
                                                );
            }
            catch (bad_cast &) {
                cerr << "pattern_consexpr_infer(): bad cast ArgumentType.\n";
            }
        }
        ins_map_clear_flag = false;
        
        for (size_t i = 0; i < component->arguments.size(); ++i) {
            if (typeid(*consexpr.args[i]) == typeid(VarExpr)) {  
                if (typeid(*component->arguments[i]) == typeid(ArgumentType)) {  
                    //institution and map theArgTypeMapping
                    ArgumentType &com_arg_trans = reinterpret_cast<ArgumentType &>(*component->arguments[i]);
                    auto &ins_type = theArgumentTypeInsMapping.find(com_arg_trans.name)->second.get();
                    consexpr.args[i]->expr_type = ins_type->clone();

                    try {
                        VarExpr &consexpr_arg_trans = dynamic_cast<VarExpr &>(*consexpr.args[i]);
                        theArgTypeMapping.emplace(
                                                    consexpr_arg_trans.name, 
                                                    ref(consexpr_arg_trans.expr_type)
                                                );
                    }
                    catch (bad_cast &) {
                        cerr << "pattern_consexpr_infer(): bad cast ArgumentType.\n";
                    }
                }
                else if (typeid(*component->arguments[i]) == typeid(TemplateType)) {
                    consexpr.args[i]->expr_type = component->arguments[i]->clone();
                    template_type_ins(consexpr.args[i]->expr_type);

                    try {
                        VarExpr &consexpr_arg_trans = dynamic_cast<VarExpr &>(*consexpr.args[i]);
                        theArgTypeMapping.emplace(
                                                    consexpr_arg_trans.name, 
                                                    ref(consexpr_arg_trans.expr_type)
                                                );
                    }
                    catch (bad_cast &) {
                        cerr << "pattern_consexpr_infer(): bad cast VarExpr.\n";
                    }
                }
                else if (typeid(*component->arguments[i]) == typeid(NormalType)) {
                    consexpr.args[i]->expr_type = component->arguments[i]->clone();
                    VarExpr& consexpr_arg_trans = reinterpret_cast<VarExpr&>(*consexpr.args[i]);
                    theArgTypeMapping.emplace(
                                                consexpr_arg_trans.name,
                                                ref(consexpr_arg_trans.expr_type)
                                            );
                }
                else {
                    //FuncType
                    cerr << "bad match FuncType in pattern_consexpr_infer()\n";
                }
            }
            else if (typeid(*consexpr.args[i]) == typeid(ConsExpr)) {
                if (typeid(*component->arguments[i]) == typeid(ArgumentType)) {
                    //institution and map theArgTypeMapping
                    ArgumentType &com_arg_trans = reinterpret_cast<ArgumentType &>(*component->arguments[i]);
                    auto &ins_type = theArgumentTypeInsMapping.find(com_arg_trans.name)->second.get();
                    consexpr.args[i]->expr_type = ins_type->clone();
                }
                else if (typeid(*component->arguments[i]) == typeid(TemplateType)) {
                    consexpr.args[i]->expr_type = component->arguments[i]->clone();
                    template_type_ins(consexpr.args[i]->expr_type);
                }
                else if (typeid(*component->arguments[i]) == typeid(NormalType)) {
                    consexpr.args[i]->expr_type = component->arguments[i]->clone();
                }
                else {
                    // FuncType
                    cerr << "bad match FuncType in pattern_consexpr_infer()\n";
                }
                ConsExpr &consexpr_trans = reinterpret_cast<ConsExpr &>(*consexpr.args[i]);
                pattern_consexpr_infer(consexpr_trans, funcname);
            }
            else {
                cerr << "pattern_consexpr_infer(): error map.\n";
            }
            //Integeral type has been given NormalType
        }
    }
}

void TypeInference::pattern_list_or_set_infer(Expr &expr, const std::string &funcname) {
    if (typeid(expr) == typeid(ListExpr)) {
        ListExpr &expr_trans = reinterpret_cast<ListExpr &>(expr);
        
        // trans_expr_type includes the institution type(s)
        TemplateType &trans_expr_type = reinterpret_cast<TemplateType &>(*expr_trans.expr_type); 

        // ins_type is the expr_type of the first element
        auto &ins_type = trans_expr_type.args[0];

        for (size_t i = 0; i < expr_trans.exprs.size(); ++i) {
            expr_trans.exprs[i]->expr_type = ins_type->clone();
            if (typeid(*expr_trans.exprs[i]) == typeid(VarExpr)) {
                VarExpr &list_subexpr_trans = reinterpret_cast<VarExpr &>(*expr_trans.exprs[i]);
                theArgTypeMapping.emplace(
                                            list_subexpr_trans.name, 
                                            ref(list_subexpr_trans.expr_type)
                                         );
            }
            else if (typeid(*expr_trans.exprs[i]) == typeid(ConsExpr)) {
                ConsExpr &list_subexpr_trans = reinterpret_cast<ConsExpr &>(*expr_trans.exprs[i]);
                pattern_consexpr_infer(list_subexpr_trans, funcname);
            }
            else if (typeid(*expr_trans.exprs[i]) == typeid(ListExpr) || typeid(*expr_trans.exprs[i]) == typeid(SetExpr)) {
                pattern_list_or_set_infer(*expr_trans.exprs[i], funcname);
            }
            else {
                // lambda...
            }
        }
    }
    else {
        SetExpr &expr_trans = reinterpret_cast<SetExpr &>(expr);
        
        // trans_expr_type includes the institution type(s)
        TemplateType &trans_expr_type = reinterpret_cast<TemplateType &>(*expr_trans.expr_type); 

        // ins_type is the expr_type of the first element
        auto &ins_type = trans_expr_type.args[0];

        for (size_t i = 0; i < expr_trans.exprs.size(); ++i) {
            expr_trans.exprs[i]->expr_type = ins_type->clone();
            if (typeid(*expr_trans.exprs[i]) == typeid(VarExpr)) {
                VarExpr &set_subexpr_trans = reinterpret_cast<VarExpr &>(*expr_trans.exprs[i]);
                theArgTypeMapping.emplace(
                                            set_subexpr_trans.name, 
                                            ref(set_subexpr_trans.expr_type)
                                        );
            }
            else if (typeid(*expr_trans.exprs[i]) == typeid(ConsExpr)) {
                ConsExpr &set_subexpr_trans = reinterpret_cast<ConsExpr &>(*expr_trans.exprs[i]);
                pattern_consexpr_infer(set_subexpr_trans, funcname);
            }
            else if (typeid(*expr_trans.exprs[i]) == typeid(ListExpr) || typeid(*expr_trans.exprs[i]) == typeid(SetExpr)) {
                pattern_list_or_set_infer(*expr_trans.exprs[i], funcname);
            }
            else {
                // lambda...
            }
        }
    }
}
} // namespace hol2cpp