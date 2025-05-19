#include "synthesizer.hpp"
#include "../utility/config.hpp"
//for typeInfo
#include "../codegen/codegen.hpp"

using namespace std;

namespace hol2cpp{

void
Synthesizer::syn_func(const FuncEntity& func, bool is_impl)
{
  indent_ = 0;
  if (!func.template_args().empty()) {
    syn_func_template(func);
  }
  syn_func_definition(func, is_impl);
}

void
Synthesizer::syn_func_template(const FuncEntity& func)
{
  auto& template_args = func.template_args();
  "template<"_fs.outf(newline());
  for (size_t i = 0; i < template_args.size(); ++i) {
    if (i == 0) {
      "typename $"_fs.outf(out_.get(), template_args[i]);
    } else {
      ", typename $"_fs.outf(out_.get(), template_args[i]);
    }
  }
  ">\n"_fs.outf(out_.get());
}

void
Synthesizer::syn_func_definition(const FuncEntity& func, bool is_impl)
{
  auto params = syn_func_params(func);
  auto result_type = func.result_typeinfo().to_str(-1);

  "$ $($)"_fs.outf(newline(), result_type, func.name(), params);

  if (!is_impl) {
    ";\n\n"_fs.outf(out_.get());
    return;
  }

  " {\n"_fs.outf(out_.get());
  add_indent();
  {
    if(func.func_mem_mode() == 1){
      "auto impl = [&]() -> $ {\n"_fs.outf(newline(), result_type);
      add_indent();
    }

    syn_func_body(func);

    if(func.func_mem_mode() == 1){
      sub_indent();
      "};\n\n"_fs.outf(newline());

      auto param_types = syn_func_param_types(func);
      auto args = syn_func_args(func);

      "static std::map<std::tuple<$>, $> cache;\n"_fs.outf(
        newline(), param_types, result_type);
      "auto args = std::make_tuple($);\n"_fs.outf(newline(), args);
      "auto it = cache.find(args);\n"_fs.outf(newline());
      "return it != cache.end() ? it->second : (cache.emplace(std::move(args), impl()).first->second);\n"_fs
        .outf(newline());
    }
  }
  sub_indent();
  "}\n\n"_fs.outf(newline());
}

void
Synthesizer::syn_func_body(const FuncEntity& func)
{
  auto& statements = func.statements();
  for (size_t i = 0; i < statements.size(); ++i) {
    if (i) {
      "\n"_fs.outf(out_.get());
    }

    for (auto& statement : statements[i]) {
      if (!statement.empty()) {
        newline() << statement << endl;
      }
    }
  }
}

string
Synthesizer::syn_func_params(const FuncEntity& func)
{
  string params;
  auto& types = func.typeinfos();
  for (size_t i = 0; i < types.size() - 1; ++i) {
    if (i) {
      params += ", ";
    }
    //判断当前参数的类型是否是容器，此处只处理list
    if (types[i].name == theTemplateTypeMapping["list"]){
      if(theConfig.close_moveStd() == false){
        params += "$arg$"_fs.format(types[i].to_str_as_arg(0), i + 1);
      }else{
        //不使用移动，函数参数使用常量引用
        params += "$arg$"_fs.format(types[i].to_str_as_arg(1), i + 1);
      }
    }else{
      //根据函数的递归类型决定以什么样的方式进行代码生成优化
      params += "$arg$"_fs.format(types[i].to_str_as_arg(func.func_gen_mode()), i + 1);
    }
  }
  return params;
}

// for memorization
string
Synthesizer::syn_func_param_types(const FuncEntity& func)
{
  string param_types;
  auto& types = func.typeinfos();
  for (size_t i = 0; i < types.size() - 1; ++i) {
    if (i) {
      param_types += ", ";
    }
    param_types += "$"_fs.format(types[i].to_str(func.func_gen_mode()));
  }
  return param_types;
}
// for memorization
string
Synthesizer::syn_func_args(const FuncEntity& func)
{
  string args;
  auto& types = func.typeinfos();
  for (size_t i = 0; i < types.size() - 1; ++i) {
    if (i) {
      args += ", ";
    }
    args += "arg$"_fs.format(i + 1);
  }
  return args;
}



}