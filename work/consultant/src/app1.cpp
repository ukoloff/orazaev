/* @brief Write N random string expressions to given file.
 *
 * Realization of test problem 1.
 */
#include <fstream>
#include <cstdlib>
#include <ctime>

#include "string_expression.h"


using namespace NStringExpression;


std::string usage() {
    return "Usage: app1 <N> <output_file_name>";
}


bool coinflip(double probability=0.5) {
    return rand() % 1000 < 1000 * probability;
}


std::vector<TokenHolder> generate_tokens(int max_tokens=50,
                                         int min_tokens=1,
                                         double expression_probability=0.3)
{
    int number_of_tokens = rand() % (max_tokens - min_tokens) + min_tokens;
    std::vector<TokenHolder> tokens;
    while (number_of_tokens) {
        if (!coinflip(expression_probability)) {
            char random_char = rand() % 26 + 'A';
            tokens.push_back(TokenHolder(new Character(random_char)));
            --number_of_tokens;
            continue;
        }

        int number_of_tokens_in_subexpr = number_of_tokens - rand() % number_of_tokens;
        TokenHolder subexpr(new InnerExpression(generate_tokens(number_of_tokens_in_subexpr + 1,
                                                           number_of_tokens_in_subexpr,
                                                           expression_probability)));
        tokens.push_back(subexpr);
        number_of_tokens -= number_of_tokens_in_subexpr;
    }

    return tokens;
}


/* @brief generate N random string expressions.
 */
std::vector<std::string> generate(int n) {
    std::vector<std::string> result;
    for (int i = 0; i < n; ++i) {
        StringExpression expr(generate_tokens());
        result.push_back(expr.str());
    }

    return result;
}


void app1(int n, const char * const file_name) {
    std::vector<std::string> exprs = generate(n);

    std::fstream output_file(file_name, std::fstream::out);
    if (!output_file.is_open()) {
        std::cerr << "Can't open file '" << file_name << "' with read mode.\n";
    }

    for (std::vector<std::string>::const_iterator cit = exprs.begin();
         cit != exprs.end();
         ++cit)
    {
        output_file << *cit << std::endl;
    }

    output_file.close();
}


int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << usage() << std::endl;
        exit(1);
    }

    std::srand(time(0));

    app1(atoi(argv[1]), argv[2]);
    return 0;
}
