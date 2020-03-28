#include "code.hpp"

using namespace std;

namespace hol2cpp {
void Code::generate(ostream &out) {
  for (auto &entity : func_entities_) {
    generate_single(out, entity);
    out << endl;
  }
}

void Code::generate_single(ostream &out, FuncEntity &entity) {
  if (entity.template_args().empty()) {
    generate_normal(out, entity);
  } else {
    generate_template(out, entity);
  }
}

void Code::generate_normal(ostream &out, FuncEntity &entity) {
  auto &types = entity.types();
  out << types.front() << " " << entity.name() << "(";
  for (size_t i = 1; i < types.size(); ++i) {
    if (i == 1) {
      out << types[i];
    } else {
      out << ", " << types[i];
    }
  }
  out << ") {}" << endl;
}

void Code::generate_template(ostream &out, FuncEntity &entity) {
  auto &template_args = entity.template_args();
  out << "template<";
  for (size_t i = 0; i < template_args.size(); ++i) {
    if (i == 0) {
      out << "typename " << template_args[i];
    } else {
      out << ", typename " << template_args[i];
    }
  }
  out << ">" << endl;
  generate_normal(out, entity);
}
}
