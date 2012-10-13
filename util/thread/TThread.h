/*
 *      TThread class.
 *
 *      by AOrazaev
 *
 *      gmail me AOrazaev[at]gmail.com
 */
#ifndef TTHREAD_H
#define TTHREAD_H

#include <pthread.h>
#include "../patterns/TSingleton.h"
#include "TMutex.h"

class TThread {
    pthread_t thread;
    TSingleton<TMutex> SMutex;

    TThread(const TThread& copy);
    static void *thread_func(void *d) {  ((TThread *) d)->run(); }

public:
    TThread();
    virtual ~TThread();

    virtual void run() = 0;
//    virtual bool isDone() = 0;
    
    void Create();
    void Join();
    void Detach();

    void mutexLock();
    void mutexUnlock();

    class EThread : public TError {
    public:
        EThread(const std::string & m) throw()
            : TError(m)
        { }
    };
    
    static void Cancel(const TThread& th) {
        if (pthread_cancel(th.thread) != 0)
            throw EThread("Can't cancel thread");
    }

};

#endif
