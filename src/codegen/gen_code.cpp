#include "codegen.hpp"

#include <cctype>
#include <iostream>
#include <algorithm>

using namespace std;

namespace hol2cpp {
/**
 * TODO: distinguish ShortDef with others
*/
void Theory::gen_code(Code &code) const {
    size_t predefined = 0, datatype_defs = 0, function_defs = 0, shortdef_defs = 0;
    vector<string> datatype_gens, function_gens, shortdef_gens;

    for (size_t i = 0; i < definitions.size(); ++i) {
        auto decl = definitions[i].get();

        if      (decl->is_datatype_decl()) { ++datatype_defs; }
        else if (decl->is_function_decl()) { ++function_defs; }
        else                               { ++shortdef_defs; }

        if (!decl->is_error()) try {
            if (decl->is_predefined()) { ++predefined; }
            else { decl->gen_code(code); }

            if (decl->is_datatype_decl()) {
                datatype_gens.push_back(decl->def_name());
            } else if (decl->is_function_decl()) {
                function_gens.push_back(decl->def_name());
            } else {
                shortdef_gens.push_back(decl->def_name());
            }
        } catch (const exception &e) {
            string name;
            if (auto datatype_decl = dynamic_cast<DataTypeDef *>(decl)) {
                name = "datatype " + datatype_decl->decl_type->main_name();
                code.pop_datatype();
            } else {
                auto fun_decl = static_cast<FunctionDef *>(decl);
                name = "function " + fun_decl->name;
                code.pop_function();
            }
            "$ at No.$ definition of $:\n`$\n\n"_fs.outf(
                cerr, info::light_red("codegen error"), i + 1, name, e.what()
            );
        }
    }

    auto defs = definitions.size() - shortdef_defs;
    auto gens = datatype_gens.size() + function_gens.size();
    "$\n`scanned $ definitions, contain $ datatypes and $ functions;\n"_fs.outf(
        cout, info::light_green("Result:"), defs, datatype_defs, function_defs
    );
    "`generated $ definitions, contain $ predefined:\n"_fs.outf(
        cout, gens, predefined
    );
    "``$ datatypes and $ functions.\n"_fs.outf(
        cout, datatype_gens.size(), function_gens.size()
    );

    cout << "````datatypes:" << endl;
    for (auto &datatype : datatype_gens) {
        cout << datatype << ", ";
    }

    cout << "\n````functions:" << endl;
    for (auto &function : function_gens) {
        cout << function << ", ";
    }

    cout << endl;
}

void DataTypeDef::gen_code(Code &code) const {
    code.add_header("variant");

    auto name = decl_type->main_name();
    auto &data_type = code.entry_data_type(name);

    data_type.name() = name;
    data_type.self() = decl_type->gen_datatype(data_type);

    vector<vector<Ptr<Type>>> abstracts;
    for (size_t i = 0; i < components.size(); ++i) {
        abstracts.push_back({});
        data_type.entry_component();
        data_type.add_constructor(components[i].constructor);
        code.bind_cons(components[i].constructor, data_type);

        for (auto &type : components[i].arguments) {
            auto field_type = type->gen_datatype(data_type);
            data_type.add_field_type(field_type);
            if (field_type == data_type.self()) {
                data_type.is_recuisive() = true;
                code.add_header("memory");
            }
            abstracts.back().push_back(move(type));
        }
    }
    data_type.abstracts() = move(abstracts);
}

void FunctionDef::gen_code(Code &code) const {
    auto &entity = code.entry_func_entity(name);

    entity.name() = name;
    type->gen_funcentity(entity);
    for (auto &equation : equations) {
        entity.entry_equation();
        entity.add_expr("// $", equation.raw_str);
        equation.gen_funcentity(entity);
        entity.close_equation();
    }

    if (!entity.statements().empty() && !entity.statements().back().empty()) {
        auto &last_stmt = entity.statements().back().back();
        if (last_stmt.back() == '}') {
            entity.app_last_stmt(" else { // auto-generated for -Wreturn-type");
            entity.add_indent().add_expr("std::abort();").sub_indent().add_expr("}");
        }
    }
}

void ShortDef::gen_code(Code &code) const {
    code.add_short_def(name, make_unique<ShortDef>(name, move(parameters), move(expr)));
}

/**
 * @brief check the datatype is pre-defined or not
 * 
 * @return true if the datatype is pre-defined
 * @return false if not
 */
bool DataTypeDef::is_predefined() const {
    return theNormalTypeMapping.count(decl_type->main_name())
        || theTemplateTypeMapping.count(decl_type->main_name())
    ;
}

/**
 * @brief check the function is pre-defined or not
 * 
 * @return true if the function is pre-defined
 * @return false if not
 */
bool FunctionDef::is_predefined() const {
    static set<string> predefineds {
        "length", "take", "drop", "append", "upto", "upt"
    };
    return predefineds.count(name);
}
} // namespace hol2cpp
