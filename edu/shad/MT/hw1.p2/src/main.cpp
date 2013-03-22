#include <stdlib.h>
#include <crawler.h>

void usage(const char* prog) {
    printf("Usage:\n\t%s [url] [max web graph depth] [max pages to download] [path to pages.dump]\n", prog);
    exit(1);
}

int main(int argc, char** argv) {
    if (argc < 5) {
        usage(argv[0]);
    }

    std::string url = argv[1];
    std::string path = argv[4];
    size_t depth = atoi(argv[2]) - 1;
    size_t maxPages = atoi(argv[3]);

    TCrawlerConfiguration config(url, depth, maxPages);
    config.pathToDumpFile = path;

    TCrawler(config).Start();
    return 0;
}
