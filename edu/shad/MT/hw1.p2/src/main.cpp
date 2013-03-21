#include <crawler.h>

int main() {
    TCrawlerConfiguration config("http://silikatsemey.kz/", 2);
    config.pathToDumpFile = "./results";

    TCrawler(config).Start();
    return 0;
}
