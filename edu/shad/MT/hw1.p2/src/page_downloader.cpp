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
        && CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L))
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

std::string TPageDownloader::GetUrl(const std::string& url) throw()
{
    return GetUrl(url, DEFAULT_TIMEOUT);
}
