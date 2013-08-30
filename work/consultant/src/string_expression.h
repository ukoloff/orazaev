/* @brief Data declarations for string expressions problem.
 *
 * Grammar
 * -------
 * StringExpression is one of:
 *      empty
 *      Expression + StringExpression
 *      Character + StringExpression
 *
 * InnerExpression is one of:
 *      (StringExpression)
 *
 * Character is one stand alone character.
 *
 */
#pragma once

#include <tr1/memory>
#include <vector>
#include <string>
#include <iostream>
#include <stdexcept>


namespace NStringExpression {
    class Token;
    typedef std::tr1::shared_ptr<Token> TokenHolder;


    /* @brief Abstract token class.
     */
    class Token {
    public:
        virtual ~Token()
        { }

        virtual std::string str() const = 0;
        virtual void append(const TokenHolder&) = 0;
        virtual void sort() = 0;
    };


    /* @brief Main expression without parentheses.
     *
     * Example: A+(B+(C+D)+E)
     */
    class StringExpression : public Token {
    public:
        StringExpression()
        { }

        StringExpression(const std::vector<TokenHolder>& subtokens)
            : Token()
            , subtokens(subtokens)
        { }

        void append(const TokenHolder& token) {
            subtokens.push_back(token);
        }

        std::string str() const;
        void sort();

    protected:
        std::string subtokens_str() const;

    private:
        std::vector<TokenHolder> subtokens;
    };


    /* @brief Expression in parentheses.
     *
     * Example: (A+(B+C)+D)
     */
    class InnerExpression : public StringExpression {
    public:
        InnerExpression()
        { }

        InnerExpression(const std::vector<TokenHolder>& subtokens)
            : StringExpression(subtokens)
        { }

        std::string str() const;
    };


    /* @brief One character.
     *
     * Example: A
     */
    class Character : public Token {
    public:
        Character(char ch)
            : Token()
            , ch(ch)
        { }

        std::string str() const {
            return std::string(1, ch);
        }

        void append(const TokenHolder&) {
            throw std::logic_error("You cannot append any kind of tokens to Character.");
        }

        void sort()
        { }

    private:
        char ch;
    };


    /* @brief Build expression by given string.
     */
    StringExpression build_expression(const std::string& str);


    /* @brief Sorts given string as string expression.
     *
     * It is the A function, what is defined in task description.
     */
    std::string sort_string(const std::string& str);
}
