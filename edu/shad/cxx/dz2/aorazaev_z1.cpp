/*
 *      C++ homework. Problem 1.
 *
 *      Due date: 25.09.12
 *
 *      Author: Aman Orazaev
 */
#include<iostream>
#include<string>
#include<stdio.h>

const std::string multilineCommentStart = "/*";
const std::string multilineCommentEnd = "*/";

const std::string lineComment = "//";

const char dquote = '"';
const char quote = '\'';
const char disable = '\\';
const char slash = '/';

enum StatesType {
    CODE,               // Block of code.
    DQUOTTED_STRING,    // "string"
    QUOTTED_CHAR,       // 'char'
    LINE_COMMENT,       // Line comment "// comment"
    MULTILINE_COMMENT   // Multiline comment "/* comment */"
};

void removeCommentsFromCxxProgram(
    std::istream & in,
    std::ostream & out) {

    StatesType state = CODE;
    bool disabled = false;

    std::string buffer = "$";

    char ch;
    while(in.get(ch)) {
        buffer.push_back(ch);
        if (buffer.size() > 2) {
            buffer.erase(buffer.begin());
        }
        if (disabled) {
            out << buffer[buffer.size() - 1];
            disabled = false;
            continue;
        }
        if (buffer[buffer.size() - 1] == disable) {
            out << buffer[buffer.size() - 1];
            disabled = true;
            continue;
        }
        switch(state) {
            case CODE:
                if (buffer[buffer.size() - 1] == dquote) {
                    state = DQUOTTED_STRING;
                    out << buffer[buffer.size() - 1];
                } else if (buffer[buffer.size() - 1] == quote) {
                    state = QUOTTED_CHAR;
                    out << buffer[buffer.size() - 1];
                } else if (buffer == lineComment) {
                    state = LINE_COMMENT;
                } else if (buffer == multilineCommentStart) {
                    state = MULTILINE_COMMENT;
                } else if (buffer[0] == slash) {
                    out << buffer;
                } else if (buffer[buffer.size() - 1] != slash) {
                    out << buffer[buffer.size() - 1];
                }
                break;

            case DQUOTTED_STRING:
                if (buffer[1] == dquote) {
                    state = CODE;
                }
                out << buffer[buffer.size() - 1];
                break;

            case QUOTTED_CHAR:
                if (buffer[1] == quote) {
                    state = CODE;
                }
                out << buffer[buffer.size() - 1];
                break;

            case LINE_COMMENT:
                if (buffer[buffer.size() - 1] == '\n') {
                    out << '\n';
                    state = CODE;
                }
                break;

            case MULTILINE_COMMENT:
                if (buffer == multilineCommentEnd) {
                    state = CODE;
                    buffer = "$";
                }
                break;
            default:
                throw 1;
        };
        /* std::cout << (int) state << std::endl; */
        /* std::cout << " " << (int) state << " " <<  buffer
                  << " " << disabled << std::endl; */
    }
}


int main() {
    removeCommentsFromCxxProgram(std::cin, std::cout);
    return 0;
}
