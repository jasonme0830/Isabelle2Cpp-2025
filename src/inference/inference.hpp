#pragma once

#include "../ir/ast.hpp"
#include <type_traits>

namespace hol2cpp {
struct TypeInference {
public:
    TypeInference(Theory &thy);
    ~TypeInference();

public:
    void theory_infer();

    void map_init(); 
    void map_dtypedef_cons(DatatypeDef &dtypedef);
    void map_funcdef_type(FunctionDef &funcdef);
    void map_srtdef_type(ShortDef &srtdef);

    void definition_infer(Definition &def);

    void shortdef_infer(ShortDef &srtdef);

    void functiondef_infer(FunctionDef &funcdef);
    void function_pattern_infer(Expr& pattern, FunctionDef &funcdef);
    void function_expr_infer(Expr &expr, const std::string &funcname);

    void pattern_infer(Ptr<Expr> &expr, const std::string &funcname);
    void pattern_consexpr_infer(ConsExpr &expr, const std::string &funcname);
    void pattern_list_or_set_infer(Expr &expr, const std::string &funcname);

    void analy_from_bottom(Expr &expr, const std::string &funcname);
    void analy_lambda_expr(Expr &expr, const std::string &funcname);


    void template_type_ins(Ptr<Type> &type);
    void template_type_map(Ptr<Type> &com, Ptr<Type> &ins);
    

    void function_apply(Ptr<Type> &functype, std::vector<Ptr<Expr>> &params);
    void function_apply_aux(Ptr<Type> &func_param_type, Ptr<Type> &ins_param_type);
    void function_update(std::vector<Ptr<Type>> &args);

    Ptr<Type> analy_if_expr(Expr &ifexpr, const std::string &funname);
    
    void gen_new_lambda_arg();
    bool is_lambda_arg(const std::string &argname);
    void print_theory();

private:
    Theory &thy;

    Ptr<NormalType> normal_nat_type;
    Ptr<NormalType> normal_bool_type;
    Ptr<NormalType> normal_unknown_type;

    std::vector<std::pair<std::reference_wrapper<Ptr<Type>>, std::reference_wrapper<Ptr<Type>>>> temp_vec;
    //std::vector<std::reference_wrapper<Ptr<Type>>> temp_vec;

    size_t lambda_counter;
    size_t lambda_depth;

    bool ins_map_clear_flag;
    std::string current_lambda_argname;

    /**
     * map constructor's name to its Datatypedef:
     * e.g. <Cons, slist>, <Nil, slist>...
    */
    std::map<std::string, std::reference_wrapper<DatatypeDef>> the_cons_type_mapping;

    /**
     * map function's name to its Functiondef:
     * e.g. <func1, func1::ref<FunctionDef>>
    */
    std::map<std::string, std::reference_wrapper<FunctionDef>> the_func_type_mapping;

    /**
     * map function's arguments to its Type:
     * e.g.
     * fun "test :: 'a list => nat" where
     * "test Nil = 0" |
     * "test (Cons x xs) = length (If ((length xs) = 0) Nil xs) + 1"
     * map:
     * <test, <x, Ptr<ArgumentType>>>, <test, <xs, Ptr<TemplateType>>>
    */
    std::map<std::string, std::reference_wrapper<Ptr<Type>>> the_arg_type_mapping;

    /**
     * map type araguments to its institution type:
     * e.g.
     * fun "test :: nat list => nat" where...
     * map:
     * <'a, nat>
    */
    std::map<std::string, std::reference_wrapper<Ptr<Type>>> the_argument_type_ins_mapping;

    std::map<std::string, std::size_t> the_lambda_ins_mapping;
    
    std::vector<Ptr<Type>> the_lambda_storge;
};

} // namespace hol2cpp