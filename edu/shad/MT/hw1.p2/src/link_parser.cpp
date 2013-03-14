#include <iterator>
#include <iostream>
#include <algorithm>

#include <link_parser.h>

std::set<std::string> TBoostXmlLinkParser::ParseText(
        const std::string& text)
{
    printf("PARSER:\n%s\n\n\n", text.c_str());
    ptree xmlTree;
    std::stringstream stream(text, std::stringstream::in);
    boost::property_tree::read_xml(stream, xmlTree);

    std::set<std::string> result;
    ParseXmlTree(xmlTree, &result);

    return result;
}

std::set<std::string> TBoostXmlLinkParser::ParseText(
        std::istream& istream)
{
    ptree xmlTree;
    boost::property_tree::read_xml(istream, xmlTree);

    std::set<std::string> result;
    ParseXmlTree(xmlTree, &result);

    return result;
}

void TBoostXmlLinkParser::ParseXmlTree(
        ptree& xmlTree,
        std::set<std::string>* result)
{
    for (ptree::value_type &node : xmlTree)
    {
        if (node.first == "a") {
            std::string href;
            try {
                href = node.second.get<std::string>("<xmlattr>.href");
            } catch (...) {
                href = "";
            }

            if (isLink(href)) {
                result->insert(href);
            }
        }

        ParseXmlTree(node.second, result);
    }
}

bool isLink(const std::string& link) {
    boost::regex start_hash(R"__(^\s*#.*)__");
    boost::regex mailto(R"__(^\s*mailto:.*)__");
    boost::regex at(R"__(.*\@.*)__");
    boost::regex file(R"__(.*\.(?:(?:jpg)|(?:png)|(?:pdf)|(?:doc)|(?:docx)|(?:gif)|(?:ppt)|(?:pptx)|(?:pps)))__");

    if (link == ""
        || boost::regex_match(link, start_hash)
        || boost::regex_match(link, mailto)
        || boost::regex_match(link, at)
        || boost::regex_match(link, file))
    {
        return false;
    }

    return true;
}

std::set<std::string> THtmlcxxLinkParser::ParseText(const std::string& text) {
    std::set<std::string> result;
    htmlcxx::HTML::ParserDom parser;
    tree<htmlcxx::HTML::Node> dom = parser.parseTree(text);

    for (auto node : dom) {
        if (node.tagName() == "a") {
            node.parseAttributes();
            std::string link = node.attribute("href").second;

            if (isLink(link)) {
                result.insert(link);
            }
        }
    }

    return result;
}

std::set<std::string> THtmlcxxLinkParser::ParseText(std::istream& istream) {
    istream >> std::noskipws;
    std::stringstream text(std::stringstream::out);
    std::copy(
        std::istream_iterator<char>(istream),
        std::istream_iterator<char>(),
        std::ostream_iterator<char>(text)
    );

    return THtmlcxxLinkParser::ParseText(text.str());
}
