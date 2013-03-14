/**
    @brief TThreadWorker.
    
    Worker thread class and its run function.
    Run function is more comfortable becouse need
    past std::ref to std::thread constructor for
    TThreadWorker class.
    
    Example:
    <code>
    TThreadGuard guard(std::thread(StartWorker, env));
    </code>

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
    TThreadWorker(const TWorkerEnvironment&);
    void Run();
private:
    TWorkerEnvironment env_;
};


/**
    @brief wrapper for std::thread constructor.
*/
void StartWorker(const TWorkerEnvironment& env);

