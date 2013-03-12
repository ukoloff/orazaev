#include <page_downloader.h>
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Enter url as program argument.\n";
        return 1;
    }

    TPageDownloader downloader;
    std::cout << downloader.GetUrl(argv[1]) << std::endl;
    downloader.GetUrl("ya.ru");

    std::cout << "\n\nNum of pages: " << downloader.GetNumbeOfPages()
              << "\nData size: " << downloader.GetSizeOfData() << "\n";

    return 0;
}
