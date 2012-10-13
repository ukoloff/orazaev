#include "../TThread.h"
#include <iostream>
#include <sstream>

static std::stringstream ss;

int main() {
    class TThread_a : public TThread {
        int a;
    public:
        TThread_a(int x) : TThread(), a(x) {}
        virtual void run() {
            mutexLock();
                ss << a << " ";
                ss << a << " ";
                ss << a << " ";
                ss << a << " ";
                ss << a << " ";
                ss << std::endl;
            mutexUnlock();
        }
    };

try {
    const unsigned t_count = 200;
    TThread_a* t[t_count];
    for (int x = 0; x < t_count; x++)
        t[x] = new TThread_a(x);

    for (int x = 0; x < t_count; x++) {
        t[x]->Create();
    }
    for (int x = 0; x < t_count; x++)
        t[x]->Join();

    for (int x = 0; x < t_count; x++)
        delete t[x];

    int e_count = 5;
    size_t tcount = 0;
    std::string past;
    while (ss.good()) {
        std::string s;
        ss >> s;
        if (past != s) {
            if (e_count == 5) {
                past = s;
                e_count = 0;
                ++tcount;
            } else {
                std::cout << "FAILED" << std::endl;
                return 1;
            }
        } 
        e_count++; 
    }
    --tcount;
    std::cout << t_count << " threads was created." << std::endl;
    std::cout << tcount << " threads was executed." << std::endl;
    std::cout << "OK" << std::endl;
}
catch (TError){
    std::cout << "TError was catched" << std::endl;
}
}
