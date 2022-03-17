#include "parser/parser.hpp"
#include "utility/argparse.hpp"
#include "synthesis/synthesizer.hpp"

#include <fstream>
#include <iostream>
#include <exception>

using namespace std;
using namespace hol2cpp;

int main(int argc, char* argv[]) {
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
              .implict_value(true);

    try {
        arg_parser.parse(argc, argv);

        auto input_file = arg_parser.get<string>("input");
        auto pos = input_file.rfind(".thy");
        if (pos == input_file.npos) {
            throw std::invalid_argument("input file should be .thy file");
        }

        string output_file;
        if (auto same_path = arg_parser.get<bool>("s")) {
            if (!arg_parser.get<string>("output").empty()) {
                throw std::invalid_argument("-s or assign output file");
            } else {
                output_file = input_file.substr(0, pos);
            }
        } else {
            output_file = arg_parser.get<string>("output");
        }

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
