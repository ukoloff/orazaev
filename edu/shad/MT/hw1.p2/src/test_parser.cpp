#include <iostream>
#include <link_parser.h>

int main() {
    std::string xmlData = "<html><head></head><body>\
        <a href=\"http://google.ru\"> link to google </a>\
        <div>\
            <a href=\"http://ya.ru\"> link to yandex </a>\
        </div>\
        <div>\
            <a href=\"http://yahoo.ru\"> link to yahoo </a>\
            <div>\
                <a href=\"http://badoo.com\"> link to badoo </a>\
            </div>\
            <img />\
            <a />\
        </div>\
    </body></html><a href='some@href' />";

    std::set<std::string> links = TLinkParser::ParseText(std::cin);

    for (std::string link : links) {
        std::cout << link << "\n";
    }

    return 0;
}
