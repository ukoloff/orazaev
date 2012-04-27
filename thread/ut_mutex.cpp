#include <pthread.h>
#include <iostream>
#include "TMutex.h"


static TMutex m;
void *run(void *attr) {
    int x = 0;
    
        m.Lock();
        std::cout << (int) pthread_self() << std::endl;
        std::cout << (int) pthread_self() << std::endl;
        std::cout << (int) pthread_self() << std::endl;
        std::cout << (int) pthread_self() << std::endl;
        std::cout << (int) pthread_self() << std::endl;
        std::cout << (int) pthread_self() << std::endl;
        x = pthread_self();
        x++;
        m.Unlock();
}

int main() {
    pthread_t pt[5];
    for (unsigned x = 0; x < 5; x++) {
        pthread_t p;
        pthread_create(pt + x, 0, run, 0);
    }
    
    for (int x = 0; x < 5; x++)
        pthread_join(pt[x], 0);

}
