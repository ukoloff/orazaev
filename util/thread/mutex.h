#ifndef TMUTEX_H
#define TMUTEX_H

#include<pthread.h>

#include"../error.h"

class TMutex {
    pthread_mutex_t mut;
public:
    TMutex();
    ~TMutex();

    void Lock();
    void Unlock();

    //exceptions
    class EMutex   : public TError {
    public:
        EMutex(const std::string & m)
            : TError(m) 
        { }
    };
};

#endif
