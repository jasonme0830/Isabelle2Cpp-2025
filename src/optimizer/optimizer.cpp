#include "optimizer.hpp"
#include "../ir/funcentity.hpp"

using namespace std;

namespace hol2cpp {
void Optimizer::enable_list_move() {
    option_.enable_list_move = true;

    auto origin_as_arg = TypeInfo::as_arg;
    TypeInfo::as_arg = [=](const TypeInfo &type) {
        if (type.name == "std::list") {
            return type.to_str();
        } else {
            return origin_as_arg(type);
        }
    };
}

const OptimizerOption &Optimizer::option() const {
    return option_;
}
} // namespace hol2cpp
