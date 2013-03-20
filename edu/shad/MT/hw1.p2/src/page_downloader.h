#pragma once

#include <curl/curl.h>
#include <string>

/**
    class TPageDownloader.
    @brief download pages from web. use libcurl.
*/
class TPageDownloader {
public:
    TPageDownloader()
        : numberOfDownloadedPages(0)
        , sizeOfDownloadedData(0)
        , totalDownloadingTime(0.)
    { }

    /** GET url -> std::string. using curl.*/
    std::string GetUrl(
        const std::string& url,
        long timeout) throw();

    /** Use default timeout: GetUrl(url, DEFAULT_TIMEOUT); */
    std::string GetUrl(const std::string& url) throw();

    /** Check is url downloadable. */
    bool CheckUrl(const std::string& url) const throw();


    double GetTotalTime() const throw();

    /** Summary functions for current downloader */
    inline size_t GetNumbeOfPages() const { return numberOfDownloadedPages; }
    inline unsigned long long GetSizeOfData() const { return sizeOfDownloadedData; }

private:
    static const long DEFAULT_TIMEOUT = 30;

private:
    static size_t string_write(
        void *contents,
        size_t size,
        size_t nmemb,
        void *userp);

    static size_t null_write(
        void *contents,
        size_t size,
        size_t nmemb,
        void *userp);

    size_t numberOfDownloadedPages;
    unsigned long long sizeOfDownloadedData;

    double totalDownloadingTime;
};

