#pragma once

#include <tr1/memory>
#include <vector>
#include <string>
#include <iostream>
#include <stdexcept>


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


/* @brief Expression in parentheses.
 *
 * Example: (A+(B+C)+D)
 */
class InnerExpression : public Token {
public:
    InnerExpression()
    { }

    InnerExpression(const std::vector<TokenHolder>& subtokens)
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


/* @brief Main expression without parentheses.
 *
 * Example: A+(B+(C+D)+E)
 */
class StringExpression : public InnerExpression {
public:
    StringExpression()
    { }

    StringExpression(const std::vector<TokenHolder>& subtokens)
        : InnerExpression(subtokens)
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


StringExpression build_expression(const std::string& str);
