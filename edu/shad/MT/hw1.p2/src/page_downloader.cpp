#include "page_downloader.h"


size_t TPageDownloader::string_write(
    void *contents,
    size_t size,
    size_t nmemb,
    void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

size_t TPageDownloader::null_write(
    void *contents,
    size_t size,
    size_t nmemb,
    void *userp)
{
    return size * nmemb;
}

std::string TPageDownloader::GetUrl(
    const std::string& url,
    long timeout) throw()
{
    std::string buffer;
    CURLcode code(CURLE_FAILED_INIT);
    CURL* curl = curl_easy_init();

    if (curl) {
        if(CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_URL, url.c_str()))
        && CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, string_write))
        && CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer))
        && CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L))
        && CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_WILDCARDMATCH, 0))
        && CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L))
        && CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L))
        && CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_HEADER, 1L))
        && CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout))) {
            code = curl_easy_perform(curl);
        }
        curl_easy_cleanup(curl);
    }

    if (code != CURLE_OK) {
        buffer = "DOWNLOADING ERROR: " +
                std::string(curl_easy_strerror(code));
    } else {
        ++numberOfDownloadedPages;
        sizeOfDownloadedData += buffer.size();
    }

    return buffer;
}

bool TPageDownloader::CheckUrl(const std::string& url) const throw() {
    CURLcode code(CURLE_FAILED_INIT);
    CURL* curl = curl_easy_init();

    if (curl) {
        if (CURLE_OK == curl_easy_setopt(curl, CURLOPT_HEADER, 1L)
        && CURLE_OK == curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, null_write)
        && CURLE_OK == curl_easy_setopt(curl, CURLOPT_WRITEDATA, 0)
        && CURLE_OK == curl_easy_setopt(curl, CURLOPT_WILDCARDMATCH, 0)
        && CURLE_OK == curl_easy_setopt(curl, CURLOPT_NOBODY, 1L)
        && CURLE_OK == curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L)
        && CURLE_OK == curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 5L)
        && CURLE_OK == curl_easy_setopt(curl, CURLOPT_URL, url.c_str())) {
            code = curl_easy_perform(curl);
        }
        curl_easy_cleanup(curl);
    }

    if (code != CURLE_OK) {
        return false;
    }

    return true;
}

std::string TPageDownloader::GetUrl(const std::string& url) throw()
{
    return GetUrl(url, DEFAULT_TIMEOUT);
}
