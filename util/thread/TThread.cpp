#include "TThread.h"

TThread::TThread() : thread(0), SMutex() {
}

TThread::~TThread() {}

void TThread::Create() {
    if (pthread_create(&thread, 0, TThread::thread_func, (void*) this) < 0 ) {
        std::cerr << "Can't create new thread!" << std::endl;
        throw ECreate();
    } 
}

void TThread::Join() {
    if (pthread_join(thread, 0) < 0) {
        std::cerr << "Thread can't join!" << std::endl;
        throw EJoin();
    }
}

void TThread::mutexLock() {
    SMutex->Lock();
}

void TThread::mutexUnlock() {
    SMutex->Unlock();
}
