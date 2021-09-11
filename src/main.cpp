#include "code.hpp"
#include "parser.hpp"
#include "argparse.hpp"

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

    try {
        arg_parser.parse(argc, argv);

        auto input_file = arg_parser.get<string>("input");
        ifstream fin(input_file);
        if (!fin.good()) {
            cout << "can't open file " << input_file << endl;
            return -1;
        }

        auto theory = Parser(fin).gen_theory();
        auto output_file = arg_parser.get<string>("output");

        Code code(output_file.empty() ? theory.name : output_file);
        theory.codegen(code);
        code.generate();

    } catch(const exception& e) {
        cerr << e.what() << endl;
    }

    return 0;
}
