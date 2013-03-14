#include <iostream>

#include <link_parser.h>

int main() {
    //std::cout << GetDomain("http://ndev.vsv.lokos.net/") << std::endl;
    /*std::cout << GetDomain("http://ya.ru") << std::endl;
    std::cout << GetDomain("https://ya.ru") << std::endl;
    std::cout << GetDomain("https://ya.ru/search?q='something not interesting'") << std::endl;
    std::cout << GetDomain("https://ya.ru/bada/home/asdfa/asdfsqw/q/erqer/asdf/asdf/zx/cvzx/cv/index.html") << std::endl;
    std::cout << GetDomain("https://www.ya.ru/bada/home/asdfa/asdfsqw/q/erqer/asdf/asdf/zx/cvzx/cv/index.html") << std::endl;

    std::cout << GetDomain("some.html") << std::endl;
    std::cout << GetDomain("3some.html") << std::endl;
    std::cout << GetDomain("ome") << std::endl;
    std::cout << GetDomain("index.php") << std::endl;*/

    std::cout << NormalizeUrl("http://silikatsemey.kz/") << std::endl;
    std::cout << "'" << NormalizeUrl("silikatsemey.kz/ ") << "'" <<std::endl;

    return 0;
}
