#ifndef CMC_THREAD_INSPECOTR
#define CMC_THREAD_INSPECOTR

#include "../util/thread/TTHread.h"
#include "../util/socket/TSocket.h"

#include <map>
#include <list>

template <class MSG, class ANS>
class TThreadOperator : public TThread {
    TSocket connection;
    std::map<MSG, ANS>* pdata

    bool done;
    static int deadline;

    TThreadOperator();
public:
    static void setDeadline(int Deadline);

    TThreadOperator(TSocket Connection, map<MSG, ANS>* Pdata);
    ~TThreadOperator();

    virtual void run();
    virtual bool isDone() throw();


}:

template <class MSG, class ANS>
class TThreadInspector {
    std::list<TThread *> threadList;

    void inspect();
public:
    TThreadInspector();

    void create(TSocket connection);
};

template <class MSG, class ANS>
TThreadOperator<MSG, ANS>::deadline = 0;

template <class MSG, class ANS>
void TThreadOperator<MSG, ANS>::setDeadline(int Deadline) {
    TThreadOperator<MSG, ANS>::deadline = Deadline;
}

#endif
