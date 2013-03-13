#include <link_parser.h>

std::set<std::string> TLinkParser::ParseText(
        const std::string& text)
{
    ptree xmlTree;
    std::stringstream stream(text, std::stringstream::in);
    boost::property_tree::read_xml(stream, xmlTree);

    std::set<std::string> result;
    ParseXmlTree(xmlTree, &result);

    return result;
}

void TLinkParser::ParseXmlTree(
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

bool TLinkParser::isLink(const std::string& link) {
    boost::regex start_hash(R"__(^\s*#.*)__");
    boost::regex mailto(R"__(^\s*mailto:.*)__");
    boost::regex at(R"__(.*\@.*)__");

    if (link == ""
        || boost::regex_match(link, start_hash)
        || boost::regex_match(link, mailto)
        || boost::regex_match(link, at))
    {
        return false;
    }

    return true;
}
