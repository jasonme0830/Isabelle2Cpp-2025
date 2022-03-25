#include "parser/parser.hpp"
#include "utility/argparse.hpp"
#include "optimizer/optimizer.hpp"
#include "synthesis/synthesizer.hpp"

#include <fstream>
#include <iostream>
#include <exception>

using namespace std;
using namespace hol2cpp;

struct Config {
    string input_file;
    string output_file;
};

ArgumentParser build_parser();
Config parse_config(int, char *[]);

int main(int argc, char* argv[]) {
    try {
        auto [input_file, output_file] = parse_config(argc, argv);

        ifstream fin(input_file);
        if (!fin.good()) {
            cout << "can't open file " << input_file << endl;
            return -1;
        }

        auto theory = Parser(fin, input_file).gen_theory();
        auto code = theory.gen_code();

        Synthesizer syner(output_file.empty() ? theory.name : output_file);
        syner.synthesize(code);

    } catch(const exception& e) {
        cerr << e.what() << endl;
    }

    return 0;
}

Config parse_config(int argc, char *argv[]) {
    auto arg_parser = build_parser();
    arg_parser.parse(argc, argv);

    Config config;

    // check the input theory file
    config.input_file = arg_parser.get<string>("input");
    auto pos = config.input_file.rfind(".thy");
    if (pos == config.input_file.npos) {
        throw std::invalid_argument("input file should be .thy file");
    }

    // determine the output file
    if (auto same_path = arg_parser.get<bool>("s")) {
        if (!arg_parser.get<string>("output").empty()) {
            throw std::invalid_argument("-s or assign output file");
        } else {
            config.output_file = config.input_file.substr(0, pos);
        }
    } else {
        config.output_file = arg_parser.get<string>("output");
    }

    // set optimizer options
    if (arg_parser.get<bool>("move-list")) {
        theOptimizer.enable_move_list();
    }
    if (arg_parser.get<bool>("reduce-cond")) {
        theOptimizer.enable_reduce_cond();
    }

    // set experimental options
    if (arg_parser.get<bool>("use-class")) {
        theOptimizer.enable_use_class();
    }
    if (arg_parser.get<bool>("uncurry")) {
        theOptimizer.enable_uncurry();
    }

    return config;
}

ArgumentParser build_parser() {
    ArgumentParser arg_parser("hol2cpp");

    arg_parser.add_argument("input")
              .help("source hol file")
    ;
    arg_parser.add_argument("output")
              .help("output hpp/cpp file")
              .default_value(""s)
    ;
    arg_parser.add_argument("-s")
              .help("choose the same path with input file")
              .default_value(false)
              .implict_value(true)
    ;

    arg_parser.add_argument("--move-list")
              .help("enable move-list")
              .default_value(false)
              .implict_value(true)
    ;
    arg_parser.add_argument("--reduce-cond")
              .help("enable reduce-cond")
              .default_value(false)
              .implict_value(true)
    ;

    arg_parser.add_argument("--use-class")
              .help("generating class instead struct for a datatype")
              .default_value(false)
              .implict_value(true)
    ;
    arg_parser.add_argument("--uncurry")
              .help("enable uncurrying")
              .default_value(false)
              .implict_value(true)
    ;

    return arg_parser;
}
