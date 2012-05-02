#include <pthread.h>
#include <iostream>
#include <cstdlib>

void *run(void *args) {
    std::cout << "Hello, I am " << pthread_self() << std::endl;
    sleep(10);
    //return 0;
}

int main() {
    std::cout << pthread_self() << std::endl;

    pthread_t thread;
    pthread_create(&thread, 0, run, 0);

    pthread_join(thread, 0);
    std::cout << "pthread_exit() was!" << std::endl;
    return 0;
}
