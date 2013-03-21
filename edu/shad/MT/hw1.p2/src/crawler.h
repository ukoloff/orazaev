/**
    @file crawler.h
    @brief TCrawler class description.

    PDC course home work 1. problem 2.
    Develop an multithreaded crawler.

    @author Aman Orazaev (aorazaev@gmai.com)
*/
#pragma once
#include <string>

/**
    @brief Wrapper around singlethreaded crawler configuration data.

    Fields.
      startUrl: zero level url to download.
      maxDepth: max level of graph depth to download.
      pathToDumpFile: path where dump file should be writed. (default current path)
      dumpFileName: dump file with add downloaded pages name. (default pages.dump)

    There no virtual destructor in class because we dont need polymorphism here.
*/
struct TSimpleCrawlerConfiguration {
    TSimpleCrawlerConfiguration(const std::string& url,
                          size_t maxDepth)
        : startUrl(url)
        , maxDepth(maxDepth)
        , pathToDumpFile(".")
        , dumpFileName("pages.dump")
    { }

    std::string startUrl;
    size_t maxDepth;
    std::string pathToDumpFile;
    std::string dumpFileName;
};

/**
    @brief Wrapper around multithreaded crawler configuration data.

    Fields.
      numberOfDownloaderThreads: number of threads who will do download.
      numberOfParserThreads: number of threads who will do parsing.
*/
struct TCrawlerConfiguration : public TSimpleCrawlerConfiguration {
    TCrawlerConfiguration(const std::string& url,
                          size_t maxDepth)
        : TSimpleCrawlerConfiguration(url, maxDepth)
        , numberOfDownloaderThreads(0)
        , numberOfParserThreads(0)
    {
        // FIXME(orazaev@): Calculation default number of threads.
        numberOfDownloaderThreads = 4;
        numberOfParserThreads = 1;
    }

    size_t numberOfDownloaderThreads;
    size_t numberOfParserThreads;
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

/**
    @brief Singlethreaded crawler.

    Need for timing.
*/
class TSimpleCrawler {
public:
    TSimpleCrawler(const std::string url,
             size_t maxDepth)
        : _config(url, maxDepth)
    { }

    TSimpleCrawler(const TSimpleCrawlerConfiguration& config)
        : _config(config)
    { }

    void Start();

private:
    TSimpleCrawlerConfiguration _config;
};
