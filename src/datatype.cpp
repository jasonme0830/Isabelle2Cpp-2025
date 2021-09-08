#include "datatype.hpp"

namespace hol2cpp {
bool &DataType::is_recuisive() {
    return is_recuisive_;
}

bool DataType::is_normal_type() {
    return template_args_.empty();
}

std::string &DataType::name() {
    return name_;
}

std::vector<std::string> &DataType::template_args() {
    return template_args_;
}

std::size_t DataType::find_argument_type(const std::string &name) {
    return template_mapping_.at(name);
}

std::string DataType::add_argument_type(std::string name) {
    if (!template_mapping_.count(name)) {
        template_mapping_[name] = template_args_.size();
        template_args_.push_back("T" + std::to_string(template_args_.size() + 1));
    }
    return template_args_[template_mapping_[name]];
}

std::size_t DataType::pos_of_cons(const std::string &cons) {
    return constructor_mapping_[cons];
}

void DataType::add_constructor(const std::string &cons) {
    constructor_mapping_[cons] = constructors_.size();
    constructors_.push_back(cons);
}

const std::vector<std::string> &DataType::constructors() const {
    return constructors_;
}

void DataType::entry_component() {
    components_.emplace_back();
}

void DataType::add_field_type(const std::string &type) {
    components_.back().push_back(type);
}

const std::vector<std::vector<std::string>> &DataType::components() const {
    return components_;
}

std::vector<std::vector<Ptr<Type>>> &DataType::abstracts() {
    return abstracts_;
}
} // namespace hol2cpp
