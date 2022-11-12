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
        } 
        else if (auto def = dynamic_cast<FunctionDef *>(def_ptr.get())) {
            map_funcdef_type(*def);
        }
        else continue;
    }
}

void TypeInference::map_dtypedef_cons(DatatypeDef &dtypedef) {
    for (auto &component : dtypedef.components) {
        the_dtypedef_mapping.emplace(component.constructor, ref(dtypedef));

        Ptr<FuncType> functype = make_shared<FuncType>();

        for (auto &argument: component.arguments) {
            functype->types.push_back(argument->clone());
        }
        functype->types.push_back(dtypedef.decl_type);

        the_cons_type_mapping.emplace(component.constructor, functype);
    }
}

void TypeInference::map_funcdef_type(FunctionDef &funcdef) {
    the_func_type_mapping.emplace(funcdef.name, ref(funcdef));
}

void TypeInference::map_srtdef_type(ShortDef &srtdef) {
    // todo...
}
} // namespace hol2cpp
