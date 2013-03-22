#include <crawler.h>

int main() {
    TCrawlerConfiguration config("http://silikatsemey.kz/", 3, 1000);
    config.pathToDumpFile = "./results";

    TCrawler(config).Start();
    return 0;
}
