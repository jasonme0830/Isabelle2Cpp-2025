#include <fstream>
#include <iostream>
#include <exception>
#include "code.hpp"
#include "parser.hpp"
#include "argparse.hpp"

using namespace std;
using namespace hol2cpp;

int main(int argc, char* argv[])
{
    ArgumentParser arg_parser("hol2cpp");
    arg_parser.add_argument("file")
              .help("source hol file");

    try
    {
        arg_parser.parse(argc, argv);

        auto path = arg_parser.get<string>("file");
        auto fin = ifstream(path);
        if (!fin.good())
        {
            cout << "can't open file " << path << endl;
            return 0;
        }

        string source;
        while (!fin.eof())
        {
            source += fin.get();
        }

        auto decls = Parser().pas_func_decls(source);
        Code code;
        for (auto &decl : decls)
        {
            code.entry_func();
            decl->build_entity(code.current_entity());
        }
        code.generate();
    }
    catch(const exception& e)
    {
      cerr << e.what() << endl;
    }

    return 0;
}
