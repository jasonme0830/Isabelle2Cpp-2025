#include "../ir/ast.hpp"

using namespace std;

namespace hol2cpp {
Ptr<Type> Type::clone() const {
    Ptr<Type> other(this->clone());
    assert(typeid(*this) == typeid(*other));
    return other;
}

Ptr<Type> NormalType::clone() const {
    return make_shared<NormalType>(*this);
}

Ptr<Type> ArgumentType::clone() const {
    return make_shared<ArgumentType>(*this);
}

Ptr<Type> TemplateType::clone() const {
    Ptr<TemplateType> tmpltype = make_shared<TemplateType>();
    tmpltype->name = name;
    for (auto iter = args.cbegin(); iter != args.cend(); ++iter) {
        tmpltype->args.push_back(iter->get()->clone());
    }
    return tmpltype;
}

Ptr<Type> FuncType::clone() const {
    Ptr<FuncType> functype = make_shared<FuncType>();
    for (auto iter = types.cbegin(); iter != types.cend(); ++iter) {
        functype->types.push_back(iter->get()->clone());
    }
    return functype;
}
} // namespace hol2cpp