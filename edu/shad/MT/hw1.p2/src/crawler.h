/**
    @file crawler.h
    @brief TCrawler class description.

    PDC course home work 1. problem 2.
    Develop multithreaded crawler.

    @author Aman Orazaev (aorazaev@gmai.com)
*/
#pragma once
#include <string>

/**
    @brief Wrapper around crawler configuration data.

    Fiels.
    startUrl: zero level url to download.
    maxDepth: max level of graph depth to download.
    numberOfDownloaderThreads: number of threads who will do download.
    numberOfParserThreads: number of threads who will do parsing.
    pathToDumpFile: path where dump file should be writed. (default current path)
    dumpFileName: dump file with add downloaded pages name. (default pages.dump)
*/
struct TCrawlerConfiguration {
    TCrawlerConfiguration(const std::string& url,
                          size_t maxDepth)
        : startUrl(url)
        , maxDepth(maxDepth)
        , numberOfDownloaderThreads(0)
        , numberOfParserThreads(0)
        , pathToDumpFile(".")
        , dumpFileName("pages.dump")
    {
        // FIXME(orazaev@): Calculation default number of threads.
        numberOfDownloaderThreads = 4;
        numberOfParserThreads = 1;
    }

    std::string startUrl;
    size_t maxDepth;
    size_t numberOfDownloaderThreads;
    size_t numberOfParserThreads;
    std::string pathToDumpFile;
    std::string dumpFileName;
};

/**
    @brief Multithreaded crawler.

    You can pass url and maxDepth to the constructor for
    automatically detect number of threads(parser and downloaders).
    Or you can manualy configure TCrawlerConfiguration and pass it
    to constructor.

    TCrawler may have several downloader threads and several parser
    threads, but only one main thread writes downloaded pages on disk.
*/
class TCrawler {
public:
    TCrawler(const std::string url,
             size_t maxDepth)
        : _config(url, maxDepth)
    { }

    TCrawler(const TCrawlerConfiguration& config)
        : _config(config)
    { }

    /** @brief Start crawling. */
    void Start();

private:
    TCrawlerConfiguration _config;
};
