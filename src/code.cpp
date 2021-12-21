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

    if (!data_type.is_normal_type()) {
        gen_template_type_header(data_type);
    }
    gen_type_rest(data_type);
}

void Code::gen_type_rest(DataType &data_type) {
    auto &name = data_type.name();
    auto &self = data_type.self();
    auto &constructors = data_type.constructors();

    TypeInfo variant("std::variant");

    "struct $ {\n"_fs.outf(newline(), name);
    add_indent();
    auto &components = data_type.components();

    // generate struct _Ci { ... };
    for (size_t i = 0; i < components.size(); ++i) {
        variant.arguments.emplace_back("_$"_fs.format(constructors[i]));

        if (components[i].empty()) {
            "struct _$ {};\n"_fs.outf(newline(), constructors[i]);
        } else {
            "struct _$ {\n"_fs.outf(newline(), constructors[i]);
            add_indent();
            // generate members
            for (size_t j = 0; j < components[i].size(); ++j) {
                if (components[i][j] == self) {
                    "std::shared_ptr<$> p$_;\n"_fs.outf(newline(), components[i][j], j + 1);
                } else {
                    "$ p$_;\n"_fs.outf(newline(), components[i][j], j + 1);
                }
            }
            out_.get() << endl;

            // generate methods
            for (size_t j = 0; j < components[i].size(); ++j) {
                if (components[i][j] == self) {
                    "$ p$() const { return *p$_; }\n"_fs.outf(newline(), components[i][j], j + 1, j + 1);
                } else {
                    "const $ &p$() const { return p$_; }\n"_fs.outf(newline(), components[i][j], j + 1, j + 1);
                }
            }
            sub_indent();
            "};\n"_fs.outf(newline());
        }
    }
    out_.get() << endl;

    // generate std::variant<_C0, ..., _Ck> value_;
    "$ value_;\n\n"_fs.outf(newline(), variant.to_str());

    // generate static constructions
    for (size_t i = 0; i < components.size(); ++i) {
        "static $ $("_fs.outf(newline(), self, constructors[i]);
        for (size_t j = 0; j < components[i].size(); ++j) {
            if (j == 0) {
                "$ p1"_fs.outf(out_.get(), components[i][j]);;
            } else {
                ", $ p$"_fs.outf(out_.get(), components[i][j], j + 1);
            }
        }
        ") {\n"_fs.outf(out_.get());
        add_indent();

        "return $ { _$ { "_fs.outf(newline(), self, constructors[i]);
        for (size_t j = 0; j < components[i].size(); ++j) {
            if (components[i][j] == self) {
                if (j == 0) {
                    "std::make_shared<$>(p1)"_fs.outf(out_.get(), self);
                } else {
                    ", std::make_shared<$>(p$)"_fs.outf(out_.get(), self, j + 1);
                }
            } else {
                if (j == 0) {
                    "p1"_fs.outf(out_.get());
                } else {
                    ", p$"_fs.outf(out_.get(), j + 1);
                }
            }
        }
        " } };\n"_fs.outf(out_.get());

        sub_indent();
        "}\n"_fs.outf(newline());
    }
    out_.get() << endl;

    // generate is_Ci()
    for (auto &constructor : constructors) {
        "bool is_$() const { return std::holds_alternative<_$>(value_); }\n"_fs.outf(newline(), constructor, constructor);
    }
    out_.get() << endl;

    // generate as_Ci()
    for (size_t i = 0; i < components.size(); ++i) {
        if (components[i].empty()) {
            continue;
        }

        "const _$ &as_$() const { return std::get<_$>(value_); }"_fs.outf(newline(), constructors[i], constructors[i], constructors[i]);
    }
    out_.get() << endl;

    sub_indent();
    "};\n\n"_fs.outf(newline());
}

void Code::gen_template_type_header(DataType &data_type) {
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
        if (i) {
            "\n"_fs.outf(out_.get());
        }

        for (auto &statement : statements[i]) {
            if (!statement.empty()) {
                newline() << statement << endl;
            }
        }
    }
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
