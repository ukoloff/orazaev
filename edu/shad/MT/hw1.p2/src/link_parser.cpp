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

            if (TUrlProcess::IsLink(href)) {
                result->insert(href);
            }
        }

        ParseXmlTree(node.second, result);
    }
}

const boost::regex TUrlProcess::checkIsArchive(R"__(.*\.(?:(?:zip)|(?:rar)|(?:tar\.bz)|(?:tar\.bz2)|(?:7z)))__", boost::regex::icase);
const boost::regex TUrlProcess::checkIsVideo(R"__(.*\.(?:(?:avi)|(?:mpeg)|(?:m4)|(?:3gp)|(?:mov)|(?:swf)))__", boost::regex::icase);
const boost::regex TUrlProcess::checkIsImage(R"__(.*\.(?:(?:jpg)|(?:png)|(?:gif)))__", boost::regex::icase);
const boost::regex TUrlProcess::checkIsDocument(R"__(.*\.(?:(?:xml)|(?:pdf)|(?:doc)|(?:docx)|(?:ppt)|(?:pptx)|(?:pps)|(?:exe)|(?:txt)|(?:xls)|(?:xlsx)|(?:djvu)))__", boost::regex::icase);
const boost::regex TUrlProcess::checkIsMailto(R"__(^\s*mailto:.*)__", boost::regex::icase);
const boost::regex TUrlProcess::checkIsAt(R"__(.*\@.*)__", boost::regex::icase);
const boost::regex TUrlProcess::checkIsHttpProtocol(R"__(^\s*http://)__", boost::regex::icase);
const boost::regex TUrlProcess::checkIsHttpsProtocol(R"__(^\s*https://)__", boost::regex::icase);
const boost::regex TUrlProcess::checkIsHtml(R"__(.*\.(?:(?:html)|(?:htm)|(?:php)))__", boost::regex::icase);
const boost::regex TUrlProcess::checkDomainZone(R"__(^[\w\d][\w\d\.-]*\.[a-z]{2,4})__", boost::regex::icase);
const boost::regex TUrlProcess::checkIsSlashEnded(R"__(/$)__", boost::regex::icase);
const boost::regex TUrlProcess::checkIsSeveralSlashes(R"__(//*)__", boost::regex::icase);
const boost::regex TUrlProcess::checkIsHashTag(R"__(#.*$)__", boost::regex::icase);
const boost::regex TUrlProcess::checkIsStartWithSpaces(R"__(^\s\s*)__", boost::regex::icase);
const boost::regex TUrlProcess::checkIsTrailingSpaces(R"__(\s\s*$)__", boost::regex::icase);
const boost::regex TUrlProcess::checkIsJsQuery(R"__(\?.*$)__", boost::regex::icase);
const boost::regex TUrlProcess::charactersAfterSlash(R"__(/.*$)__", boost::regex::icase);
const boost::regex TUrlProcess::www(R"__(^\s*www\.)__", boost::regex::icase);


bool TUrlProcess::IsLink(const std::string& link) {
    if (link == ""
        || boost::regex_match(link, checkIsMailto)
        || boost::regex_match(link, checkIsAt)
        || boost::regex_match(link, checkIsImage)
        || boost::regex_match(link, checkIsDocument)
        || boost::regex_match(link, checkIsArchive)
        || boost::regex_match(link, checkIsVideo))
    {
        return false;
    }

    return true;
}

std::string TUrlProcess::GetDomain(const std::string& url) {
    std::string result = boost::regex_replace(url, checkIsHttpProtocol, "");
    result = boost::regex_replace(result, checkIsHttpsProtocol, "");
    result = boost::regex_replace(result, charactersAfterSlash, "");
    result = boost::regex_replace(result, www, "");

    return IsDomain(result) ? result : "";
}

bool TUrlProcess::IsDomain(const std::string& something) {
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

std::string TUrlProcess::NormalizeUrl(const std::string& url) {

    std::string result = boost::regex_replace(url, checkIsStartWithSpaces, "");
    result = boost::regex_replace(result, checkIsTrailingSpaces, "");
    result = boost::regex_replace(result, checkIsHttpProtocol, "");
    result = boost::regex_replace(result, checkIsHttpsProtocol, "");
    result = boost::regex_replace(result, www, "");
    result = boost::regex_replace(result, checkIsHashTag, "");
    result = boost::regex_replace(result, checkIsSeveralSlashes, "/");

    result = boost::regex_replace(result, checkIsJsQuery, "");

    result = boost::regex_replace(result, checkIsSlashEnded, "");

    return result;
}

std::set<std::string> THtmlcxxLinkParser::ParseText(
        const std::string& text,
        const std::string& url)
{
    std::set<std::string> result;
    htmlcxx::HTML::ParserDom parser;
    tree<htmlcxx::HTML::Node> dom = parser.parseTree(text);
    std::string domain = TUrlProcess::GetDomain(url);

    for (auto node : dom) {
        if (node.tagName() == "a") {
            node.parseAttributes();
            std::string link = node.attribute("href").second;

            if (TUrlProcess::IsLink(link)) {
                if (TUrlProcess::GetDomain(link) != "") {
                    result.insert(TUrlProcess::NormalizeUrl(link));
                } else {
                    result.insert(TUrlProcess::NormalizeUrl(domain + "/" + link));
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
