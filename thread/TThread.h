/*
 *      POSIX thread (pthread) wrapper.
 *
 *      by AOrazaev
 *
 *      gmail me AOrazaev[at]gmail.com
 */
#ifndef TTHREAD_H
#define TTHREAD_H

#include <pthread>

class TThread {
    pthread_t thread;

    TThread(const TThread& copy);
    static void *thread_func(void *d) { ((TThread *) d)->run(); }
public:
    TThread() {}
    virtual ~TThread() {}

    virtual run() = 0;
    
    void Create();
    void Join();

    //exceptions
    class EThread {};
    class ECreate : public EThread {};
    class EJoin   : public EThread {};
};

#endif
