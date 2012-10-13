#include "TThread.h"

TThread::TThread() : thread(0), SMutex() {
}

TThread::~TThread() {}

void TThread::Create() {
    if (thread) {
        throw EThread("Thread is already created!");
    }
    if (pthread_create(&thread, 0, TThread::thread_func, (void*) this) != 0 ) {
        throw EThread("Can't create new thread!");
    } 
}

void TThread::Join() {
    if (pthread_join(thread, 0) != 0) {
        throw EThread("Thread can't join!");
    }
}

void TThread::Detach() {
    if (pthread_detach(thread) != 0) {
        throw EThread("Can't detach thread!");
    }
}

void TThread::mutexLock() {
    SMutex->Lock();
}

void TThread::mutexUnlock() {
    SMutex->Unlock();
}
