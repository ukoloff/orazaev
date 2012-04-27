#ifndef TMUTEX_H
#define TMUTEX_H

#include<pthread.h>

#include"../util/TError.h"

class TMutex {
    pthread_mutex_t mut;
public:
    TMutex();
    ~TMutex();

    void Lock();
    void Unlock();

    //exceptions
    class EMutex   : public TError {};
    class EInit    : public EMutex {};
    class ELock    : public EMutex {};
    class EUnlock  : public EMutex {};
    class EDestroy : public EMutex {};
};

#endif
