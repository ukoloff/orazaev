#include<iostream>
#include<stack>
#include<string>

const char OPENBRACE[3] = { '(', '[', '{' };
const char CLOSEBRACE[3] = { ')', ']', '}' };
const size_t BRACECOUNT = 3;

size_t openBraceNo(char c) {
    size_t ans = -1;
    for(size_t i = 0; i < BRACECOUNT; ++i) {
        if (c == OPENBRACE[i]) {
            ans = i;
            break;
        }
    }

    return ans;
}

bool checkBraceSequence(const std::string & seq) {
    bool ans = true;
    std::stack<char> s;

    for(std::string::const_iterator it = seq.begin();
        it != seq.end(); ++it) {
        size_t braceNo = openBraceNo(*it);
        if (braceNo != -1) {
            s.push(CLOSEBRACE[braceNo]);
        } else {
            if (s.top() != *it) {
                ans = false;
                break;
            }
            s.pop();
        }
    }

    if (ans == true && !s.empty()) {
        ans = false;
    }
    return ans;
}

int main() {
    std::cout << (int) checkBraceSequence("") << std::endl;
    std::cout << (int) checkBraceSequence("()") << std::endl;
    std::cout << (int) checkBraceSequence("{}") << std::endl;
    std::cout << (int) checkBraceSequence("[]") << std::endl;
    std::cout << (int) checkBraceSequence("{()[()][]{}}") << std::endl;
    std::cout << (int) checkBraceSequence("(}") << std::endl;
    std::cout << (int) checkBraceSequence("{()()()()]") << std::endl;
    std::cout << (int) checkBraceSequence("((((((((") << std::endl;

    return 0;
}
