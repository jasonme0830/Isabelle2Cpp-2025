#include "synthesizer.hpp"
#include "../utility/config.hpp"

using namespace std;

namespace hol2cpp{

void
Synthesizer::syn_class(const Datatype& datatype)
{
  indent_ = 0;
  if (!datatype.is_normal_type()) {
    syn_class_template(datatype);
  }
  syn_class_definition(datatype);
}

void
Synthesizer::syn_class_template(const Datatype& datatype)
{
  auto& targs = datatype.template_args();
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

void
Synthesizer::syn_class_definition(const Datatype& datatype)
{
  auto use_class = theConfig.use_class();

  auto& name = datatype.name();
  auto& self = datatype.self();
  auto& constructors = datatype.constructors();

  TypeInfo variant("std::variant");

  if (use_class) {
    "class $ {\n"_fs.outf(newline(), name);
  } else {
    "struct $ {\n"_fs.outf(newline(), name);
  }

  add_indent();
  auto& components = datatype.components();

  // generate struct _Ci { ... };
  for (size_t i = 0; i < components.size(); ++i) {
    variant.arguments.emplace_back("_$"_fs.format(constructors[i]));

    if (components[i].empty()) {
      syn_class_def_struct_empty(datatype, i);
    } else {
      "struct _$ {\n"_fs.outf(newline(), constructors[i]);
      add_indent();

      // for operator<
      string lhs, rhs;

      // generate members
      syn_class_def_struct_members(datatype, i, lhs, rhs);

      // generate methods
      syn_class_def_struct_getMethod(datatype, i);
      out_.get() << endl;

      //参数拷贝构造函数
      syn_class_def_struct_paramConstructor(datatype, i);
      //深拷贝构造函数
      syn_class_def_struct_copyConstructor(datatype, i);
      //移动构造函数
      syn_class_def_struct_moveConstructor(datatype, i);
      out_.get()<<endl;
      
      //重载比较运算符<
      syn_class_def_struct_compareOperator(datatype, i, lhs, rhs);
      //重载相等运算符==
      syn_class_def_struct_equivOperator(datatype, i, lhs, rhs);
      //重载拷贝赋值运算符=
      syn_class_def_struct_copyOperator(datatype, i);
      //重载移动赋值运算符=
      syn_class_def_struct_moveOperator(datatype, i);


      sub_indent();
      "};\n"_fs.outf(newline()); 
    }
  }
  out_.get() << endl;
  
  // generate std::variant<_C0, ..., _Ck> value_;
  if (use_class) {
    "public:\n"_fs.outf(newline(-2));
    out_.get()<<endl;
  } 
  // generate std::variant<_C0, ..., _Ck> value_;
  syn_class_def_variant(datatype, variant);
  out_.get()<<endl;
  
  //默认构造函数
  syn_class_def_defaultConstructor(datatype, variant);
  //value构造函数
  syn_class_def_valueConstructor(datatype, variant);
  //拷贝构造函数
  syn_class_def_copyConstructor(datatype);
  //移动构造函数
  syn_class_def_moveConstructor(datatype);
  out_.get() << endl;

  // generate static constructions
  syn_class_def_staticConstructor(datatype);
  // generate is_Ci()
  syn_class_def_isfunction(datatype);
  // generate as_Ci()
  syn_class_def_asfunction(datatype);

  // generate operator<
  syn_class_def_compareOperator(datatype);
  // generate operator==
  syn_class_def_equivOperator(datatype);
  // generate move operator=
  syn_class_def_moveOperator(datatype);
  //generate copy operator=
  syn_class_def_copyOperator(datatype);

  out_.get()<<endl;

  sub_indent();
  "};\n\n"_fs.outf(newline());
}

void
Synthesizer::syn_class_def_variant(const Datatype& datatype, const TypeInfo& variant){
  "$ value_;\n"_fs.outf(newline(), variant.to_str(-1));
}
void
Synthesizer::syn_class_def_defaultConstructor(const Datatype& datatype, const TypeInfo& variant){
  auto& name = datatype.name();
  // auto& self = datatype.self();
  auto& constructors = datatype.constructors();
  auto& components = datatype.components();

  "//默认构造函数\n"_fs.outf(newline());
  "$(){\n"_fs.outf(newline(), name);
  add_indent();
  for (size_t i = 0; i < components.size();++i)  {
    if(components[i].size() == 0){
      "value_ = _$();\n"_fs.outf(newline(), constructors[i]);
    }
  }
  sub_indent();
  "}\n"_fs.outf(newline());
}
void
Synthesizer::syn_class_def_valueConstructor(const Datatype& datatype, const TypeInfo& variant){
  auto& name = datatype.name();

  "//value构造函数\n"_fs.outf(newline());
  "$($value) : value_(value) {}\n"_fs.outf(
      newline(), name, variant.to_str_as_arg(-1));
}
void
Synthesizer::syn_class_def_copyConstructor(const Datatype& datatype){

  auto& name = datatype.name();
  auto& self = datatype.self();
  auto& constructors = datatype.constructors();
  // auto& components = datatype.components();
  "//深拷贝构造函数\n"_fs.outf(newline());
  "$(const $& other) { \n"_fs.outf(newline(), name, self);
    add_indent();
    for(auto& constructor : constructors){
      "if(std::holds_alternative<_$>(other.value_)){ \n"_fs.outf(newline(), constructor);
        add_indent();
        "const _$& other_node = std::get<_$>(other.value_); \n"_fs.outf(newline(), constructor, constructor);
        "value_ = other_node;\n"_fs.outf(newline());
        sub_indent();
      "} \n"_fs.outf(newline());
    }
    sub_indent();
  "} \n"_fs.outf(newline());
}
void
Synthesizer::syn_class_def_moveConstructor(const Datatype& datatype){
  auto& name = datatype.name();
  auto& self = datatype.self();
  auto& constructors = datatype.constructors();
  // auto& components = datatype.components();
  "//移动构造函数\n"_fs.outf(newline());
  "$($&& other){\n"_fs.outf(newline(), name, self);
    add_indent();
    for(auto& constructor : constructors){
      "if(std::holds_alternative<_$>(other.value_)){ \n"_fs.outf(newline(), constructor);
        add_indent();
        "_$& other_node = std::get<_$>(other.value_); \n"_fs.outf(newline(), constructor, constructor);
        "value_ = std::move(other_node);\n"_fs.outf(newline());
        sub_indent();
      "} \n"_fs.outf(newline());
    }
    sub_indent();
  "}\n"_fs.outf(newline());
}
void
Synthesizer::syn_class_def_staticConstructor(const Datatype& datatype){
  auto& name = datatype.name();
  auto& self = datatype.self();
  auto& constructors = datatype.constructors();
  auto& components = datatype.components();

  for (size_t i = 0; i < components.size(); ++i) {
    "static $ $("_fs.outf(newline(), self, constructors[i]);
    for (size_t j = 0; j < components[i].size(); ++j) {
      if (j == 0) {
        "const $ &p1"_fs.outf(out_.get(), components[i][j]);
        ;
      } else {
        ", const $ &p$"_fs.outf(out_.get(), components[i][j], j + 1);
      }
    }
    ") {\n"_fs.outf(out_.get());
    add_indent();

    "return $ ( _$ ( "_fs.outf(newline(), self, constructors[i]);
    for (size_t j = 0; j < components[i].size(); ++j) {
      if (j == 0) {
        "p1"_fs.outf(out_.get());
      } else {
        ", p$"_fs.outf(out_.get(), j + 1);
      }
    }
    " ) );\n"_fs.outf(out_.get());

    sub_indent();
    "}\n"_fs.outf(newline());
  }
}
void
Synthesizer::syn_class_def_isfunction(const Datatype& datatype){

  auto& constructors = datatype.constructors();

  out_.get() << endl;
  for (auto& constructor : constructors) {
    "bool is_$() const { return std::holds_alternative<_$>(value_); }\n"_fs
      .outf(newline(), constructor, constructor);
  }
}
void
Synthesizer::syn_class_def_asfunction(const Datatype& datatype){

  auto& constructors = datatype.constructors();
  auto& components = datatype.components();

  for (size_t i = 0; i < components.size(); ++i) {
    if (!components[i].empty()) {
      "const _$ &as_$() const { return std::get<_$>(value_); }\n"_fs.outf(
        newline(), constructors[i], constructors[i], constructors[i]);
    }
  }
}
void
Synthesizer::syn_class_def_compareOperator(const Datatype& datatype){

  auto& self = datatype.self();

  out_.get() << endl;
  "bool operator<(const $ &rhs) const { return value_ < rhs.value_; }\n"_fs
    .outf(newline(), self);
}
void
Synthesizer::syn_class_def_equivOperator(const Datatype& datatype){
  auto& self = datatype.self();

  out_.get() << endl;
  "bool operator==(const $ &rhs) const { return value_ == rhs.value_; }\n"_fs
    .outf(newline(), self);
}
void
Synthesizer::syn_class_def_copyOperator(const Datatype& datatype){

  auto& self = datatype.self();
  auto& constructors = datatype.constructors();

  "//拷贝赋值运算符\n"_fs.outf(newline());
  "$& operator=(const $& other){ \n"_fs.outf(newline(), self, self);
    add_indent();
    "if(this != &other){ \n"_fs.outf(newline());
      add_indent();
      for(auto& constructor : constructors){
        "if(std::holds_alternative<_$>(other.value_)){ \n"_fs.outf(newline(), constructor);
          add_indent();
          "const _$& other_node = std::get<_$>(other.value_); \n"_fs.outf(newline(), constructor, constructor);
          "value_ = other.value_; \n"_fs.outf(newline());
          sub_indent();
        "} \n"_fs.outf(newline());
      }
      sub_indent();
    "} \n"_fs.outf(newline());
    "return *this; \n"_fs.outf(newline());
    sub_indent();
  "}\n"_fs.outf(newline());
}
void
Synthesizer::syn_class_def_moveOperator(const Datatype& datatype){

  auto& self = datatype.self();
  auto& constructors = datatype.constructors();

  "$& operator=($&& other) noexcept {\n"_fs.outf(newline(), self, self);
    add_indent();
    "if(this != &other){\n"_fs.outf(newline());
      add_indent();
      for(const auto& constructor : constructors){
        "if(std::holds_alternative<_$>(other.value_)){\n"_fs.outf(newline(), constructor);
          add_indent();
          "_$& other_value = std::get<_$>(other.value_);\n"_fs.outf(newline(),constructor, constructor);
          "value_ = std::move(other_value);\n"_fs.outf(newline());
          sub_indent();
        "}\n"_fs.outf(newline());
      }
      sub_indent();
    "}\n"_fs.outf(newline());
    "return *this;\n"_fs.outf(newline());
    sub_indent();
  "}\n"_fs.outf(newline());
}

void
Synthesizer::syn_class_def_struct_empty(
const Datatype& datatype, size_t i){
  auto& constructors = datatype.constructors();

  "struct _$ {\n"_fs.outf(newline(), constructors[i]);
  add_indent();
  //默认构造函数
  "_$() {}\n"_fs.outf(newline(), constructors[i]);
  //拷贝构造函数
  "_$(const _$& other){ }\n"_fs.outf(
    newline(), constructors[i], constructors[i]);
  //移动构造函数
  "_$(_$&& other) noexcept{ }\n"_fs.outf(
    newline(),constructors[i], constructors[i]);

  //重载小于运算符
  "bool operator<(const _$ &) const { return false; }\n"_fs.outf(
    newline(), constructors[i]);
  //重载相等运算符
  "bool operator==(const _$ &) const { return true; }\n"_fs.outf(
    newline(), constructors[i]);
  //拷贝赋值运算符
  "_$& operator=(const _$& other) { return *this; }\n"_fs.outf(
    newline(), constructors[i], constructors[i]);
  //移动赋值运算符
  "_$& operator=(_$&& other) noexcept { return *this; }\n"_fs.outf(
    newline(), constructors[i], constructors[i]);

  sub_indent();
  "};\n"_fs.outf(newline());
}
void
Synthesizer::syn_class_def_struct_members(
const Datatype &datatype, size_t i, string &lhs, string &rhs){

  auto& name = datatype.name();
  auto& self = datatype.self();
  auto& constructors = datatype.constructors();
  auto& components = datatype.components();

  // generate members
  for (size_t j = 0; j < components[i].size(); ++j) {
    if (j) {
      lhs += ", ";
      rhs += ", ";
    }

    if (components[i][j] == self) {
      "std::shared_ptr<$> p$_;\n"_fs.outf(
        newline(), components[i][j], j + 1);

      lhs.push_back('*');
      rhs.push_back('*');
    } else {
      "$ p$_;\n"_fs.outf(newline(), components[i][j], j + 1);
    }

    lhs += "p$_"_fs.format(j + 1);
    rhs += "rhs.p$_"_fs.format(j + 1);
  }
  out_.get() << endl;
}
void
Synthesizer::syn_class_def_struct_getMethod(
const Datatype& datatype, size_t i){
  auto& self = datatype.self();
  auto& components = datatype.components();

  // generate methods
  for (size_t j = 0; j < components[i].size(); ++j) {
    if (components[i][j] == self) {
      "$ p$() const { return *p$_; }\n"_fs.outf(
        newline(), components[i][j], j + 1, j + 1);
    } else {
      "const $ &p$() const { return p$_; }\n"_fs.outf(
        newline(), components[i][j], j + 1, j + 1);
    }
  }

  out_.get()<< endl;
}
void
Synthesizer::syn_class_def_struct_paramConstructor(const Datatype &datatype, size_t i){
  auto& name = datatype.name();
  auto& self = datatype.self();
  auto& constructors = datatype.constructors();
  auto& components = datatype.components();

  "_$("_fs.outf(newline(), constructors[i]);
    for(size_t j=0;j<components[i].size();++j){
      if(j< components[i].size()-1 ){
        "const $ &p$, "_fs.outf(out_.get(), components[i][j], j+1);
      }else{
        "const $ &p$ "_fs.outf(out_.get(), components[i][j], j+1);
      }
    }
  ")\n"_fs.outf(out_.get());
    add_indent();
    for(size_t j=0;j<components[i].size();++j){
      if(j==0){
        ":"_fs.outf(newline());
      }else{
        ","_fs.outf(newline());
      }

      if(components[i][j] == self){
        "p$_(std::make_shared<$>(p$))\n"_fs.outf(out_.get(), j+1, self, j+1);
      }else{
        "p$_(p$)\n"_fs.outf(out_.get(), j+1, j+1);
      }
    }
    sub_indent();
  "{}\n"_fs.outf(newline());
}
void
Synthesizer::syn_class_def_struct_copyConstructor(const Datatype &datatype, size_t i){
  auto& name = datatype.name();
  auto& self = datatype.self();
  auto& constructors = datatype.constructors();
  auto& components = datatype.components();

  "_$(const _$& other)\n"_fs.outf(newline(), constructors[i], constructors[i]);
    add_indent();
    for(size_t j=0;j<components[i].size();++j){
      if(j==0){
        ":"_fs.outf(newline());
      }else{
        ","_fs.outf(newline());
      }

      if(components[i][j] == self){
        "p$_(std::make_shared<$>(*other.p$_))\n"_fs.outf(out_.get(), j+1, self, j+1);
      }else{
        "p$_(other.p$_)\n"_fs.outf(out_.get(), j+1, j+1);
      }
    }
    sub_indent();
  "{}\n"_fs.outf(newline());
}
void
Synthesizer::syn_class_def_struct_moveConstructor(
const Datatype& datatype, size_t i){

  // auto& name = datatype.name();
  // auto& self = datatype.self();
  auto& constructors = datatype.constructors();
  auto& components = datatype.components();

  // 移动构造函数
  "_$(_$&& other) noexcept \n"_fs.outf(newline(), constructors[i], constructors[i]);
    add_indent();
    for(size_t j=0; j<components[i].size(); ++j){
      if(j==0){
        ":p$_(std::move(other.p$_))\n"_fs.outf(newline(), j+1, j+1);
      }else{
        ",p$_(std::move(other.p$_))\n"_fs.outf(newline(), j+1, j+1);
      }
    }
    sub_indent();
  "{ }\n"_fs.outf(newline());
}
void
Synthesizer::syn_class_def_struct_compareOperator(
const Datatype& datatype, size_t i, string lhs, string rhs){

  auto& name = datatype.name();
  auto& self = datatype.self();
  auto& constructors = datatype.constructors();
  auto& components = datatype.components();

  // generate operator<
  "bool operator<(const _$ &rhs) const {\n"_fs.outf(newline(), constructors[i]);
  add_indent();
  "return std::tie($) < std::tie($);\n"_fs.outf(newline(), lhs, rhs);
  sub_indent();
  "}\n"_fs.outf(newline());
}
void
Synthesizer::syn_class_def_struct_equivOperator(
const Datatype& datatype, size_t i, string lhs, string rhs){

  auto& name = datatype.name();
  auto& self = datatype.self();
  auto& constructors = datatype.constructors();
  auto& components = datatype.components();

  for(size_t j=0; j<constructors.size(); j++){
    if(i == j){
      "bool operator==(const _$ &rhs) const {\n"_fs.outf(newline(), constructors[i]);
      add_indent();
        "if(std::tie($) < std::tie($)){\n"_fs.outf(newline(), lhs, rhs);
        add_indent();
          "return false;\n"_fs.outf(newline());
        sub_indent();
        "}else{\n"_fs.outf(newline());
        add_indent();
          "return std::tie($) == std::tie($);\n"_fs.outf(newline(), lhs, rhs);
        sub_indent();
        "}\n"_fs.outf(newline());
      sub_indent();
      "}\n"_fs.outf(newline());
    }else{
      "bool operator==(const _$ &rhs) const {\n"_fs.outf(newline(), constructors[j]);
      add_indent();
        "return false;\n"_fs.outf(newline(), lhs, rhs);
      sub_indent();
      "}\n"_fs.outf(newline());
    }
  }
}
void
Synthesizer::syn_class_def_struct_copyOperator(
const Datatype& datatype, size_t i){

  // auto& name = datatype.name();
  auto& self = datatype.self();
  auto& constructors = datatype.constructors();
  auto& components = datatype.components();

  "_$& operator=(const _$& other){ \n"_fs.outf(
    newline(), constructors[i], constructors[i]);
    add_indent();
    "if(this != &other){ \n"_fs.outf(newline());
      add_indent();
      for(size_t j=0; j<components[i].size(); ++j){
        if(components[i][j] == self){
          "p$_.reset();\n"_fs.outf(newline(), j+1);
          "p$_ = std::make_shared<$>(*other.p$_);\n"_fs.outf(newline(), j+1, self, j+1);
        }else{
          "p$_ = other.p$_; \n"_fs.outf(newline(), j+1, j+1);
        }
      }
      sub_indent();
    "} \n"_fs.outf(newline());
    "return *this; \n"_fs.outf(newline());
    sub_indent();
  "} \n"_fs.outf(newline());
}
void
Synthesizer::syn_class_def_struct_moveOperator(
const Datatype& datatype, size_t i){

  auto& name = datatype.name();
  auto& self = datatype.self();
  auto& constructors = datatype.constructors();
  auto& components = datatype.components();

  // generate move operator=
  "_$& operator=(_$&& other) noexcept {\n"_fs.outf(newline(), constructors[i], constructors[i]);
    add_indent();
    "if(this != &other) {\n"_fs.outf(newline());
      add_indent();
      for(size_t j=0; j<components[i].size(); ++j){
        "p$_ = std::move(other.p$_);\n"_fs.outf(newline(), j+1, j+1);
      }
      sub_indent();
    "}\n"_fs.outf(newline());
    "return *this;\n"_fs.outf(newline());
    sub_indent();
  "}\n"_fs.outf(newline());
}



}