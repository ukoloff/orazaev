/**
    @brief TThreadWorker.

    @author Aman Orazaev
*/
#pragma once

#include <messages.h>


/**
    @brief run funcion for std::thread.
    Process messages until get poisoned message.
*/
class TThreadWorker {
public:
    TThreadWorker(const TWorkerEnvironment& env)
        : env_(env) { }

    void Run();
private:
    TWorkerEnvironment env_;
};


/**
    @brief wrapper for std::thread constructor.
*/
void StartWorker(const TWorkerEnvironment& env);

