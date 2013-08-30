#include "string_expression.h"
#include <stack>
#include <algorithm>

namespace NStringExpression {
    namespace {
        bool is_ignored(char ch) {
            return ch == '+';
        }


        bool is_operator(char ch) {
            return ch == '(' || ch == ')' || ch == '+';
        }


        std::string strip_token(const TokenHolder& tok) {
            std::string result = tok->str();
            std::string::const_iterator sit = std::remove_if(result.begin(),
                                                             result.end(),
                                                             is_operator);
            result.resize(sit - result.begin());
            return result;
        }


        bool token_comparison(const TokenHolder& lhs, const TokenHolder& rhs) {
            return strip_token(lhs) < strip_token(rhs);
        }


        void sort_token(TokenHolder& tok) {
            tok->sort();
        }
    }


    std::string InnerExpression::str() const {
        return "(" + subtokens_str() + ")";
    }


    std::string StringExpression::subtokens_str() const {
        std::string result;

        for (std::vector<TokenHolder>::const_iterator cit = subtokens.begin();
             cit != subtokens.end();
             ++cit)
        {
            result += (**cit).str();
            if (cit + 1 != subtokens.end()) {
                result += '+';
            }
        }

        return result;
    }


    void StringExpression::sort() {
        std::for_each(subtokens.begin(), subtokens.end(), sort_token);
        std::sort(subtokens.begin(), subtokens.end(), token_comparison);
    }


    std::string StringExpression::str() const {
        return subtokens_str();
    }


    StringExpression build_expression(const std::string& str) {
        StringExpression main;
        std::stack<Token*> parentheses_level_stack;
        parentheses_level_stack.push(&main);

        for (std::string::const_iterator cit = str.begin();
             cit != str.end();
             ++cit)
        {
            if (is_ignored(*cit)) {
                continue;
            }

            if (*cit == ')') {
                if (parentheses_level_stack.size() == 1) {
                    throw std::invalid_argument("Parentheses are non balanced.");
                }
                parentheses_level_stack.pop();
                continue;
            }

            if (*cit == '(') {
                TokenHolder expr(new InnerExpression());
                parentheses_level_stack.top()->append(expr);
                parentheses_level_stack.push(expr.get());
                continue;
            }

            parentheses_level_stack.top()->append(TokenHolder(new Character(*cit)));
        }

        if (parentheses_level_stack.size() != 1) {
            throw std::invalid_argument("Parentheses are non balanced.");
        }

        return main;
    }


    std::string sort_string(const std::string& str) {
        StringExpression an_expression = build_expression(str);
        an_expression.sort();
        return an_expression.str();
    }
} // NStringExpression
