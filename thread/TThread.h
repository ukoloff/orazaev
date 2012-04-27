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
#include "../util/patterns/TSingleton.h"
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
    
    void Create();
    void Join();

    void mutexLock();
    void mutexUnlock();

    //exceptions
    class EThread : public TError {};
    class ECreate : public EThread {};
    class EJoin   : public EThread {};
};

#endif
