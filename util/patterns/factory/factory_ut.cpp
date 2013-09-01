#include "factory.hpp"
#include <cassert>
#include <string>


/* Abstract class for factory testing.
 */
class TestParser {
public:
    virtual std::string parse(const std::string& data) = 0;
    virtual ~TestParser() { }
};
/* @brief Singelton factory
 */
Factory<std::string, TestParser>& getParserFactory() {
    static Factory<std::string, TestParser> parserFactory;
    return parserFactory;
}


///////////////////////////////////////////////////


class XmlParser : public TestParser {
public:
    std::string parse(const std::string& data) {
        return std::string("XML_PARSING: ") + data;
    }
};
/* @brief Factory registration example for concrete
 *        class XmlParser.
 */
namespace {
    RegisterToFactory<XmlParser> registerXmlParser(getParserFactory(), "xml");
}


///////////////////////////////////////////////////


class TxtParser : public TestParser {
public:
    std::string parse(const std::string& data) {
        return std::string("TXT_PARSING: ") + data;
    }
};
/* @brief Concrete factory registration example
 */
namespace {
    RegisterToFactory<TxtParser> registerTxtParser(getParserFactory(), "txt");
}


///////////////////////////////////////////////////

/* @brief This code only for fabric testing.
 */
namespace {
    std::tr1::shared_ptr<TestParser> createTxtParser() {
        return std::tr1::shared_ptr<TestParser>(new TxtParser());
    }

    std::tr1::shared_ptr<TestParser> createXmlParser() {
        return std::tr1::shared_ptr<TestParser>(new XmlParser());
    }
}


///////////////////////////////////////////////////
// Tests
///////////////////////////////////////////////////


void test_Factory() {
    Factory<std::string, TestParser> ParserFactory;
    ParserFactory.add("xml", createXmlParser);
    ParserFactory.add("txt", createTxtParser);

    assert(ParserFactory.create("xml")->parse("abc") == "XML_PARSING: abc");
    assert(ParserFactory.create("txt")->parse("def") == "TXT_PARSING: def");
}


void test_RegisterToFactory() {
    assert(getParserFactory().create("xml")->parse("bcd") == "XML_PARSING: bcd");
    assert(getParserFactory().create("txt")->parse("cde") == "TXT_PARSING: cde");
}


int main() {
    test_Factory();
    test_RegisterToFactory();

    return 0;
}
