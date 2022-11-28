#include "inference/inference.hpp"
#include "parser/parser.hpp"
#include "synthesis/synthesizer.hpp"
#include "utility/argparse.hpp"
#include "utility/config.hpp"

#include <exception>
#include <fstream>
#include <iostream>

using namespace std;
using namespace hol2cpp;

struct Config
{
  string input_file;
  string output_file;
  string predef_file;
  bool print_type = false;
};

ArgumentParser
build_parser();

// affect the global config
void
parse_config(int, char*[]);

int
main(int argc, char* argv[])
{
  parse_config(argc, argv);

  ifstream fin(theConfig.input_file());
  if (!fin.good()) {
    cout << "can't open file " << theConfig.input_file() << endl;
    return -1;
  }

  Theory predef;
  if (!theConfig.predef_file().empty()) {
    ifstream fin(theConfig.predef_file());
    if (!fin.good()) {
      cout << "can't open file " << theConfig.predef_file() << endl;
      return -1;
    }

    Parser parser(fin, theConfig.predef_file());
    predef = parser.gen_predef();
  }

  auto theory = Parser(fin, theConfig.input_file()).gen_theory(move(predef));

  auto inf = TypeInference(theory);
  inf.theory_infer();
  if (theConfig.print_type()) {
    inf.print_theory();
  }

  auto code = theory.gen_code();

  Synthesizer syner(theConfig.output_file().empty() ? theory.name
                                                    : theConfig.output_file());
  syner.synthesize(code);

  return 0;
}

void
parse_config(int argc, char* argv[])
{
  auto arg_parser = build_parser();
  arg_parser.parse(argc, argv);

  if (arg_parser.get<bool>("help")) {
    arg_parser.print_help();
    exit(0);
  }

  // check the input theory file
  auto input_file = arg_parser.get<string>("input");
  auto pos = input_file.rfind(".thy");
  if (pos == input_file.npos) {
    throw std::invalid_argument("input file should be .thy file");
  }

  // determine the output file
  auto output_file = arg_parser.get<string>("output");
  if (auto same_path = arg_parser.get<bool>("s")) {
    if (!arg_parser.get<string>("output").empty()) {
      throw std::invalid_argument("-s or assign output file");
    } else {
      output_file = input_file.substr(0, pos);
    }
  }

  // common parameters
  theConfig.input_file(move(input_file));
  theConfig.output_file(move(output_file));
  theConfig.predef_file(arg_parser.get<string>("predef"));
  theConfig.print_type(arg_parser.get<bool>("print-type"));

  // set optimizer options
  theConfig.move_list(arg_parser.get<bool>("move-list"));
  theConfig.reduce_cond(arg_parser.get<bool>("reduce-cond"));
  theConfig.use_deque(arg_parser.get<bool>("use-deque"));

  // set experimental options
  theConfig.use_class(arg_parser.get<bool>("use-class"));
  theConfig.uncurry(arg_parser.get<bool>("uncurry"));
}

ArgumentParser
build_parser()
{
  ArgumentParser arg_parser("hol2cpp");

  arg_parser.add_argument("--help")
    .help("print help message")
    .default_value(false)
    .implict_value(true);

  arg_parser.add_argument("input").help("source hol file");
  arg_parser.add_argument("output")
    .help("output hpp/cpp file without extension")
    .default_value(""s);
  arg_parser.add_argument("-s")
    .help("make the output file same with input file without extension")
    .default_value(false)
    .implict_value(true);
  arg_parser.add_argument("--predef")
    .help("file contains predefined types")
    .default_value(""s);
  arg_parser.add_argument("--print-type")
    .help("enable type print for function definitions")
    .default_value(false)
    .implict_value(true);

  arg_parser.add_argument("--move-list")
    .help("enable move-list")
    .default_value(false)
    .implict_value(true);
  arg_parser.add_argument("--reduce-cond")
    .help("enable reduce-cond")
    .default_value(false)
    .implict_value(true);
  arg_parser.add_argument("--use-deque")
    .help("enable use-deque")
    .default_value(false)
    .implict_value(true);

  arg_parser.add_argument("--use-class")
    .help("generate class instead struct for a datatype")
    .default_value(false)
    .implict_value(true);
  arg_parser.add_argument("--uncurry")
    .help("enable uncurrying")
    .default_value(false)
    .implict_value(true);

  return arg_parser;
}
