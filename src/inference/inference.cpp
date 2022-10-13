#include "inference.hpp"

using namespace std;

namespace hol2cpp {

TypeInference::TypeInference(Theory &thy)
  : thy(thy), 
  ins_map_clear_flag(true), 
  normal_nat_type(new NormalType("nat")), 
  normal_bool_type(new NormalType("bool")),
  normal_unknown_type(new NormalType("UNKNOWN_TYPE")),
  lambda_counter(0),
  lambda_depth(0),
  current_lambda_argname("") {
    // ...
}

TypeInference::~TypeInference() = default;

/**
 * Inference theory
*/

void TypeInference::theory_infer() {
    map_init();
    for(auto &elem: thy.definitions) {
        definition_infer(*elem);
    }
}

/**
 * Inference definitions
*/ 

void TypeInference::definition_infer(Definition &def) {
    if (typeid(def) == typeid(ShortDef)) {
        ShortDef &trans = reinterpret_cast<ShortDef &>(def);
        shortdef_infer(trans);
    }
    else if (typeid(def) == typeid(FunctionDef)) {
        FunctionDef &trans = reinterpret_cast<FunctionDef &>(def);
        functiondef_infer(trans);
    }
    else return;
}

void TypeInference::shortdef_infer(ShortDef &srtdef) {
    // todo...
}

/**
 * Inference patterns
*/ 

void TypeInference::functiondef_infer(FunctionDef &funcdef) {
    for (auto &equation : funcdef.equations) {
        /**
         * argnum is the index of return type
         * mark the type of equation's expr
        */
        equation.expr->expr_type = funcdef.type->types[funcdef.type->types.size() - 1];

        function_pattern_infer(*equation.pattern, funcdef);
        function_expr_infer(*equation.expr, funcdef.name);
        theArgTypeMapping.clear();
    }
}

void TypeInference::function_pattern_infer(Expr& pattern, FunctionDef& funcdef) {
    // traverse funcdef's equations
    auto argnum = funcdef.type->types.size() - 1;

    try {
        ConsExpr &pattern_trans = dynamic_cast<ConsExpr &>(pattern);
        // traverse pattern's sub-exprs
        for (size_t i = 0; i < argnum; ++i) {
            // first step : map functions' arglist to the type of pattern's sub-exprs
            pattern_trans.args[i]->expr_type = funcdef.type->types[i]->clone();
            pattern_infer(pattern_trans.args[i], funcdef.name); 
        }
    }
    catch (bad_cast &) {
        cerr << "pattern_infer(): bad cast ConsExpr.\n";
    }
}

/*
 * Inference exprs
*/

void TypeInference::function_expr_infer(Expr &expr, const string &funcname) {
    analy_from_bottom(expr, funcname);
    analy_lambda_expr(expr, funcname);
}
} // namespace hol2cpp