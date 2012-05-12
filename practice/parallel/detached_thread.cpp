#include <pthread.h>
#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <string>
#include <cstdlib>


void* run(void* v) {
    for(int x = 0; x < 10; x++) {
        printf("I'm thread\n");
        sleep(1);
    }
    
    return 0;
}

void perr(std::string s) {
    std::cerr << s << std::endl;
    exit(1);
}


class TFoo {
public:
    void createThread() {
        pthread_t pt;
        if (pthread_create(&pt, 0, run, 0) != 0)
            perr("Create problem!");
        
        if (pthread_detach(pt) != 0) 
            perr("Detach problem!");
    }
};

int main() {
    /*
    pthread_t pt;
    
    if (pthread_create(&pt, 0, run, 0) != 0)
        perr("Create problem!");

    if (pthread_detach(pt) != 0)
        perr("Detach problem!");
    */

    TFoo f;
    
    f.createThread();
    sleep(20);
    return 0;
}
