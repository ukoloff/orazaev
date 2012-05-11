#include "cmc_thread_inspector.h"

//TThreadOperator description
TThreadOperator::TThreadOperator() {

}

//TThreadInspector description
template <class MSG, ANS>
TThreadInspector::TThreadInspector(TSocket Connection, std::map<MSG, ANS> * Pdata) 
    : connection(Connection)
    , pdata(Pdata) {
    inspect();
}

template <class MSG, ANS>
void TThreadInspector::inspect() {
    class TSubThread {
    public:
        virtual void run() {
            while (1) {
                for (std::list<TThread *>::iterator it = threadList.begin(); it!= threadList.end(); it++)
                    if (it->isDone()) {
                        threadList.erase(it);
                        break;
                    }
            }
        }
    };

    TSubThread st;
    st->Create();
    st->Detach();
}
