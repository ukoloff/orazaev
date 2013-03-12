/**
    @brief Thread messages description.

    Need for thread message queue. Each message
    type should have its handler or processor.

    Messages types:
        T_GET: crawler thread get command for
        download url. There are url link in string data.

        T_PARSE: thread parse command to parse links
        in page and add T_GET messages for this links
        in queue. There are page content in string data.

        T_POISON: thread terminate command.

        T_LOG: dump string data to log-dump file.
        One threaded, becouse work with out file.
        It means that that messages need one individual
        queue and one individual thread to dump pages.

    @author Aman Orazaev
*/
#pragma once

/** @brief Message type for TTaskMessage. */
enum TMessageType {T_GET, T_PARSE, T_POISON, T_LOG};

/**
    class TTaskMessage.
    @brief queued messages for std::threads.

    pData: url to get, or url content to parse.
    type: \in {T_GET, T_PARSE, T_POISON, T_LOG}, parse, get message
          or temenate thread.
*/
class TTaskMessage {
public:
    TTaskMessage(
        const std::shared_ptr<std::string>& pData,
        const TMessageType& type)
        : pData(pData)
        , type(type)
    { }


    const TMessageType& GetType() const {
        return type;
    }

    const std::shared_ptr<std::string>& GetData() const {
        return pData;
    }

private:
    const std::shared_ptr<std::string> pData;
    TMessageType type;
};
