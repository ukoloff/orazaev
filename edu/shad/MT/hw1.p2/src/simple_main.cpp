#include <crawler.h>

int main() {
    TSimpleCrawlerConfiguration config("http://silikatsemey.kz/", 2, 1000);
    config.pathToDumpFile = "./results";

    TSimpleCrawler(config).Start();
    return 0;
}
