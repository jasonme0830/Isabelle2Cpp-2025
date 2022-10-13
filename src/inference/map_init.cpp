#include "inference.hpp"

using namespace std;

namespace hol2cpp {
/**
 * map datatypes' constructor to the DatatypeDef
 * map the arguments of function's pattern
*/

void TypeInference::map_init() {
    for(auto &def_ptr : thy.definitions) {
        auto &def = *def_ptr;
        if (typeid(def) == typeid(DatatypeDef)) {
            DatatypeDef &trans = reinterpret_cast<DatatypeDef &>(def);
            map_dtypedef_cons(trans);
        }
        else if (typeid(def) == typeid(FunctionDef)) {
            FunctionDef &trans = reinterpret_cast<FunctionDef &>(def);
            map_funcdef_type(trans);
        }
        else if (typeid(def) == typeid(PreFunctionDef)) {
            PreFunctionDef &trans = reinterpret_cast<PreFunctionDef &>(def);
            map_prefuncdef_type(trans);
        }
        else continue;
    }
    // need ShortDef map init
    // for (auto &def_ptr : thy.definitions) {
    //     auto &def = *def_ptr;
    //     if (typeid(def) == typeid(ShortDef)) {
    //         try {
    //             ShortDef &trans = dynamic_cast<ShortDef &>(def);
    //             map_srtdef(trans);
    //         }
    //         catch (bad_cast &) {
    //             cerr << "bad cast ShortDef." << endl;
    //         }
    //     }
    //     else if (typeid(def) == typeid(FunctionDef)) {
    //         try {
    //             FunctionDef &trans = dynamic_cast<FunctionDef &>(def);
    //             map_funcdef(trans);
    //         }
    //         catch (bad_cast &) {
    //             cerr << "bad cast FunctionDef." << endl;
    //         }
    //     }
    //     else {
    //         cerr << "bad match" << endl;
    //     }
    // }
}

void TypeInference::map_dtypedef_cons(DatatypeDef &dtypedef) {
    for (auto &coms : dtypedef.components) {
        theConsTypeMapping.emplace(coms.constructor, ref(dtypedef));
    }
}

void TypeInference::map_funcdef_type(FunctionDef &funcdef) {
    theFuncTypeMapping.emplace(funcdef.name, ref(funcdef));
}

void TypeInference::map_prefuncdef_type(PreFunctionDef &prefuncdef) {
    thePreDefFuncTypeMapping.emplace(prefuncdef.name, ref(prefuncdef));
}

void TypeInference::map_srtdef_type(ShortDef &srtdef) {
    // todo...
}
} // namespace hol2cpp