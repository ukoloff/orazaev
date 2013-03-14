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

            if (IsLink(href)) {
                result->insert(href);
            }
        }

        ParseXmlTree(node.second, result);
    }
}

bool IsLink(const std::string& link) {
    boost::regex start_hash(R"__(^\s*#.*)__", boost::regex::icase);
    boost::regex mailto(R"__(^\s*mailto:.*)__", boost::regex::icase);
    boost::regex at(R"__(.*\@.*)__", boost::regex::icase);
    boost::regex checkIsArchive(R"__(.*\.(?:(?:zip)|(?:rar)|(?:tar\.bz)|(?:tar\.bz2)|(?:7z)))__", boost::regex::icase);
    boost::regex checkIsVideo(R"__(.*\.(?:(?:avi)|(?:mpeg)|(?:m4)|(?:3gp)|(?:mov)|(?:swf)))__", boost::regex::icase);
    boost::regex checkIsImage(R"__(.*\.(?:(?:jpg)|(?:png)|(?:gif)))__", boost::regex::icase);
    boost::regex checkIsDocument(R"__(.*\.(?:(?:pdf)|(?:doc)|(?:docx)|(?:ppt)|(?:pptx)|(?:pps)|(?:exe)|(?:txt)|(?:xls)|(?:xlsx)|(?:djvu)))__", boost::regex::icase);

    if (link == ""
        || boost::regex_match(link, start_hash)
        || boost::regex_match(link, mailto)
        || boost::regex_match(link, at)
        || boost::regex_match(link, checkIsImage)
        || boost::regex_match(link, checkIsDocument)
        || boost::regex_match(link, checkIsArchive)
        || boost::regex_match(link, checkIsVideo))
    {
        return false;
    }

    return true;
}

std::string GetDomain(const std::string& url) {
    boost::regex protocolHttp(R"__(^\s*http://)__", boost::regex::icase);
    boost::regex protocolHttps(R"__(^\s*https://)__", boost::regex::icase);
    boost::regex charactersAfterSlash(R"__(/.*$)__", boost::regex::icase);
    boost::regex www(R"__(^\s*www\.)__", boost::regex::icase);

    std::string result = boost::regex_replace(url, protocolHttp, "");
    result = boost::regex_replace(result, protocolHttps, "");
    result = boost::regex_replace(result, charactersAfterSlash, "");
    result = boost::regex_replace(result, www, "");

    return IsDomain(result) ? result : "";
}

bool IsDomain(const std::string& something) {
    boost::regex checkDomainZone(R"__(^[\w\d][\w\d\.-]*\.[a-z]{2,4})__", boost::regex::icase);
    boost::regex checkIsImage(R"__(.*\.(?:(?:jpg)|(?:png)|(?:gif)))__", boost::regex::icase);
    boost::regex checkIsDocument(R"__(.*\.(?:(?:pdf)|(?:doc)|(?:docx)|(?:ppt)|(?:pptx)|(?:pps)|(?:exe)|(?:txt)|(?:xls)|(?:xlsx)|(?:djvu)))__", boost::regex::icase);
    boost::regex checkIsArchive(R"__(.*\.(?:(?:zip)|(?:rar)|(?:tar\.bz)|(?:tar\.bz2)|(?:7z)))__", boost::regex::icase);
    boost::regex checkIsVideo(R"__(.*\.(?:(?:avi)|(?:mpeg)|(?:m4)|(?:3gp)|(?:mov)))__", boost::regex::icase);
    boost::regex checkIsHtml(R"__(.*\.(?:(?:html)|(?:htm)|(?:php)))__", boost::regex::icase);

    if (something == ""
        || boost::regex_match(something, checkIsImage)
        || boost::regex_match(something, checkIsDocument)
        || boost::regex_match(something, checkIsHtml)
        || boost::regex_match(something, checkIsArchive)
        || boost::regex_match(something, checkIsVideo)
        || !boost::regex_match(something, checkDomainZone))
    {
        return false;
    }
    return true;
}

std::string NormalizeUrl(const std::string& url) {
    boost::regex protocolHttp(R"__(^\s*http://)__", boost::regex::icase);
    boost::regex protocolHttps(R"__(^\s*https://)__", boost::regex::icase);
    boost::regex www(R"__(^\s*www\.)__", boost::regex::icase);
    boost::regex lastSlash(R"__(/$)__", boost::regex::icase);
    boost::regex severalSlashs(R"__(//*)__", boost::regex::icase);
    boost::regex hashEnded(R"__(#.*$)__", boost::regex::icase);
    boost::regex leftStrip(R"__(^\s\s*)__", boost::regex::icase);
    boost::regex rightStrip(R"__(\s\s*$)__", boost::regex::icase);
    boost::regex jsQuery(R"__(\?.*$)__", boost::regex::icase);

    std::string result = boost::regex_replace(url, leftStrip, "");
    result = boost::regex_replace(result, rightStrip, "");
    result = boost::regex_replace(result, protocolHttp, "");
    result = boost::regex_replace(result, protocolHttps, "");
    result = boost::regex_replace(result, www, "");
    result = boost::regex_replace(result, hashEnded, "");
    result = boost::regex_replace(result, severalSlashs, "/");

    result = boost::regex_replace(result, jsQuery, "");

    result = boost::regex_replace(result, lastSlash, "");

    return result;
}

std::set<std::string> THtmlcxxLinkParser::ParseText(
        const std::string& text,
        const std::string& url)
{
    std::set<std::string> result;
    htmlcxx::HTML::ParserDom parser;
    tree<htmlcxx::HTML::Node> dom = parser.parseTree(text);
    std::string domain = GetDomain(url);

    for (auto node : dom) {
        if (node.tagName() == "a") {
            node.parseAttributes();
            std::string link = node.attribute("href").second;

            if (IsLink(link)) {
                if (GetDomain(link) != "") {
                    result.insert(NormalizeUrl(link));
                } else {
                    result.insert(NormalizeUrl(domain + "/" + link));
                }
            }
        }
    }

    return result;
}

std::set<std::string> THtmlcxxLinkParser::ParseText(const std::string& text) {
    return THtmlcxxLinkParser::ParseText(text, "");
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

std::set<std::string> THtmlcxxLinkParser::ParseText(const TTaskMessage& msg) {
    return THtmlcxxLinkParser::ParseText(*msg.GetHtml(), *msg.GetUrl());
}
