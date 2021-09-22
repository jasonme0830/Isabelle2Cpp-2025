#include "code.hpp"
#include "format.hpp"
#include "funcentity.hpp"

#include <fstream>

using namespace std;

namespace hol2cpp
{
Code::Code(string filename)
  : filename_(move(filename)), out_(header_out_)
  , indent_(0), headers_{"cstdlib"} {
    header_out_ = ofstream(filename_ + ".hpp");
    if (!header_out_.good()) {
        cout << "can't open file " << filename << ".hpp" << endl;
        return;
    }
    impl_out_ = ofstream(filename_ + ".cpp");
    if (!impl_out_.good()) {
        cout << "can't open file " << filename << ".cpp" << endl;
        return;
    }
}

DataType &Code::entry_data_type(const string &name) {
    names_of_data_types_.push_back(name);
    return data_types_[name];
}

DataType *Code::find_data_type(const string &name) {
    if (data_types_.count(name)) {
        return &data_types_[name];
    } else {
        return nullptr;
    }
}

void Code::bind_cons(const string &cons, DataType &data_type) {
    cons_types_.emplace(cons, data_type);
}

DataType *Code::find_data_type_by_cons(const string &name) {
    if (cons_types_.count(name)) {
        return &cons_types_.at(name);
    } else {
        return nullptr;
    }
}

FuncEntity &Code::entry_func_entity(const string &name) {
    func_entities_.emplace(name, *this);
    names_of_func_entities_.push_back(name);
    return func_entities_.at(name);
}

FuncEntity *Code::find_func_entity(const string &name) {
    if (func_entities_.count(name)) {
        return &func_entities_.at(name);
    } else {
        return nullptr;
    }
}

void Code::add_short_def(const string &name, Ptr<ShortDef> short_def) {
    short_defs_.emplace(name, move(short_def));
}

const ShortDef *Code::get_short_def(const string &name) const {
    if (short_defs_.count(name)) {
        return short_defs_.at(name).get();
    } else {
        return nullptr;
    }
}

void Code::generate() {
    generate_header();
    generate_impl();
}

void Code::generate_header() {
    out_ = ref(header_out_);

    for (const auto &header : headers_) {
        "#include <$>\n"_fs.outf(out_.get(), header);
    }
    out_.get() << endl;

    for (auto &name : names_of_data_types_) {
        gen_data_type(data_types_[name]);
    }

    for (auto &name : names_of_func_entities_) {
        gen_single_func(func_entities_.at(name), false);
    }
}

void Code::generate_impl() {
    out_ = ref(impl_out_);

    "#include \"$\"\n\n"_fs.outf(out_.get(), filename_.substr(filename_.rfind('/') + 1) + ".hpp");

    for (auto &name : names_of_func_entities_) {
        gen_single_func(func_entities_.at(name));
    }
}

size_t &Code::indent_size() {
    static size_t value = 4;
    return value;
}

void Code::add_header(const string &header) {
    headers_.insert(header);
}

void Code::pop_datatype() {
    auto name = names_of_data_types_.back();
    for (auto &cons : data_types_[name].constructors()) {
        cons_types_.erase(cons);
    }
    data_types_.erase(name);
    names_of_data_types_.pop_back();
}

void Code::pop_function() {
    auto name = names_of_func_entities_.back();
    func_entities_.erase(name);
    names_of_func_entities_.pop_back();
}

void Code::gen_data_type(DataType &data_type) {
    indent_ = 0;

    "enum $Cons {\n"_fs.outf(newline(), data_type.name());
    add_indent();
    for (auto &cons : data_type.constructors()) {
        "$,\n"_fs.outf(newline(), cons);
    }
    sub_indent();
    "};\n\n"_fs.outf(newline());

    if (data_type.is_normal_type()) {
        gen_normal_type_header(data_type);
    } else {
        gen_template_type_header(data_type);
    }
    gen_type_rest(data_type);
}

void Code::gen_type_rest(DataType &data_type) {
    const auto &origin = data_type.name();
    auto name = data_type.name();
    if (data_type.is_recuisive()) {
        name += "Elem";
    }

    TypeInfo variant("std::variant");

    "struct $ {\n"_fs.outf(newline(), name);
    add_indent();
    auto &components = data_type.components();
    for (size_t i = 0; i < components.size(); ++i) {
        if (components[i].empty()) {
            continue;
        }
        variant.arguments.emplace_back("c$"_fs.format(i + 1));

        "struct c$ {\n"_fs.outf(newline(), i + 1);
        add_indent();
        for (size_t j = 0; j < components[i].size(); ++j) {
            "$ p$;\n"_fs.outf(newline(), components[i][j], j + 1);
        }
        sub_indent();
        "};\n"_fs.outf(newline());
    }   out_.get() << endl;

        "$($Cons cons) : cons(cons) {}\n\n"_fs.outf(newline(), name, origin);

    for (size_t i = 0; i < components.size(); ++i) {
        if (components[i].empty()) {
            continue;
        }

        "c$ &get_c$() {\n"_fs.outf(newline(), i + 1, i + 1);
        add_indent();
            "return std::get<c$>(value);\n"_fs.outf(newline(), i + 1);
        sub_indent();
        "};\n"_fs.outf(newline());

        "void set_c$("_fs.outf(newline(), i + 1);
        for (size_t j = 0; j < components[i].size(); ++j) {
            if (j == 0) {
                "$ _p1"_fs.outf(out_.get(), components[i][j]);;
            } else {
                ", $ _p$"_fs.outf(out_.get(), components[i][j], j + 1);
            }
        }
        ") {\n"_fs.outf(out_.get());

        add_indent();
            "value = c${"_fs.outf(newline(), i + 1);
        for (size_t j = 0; j < components[i].size(); ++j) {
            if (j == 0) {
                "_p1"_fs.outf(out_.get());
            } else {
                ", _p$"_fs.outf(out_.get(), j + 1);
            }
        }
        "};\n"_fs.outf(out_.get());
        sub_indent();
        "}\n"_fs.outf(newline());
    }
        out_.get() << endl;

        "$Cons cons;\n"_fs.outf(newline(), origin);
        "$ value;\n"_fs.outf(newline(), variant.to_str());
    sub_indent();
    "};\n\n"_fs.outf(newline());
}

void Code::gen_normal_type_header(DataType &data_type) {
    if (!data_type.is_recuisive()) {
        return;
    }

    const auto &origin = data_type.name();
    auto name = data_type.name() + "Elem";

    "struct $;\n"_fs                        .outf(newline(), name);
    "using $ = std::shared_ptr<$>;\n\n"_fs  .outf(newline(), origin, name);
}

void Code::gen_template_type_header(DataType &data_type) {
    const auto &origin = data_type.name();
    auto name = data_type.name();
    auto &targs = data_type.template_args();
    "template<"_fs.outf(newline());
    for (size_t i = 0; i < targs.size(); ++i) {
        if (i == 0) {
            "typename $"_fs.outf(out_.get(), targs[i]);
        } else {
            ", typename $"_fs.outf(out_.get(), targs[i]);
        }
    }
    ">\n"_fs.outf(out_.get());

    if (!data_type.is_recuisive()) {
        return;
    } else {
        name += "Elem";
    }

    "struct $;\n"_fs.outf(newline(), name);
    if (data_type.is_recuisive()) {
        "template<"_fs.outf(newline());
        for (size_t i = 0; i < targs.size(); ++i) {
            if (i == 0) {
                "typename $"_fs.outf(out_.get(), targs[i]);
            } else {
                ", typename $"_fs.outf(out_.get(), targs[i]);
            }
        }
        ">\n"_fs.outf(out_.get());
        "using $ = std::shared_ptr<$<"_fs.outf(newline(), origin, name);
        for (size_t i = 0; i < targs.size(); ++i) {
            if (i == 0) {
                out_.get() << targs[i];
            } else {
                ", $"_fs.outf(out_.get(), targs[i]);
            }
        }
        ">>;\n\n"_fs.outf(out_.get());
    }

    "template<"_fs.outf(newline());
    for (size_t i = 0; i < targs.size(); ++i) {
        if (i == 0) {
            "typename $"_fs.outf(out_.get(), targs[i]);
        } else {
            ", typename $"_fs.outf(out_.get(), targs[i]);
        }
    }
    ">\n"_fs.outf(out_.get());
}

void Code::gen_single_func(FuncEntity &entity, bool is_impl) {
    indent_ = 0;
    if (entity.template_args().empty()) {
        gen_normal_func(entity, is_impl);
    } else {
        gen_template_func(entity, is_impl);
    }
}

void Code::gen_normal_func(FuncEntity &entity, bool is_impl) {
    auto &types = entity.typeinfos();
    "$ $("_fs.outf(newline(), types.back().to_str(), entity.name());
    for (size_t i = 0; i < types.size() - 1; ++i) {
        if (i == 0) {
            "$arg$"_fs.outf(out_.get(), types[i].to_str_as_arg(), i + 1);
        } else {
            ", $arg$"_fs.outf(out_.get(), types[i].to_str_as_arg(), i + 1);
        }
    }

    if (!is_impl) {
        ");\n\n"_fs.outf(out_.get());
        return;
    }

    ") {\n"_fs.outf(out_.get());

    auto &statements = entity.statements();
    add_indent();
    for (size_t i = 0; i < statements.size(); ++i) {
        "for (;;) {\n"_fs.outf(newline());

        add_indent();
        for (auto &statement : statements[i]) {
            newline() << statement << endl;
        }
        sub_indent();

        "}\n"_fs.outf(newline());
    }
    "std::abort();\n"_fs.outf(newline());
    sub_indent();
    "}\n\n"_fs.outf(newline());
}

void Code::gen_template_func(FuncEntity &entity, bool is_impl) {
    auto &template_args = entity.template_args();
    "template<"_fs.outf(newline());
    for (size_t i = 0; i < template_args.size(); ++i) {
        if (i == 0) {
            "typename $"_fs.outf(out_.get(), template_args[i]);
        } else {
            ", typename $"_fs.outf(out_.get(), template_args[i]);
        }
    }
    ">\n"_fs.outf(out_.get());
    gen_normal_func(entity, is_impl);
}

ostream &Code::newline() {
    out_.get() << string(indent_, ' ');
    return out_.get();
}

void Code::add_indent() {
    indent_ += indent_size();
}

void Code::sub_indent() {
    indent_ -= indent_size();
}
} // namespace hol2cpp
