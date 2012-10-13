#include "../../util/thread/thread.h"
#include <stdlib.h>

class TFoo : public TThread {
public:
    TFoo() {} 
    
    virtual void run() {
        int x = rand() % 100 + 1;
        mutexLock();
        std::cout << x << std::endl;
        mutexUnlock();
    }    

    virtual bool isDone() {
        return 1;
    }
};

int main() {
    TFoo foo[20];

    for (int x = 0; x < 20; x++)
        foo[x].Create();


    for (int x = 0; x < 20; x++)
        foo[x].Join();

    return 0;
}
