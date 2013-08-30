/* @brief read string expressions from given file and write sorted expressions
 *        to another given file.
 */
#include <fstream>
#include <iostream>
#include <cstdlib>
#include "string_expression.h"

std::string usage() {
    return "app2 <input_file>";
}


void app2(const char * const input_file)
{
    std::fstream input(input_file, std::fstream::in);
    if (!input.is_open()) {
        std::cerr << "Can't open file '" << input_file << "' with read mode.\n";
        exit(1);
    }

    std::string output_file(input_file);
    output_file += "_sorted";
    std::fstream output(output_file.c_str(), std::fstream::out);
    if (!output.is_open()) {
        std::cerr << "Can't open file '" << output_file << "' with write mode.\n";
        exit(1);
    }

    std::string expr;
    while (input >> expr) {
        output << sort_string(expr) << "\n";
    }

    output.close();
    input.close();
}


int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << usage() << std::endl;
        exit(1);
    }

    app2(argv[1]);
    return 0;
}
