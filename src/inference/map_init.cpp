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
        if (auto def = dynamic_cast<DatatypeDef *>(def_ptr.get())) {
            map_dtypedef_cons(*def);
        } else if (auto def = dynamic_cast<FunctionDef *>(def_ptr.get())) {
            map_funcdef_type(*def);
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
