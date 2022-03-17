#include "codegen.hpp"

using namespace std;

namespace hol2cpp {
string NormalType::gen_datatype(Datatype &) const {
    return name;
}

string ArgumentType::gen_datatype(Datatype &type) const {
    return type.add_argument_type(name);
}

string TemplateType::gen_datatype(Datatype &type) const {
    auto res = name + '<';
    for (size_t i = 0; i < args.size(); ++i) {
        if (i == 0) {
            res += args[i]->gen_datatype(type);
        } else {
            res += ", " + args[i]->gen_datatype(type);
        }
    }
    return res + '>';
}

string FuncType::gen_datatype(Datatype &type) const {
    string res = "std::function<" + result_type()->gen_datatype(type) + '(';
    for (size_t i = 0; i < types.size() - 1; ++i) {
        if (i == 0) {
            res += types[i]->gen_datatype(type);
        } else {
            res += ", " + types[i]->gen_datatype(type);
        }
    }
    return res + ")>";
}

/**
 * @brief get the main name of the type, which is usually used for datatypes
 *
 * @return "list" for 'a list
 */
string Type::main_name() const {
    throw CodegenError("failed call of Type::main_name()");
}

string NormalType::main_name() const {
    return name;
}

string TemplateType::main_name() const {
    return name;
}
} // namespace hol2cpp
