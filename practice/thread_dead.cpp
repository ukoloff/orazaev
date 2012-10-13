#include <iostream>
#include <unistd.h>
#include <vector>

#include "../util/thread/thread.h"

class TBar : public TThread {
public:
    virtual void run() {
        for (int x = 0; x < 10; x++) {
            int x = 200;
            std::cout << x << " ";
            sleep(1);
            std::cout << x << " ";
        }
        std::cout << std::endl;
    }
};

class TFoo {
    //std::vector<TBar*> vb;
public:
    TFoo() {}

    void thread() {
        //vb.push_back(new TBar());
        //vb[0]->Create();
        TBar b;
        b.Create();
        return ;
    }

    void join() {
        //vb[0]->Join();
        sleep(20);
    }

    ~TFoo() {
        //for(std::vector<TBar*>::iterator t = vb.begin(); t != vb.end(); t++){
        //    delete *t;
        //}
    }
};


void make_thread() {
    TBar b;
    b.Create();
}
int main() {
    //TFoo foo;
    //foo.thread();
    //foo.join();

//    TBar b;
//    b.Create();

//    b.Join();

    make_thread();
    sleep(15);

    return 1;
}
