#include <crawler.h>

int main() {
    TSimpleCrawlerConfiguration config("http://silikatsemey.kz/", 3, 1000);
    config.pathToDumpFile = "./results";

    TSimpleCrawler(config).Start();
    return 0;
}
