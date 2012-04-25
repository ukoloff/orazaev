#include "TThread.h"

void TTHread::Create() {
    if (pthread_create(&thread, 0, Thread::thread_func, (void*) this) < 0 ) {
        std::cerr << "Can't create new thread!" << std::ednl;
        throw ECreate();
    } 
}

void TThread::Join() {
    if (pthread_join(thread, 0) < 0) {
        std::cerr << "Thread can't wait!" << std::endl;
        throw EJoin();
    }
}
