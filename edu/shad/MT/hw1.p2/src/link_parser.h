/**
    @brief Parse xml/html to get set of links.

    Use htmlcxx parser and boost xml parser.
    Use boost regex to check link.

    Required installed boost and htmlcxx library.
    To install that libraries just execute next:
    $ sudo apt-get install libhtmlcxx-dev libboost-all-dev


    For compiling add this flags '-lboost_graph',
    '-lboost_regex', '-lhtmlcxx' to compiler.

    @author Aman Orazaev
*/
#pragma once

#include <set>
#include <string>
#include <sstream>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/regex.hpp>
#include <htmlcxx/html/ParserDom.h>

#include <messages.h>


/** @brief check link policy */
bool isLink(const std::string&);

/** @brief Boost xml parser. */
class TBoostXmlLinkParser {
    typedef boost::property_tree::ptree ptree;
public:
    static std::set<std::string> ParseText(const std::string& text);
    static std::set<std::string> ParseText(std::istream& istream);
    static std::set<std::string> ParseText(const TTaskMessage& msg);

private:
    TBoostXmlLinkParser();

private:
    static void ParseXmlTree(ptree&, std::set<std::string>*);
};

/** @brief Htmlcxx html parser */
class THtmlcxxLinkParser {
public:
    static std::set<std::string> ParseText(
            const std::string& text,
            const std::string& url);

    static std::set<std::string> ParseText(const std::string& text);
    static std::set<std::string> ParseText(std::istream& istream);
    static std::set<std::string> ParseText(const TTaskMessage& msg);

private:
    THtmlcxxLinkParser();
};

/** Use htmlcxx parser for crawler */
typedef THtmlcxxLinkParser TLinkParser;
