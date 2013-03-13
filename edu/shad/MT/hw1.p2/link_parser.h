/**
    @brief Parse xml(html) to get vector of links.

    Use boost xml parser, and boost regex to check
    link.

    Required installed boost library.

    For compiling to add flags '-lboost_graph'
    and '-lboost_regex'.

    @author Aman Orazaev
*/
#pragma once

#include <set>
#include <string>
#include <sstream>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/regex.hpp>


/** @brief href parser class */
class TLinkParser {
    typedef boost::property_tree::ptree ptree;
public:
    static std::set<std::string> ParseText(const std::string& text);

    private:
    static void ParseXmlTree(
            ptree&,
            std::set<std::string>*);
    static bool isLink(const std::string&);

private:
    TLinkParser();
};

