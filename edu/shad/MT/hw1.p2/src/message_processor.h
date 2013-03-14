/**
    @brief Message handlers description.

    class TMsgProcessor.
    Also has TMsgProcessor class. Thread should call:
        TMsgProcessor::Process(msg, env);
    to process msg. TMsgProcessor will call message
    type handler.


    If you add new message type, you need define
    it's handler class also and add it to TMsgProcessor.
    
    @author Aman Orazaev
*/ 
#pragma once

#include <messages.h> 
#include <link_parser.h>
 

 
/** @brief Abstract message handler. */
class TMessageHandler {
protected:
    TMessageHandler(TWorkerEnvironment& env)
        : env_(env)
    { }
    ~TMessageHandler() { }

protected:
    void Process(TTaskMessage&);

protected:
    TWorkerEnvironment& env_;
};



/** @brief T_GET message handler. */
class TGetMessageHandler : public TMessageHandler {
public:
    TGetMessageHandler(TWorkerEnvironment& env)
        : TMessageHandler(env)
    { }

    void Process(const TTaskMessage& msg);
};



/** @brief T_PARSE message handler. */
class TParseMessageHandler : public TMessageHandler {
public:
    TParseMessageHandler(TWorkerEnvironment& env)
        : TMessageHandler(env)
    { }

    void Process(const TTaskMessage& msg);
};



/** @brief T_POISON message handler. */
class TPoisonMessageHandler : public TMessageHandler {
public:
    TPoisonMessageHandler(TWorkerEnvironment& env)
        : TMessageHandler(env)
    { }

    void Process(const TTaskMessage&);
};



/** @brief T_LOG message handler. */
class TLogMessageHandler : public TMessageHandler {
public:
    TLogMessageHandler(TWorkerEnvironment& env)
        : TMessageHandler(env)
    { }

    void Process(const TTaskMessage& msg);
};



/** @brief Message handlers trigger. */
class TMsgProcessor {
public:
    static void Process(
            const TTaskMessage& msg,
            TWorkerEnvironment& env);

private:
    TMsgProcessor() = delete;
    ~TMsgProcessor() = delete;
};
