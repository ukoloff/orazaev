#include <iostream>
#include <list>

#include "../../util/thread/thread.h"

class TFoo {
    std::list<int> pdata;

    void inspect() {
        TFoo *parent = this;
        class TBar : public TThread {
        public:
            virtual void run() {
                while (1) {
                    for(std::list<int>::iterator x = parent->pdata.begin(); x != parent->pdata.end(); x++ )
                        if ( *x == 0 ) {
                            parent->pdata.erase(x);
                            break;
                        }
                }
            }
        };

        TBar tb;
        tb.Create();
        tb.Detach();
    }

public:
    TFoo() {
        inspect();
    }

    void create(const int& x) {
        pdata.push_back(x);
    }

    int count() {
        return pdata.size();
    }
};

int main() {
    TFoo tf;
    
    while (1) {
        int r;
        std::cin >> r;
        tf.create(r);
        
        std::cout << tf.count() << std::endl;
    }
}
