#include "crawl/htmlparser.hpp"

#include <sstream>
#include <fstream>
#include <regex>
#include <libxml/HTMLparser.h>
#include <libxml++/libxml++.h>

namespace crawl {

  class HtmlParser::HtmlParserPrivate
  {
  public:
    HtmlParserPrivate(HtmlParser *p_parent = NULL)
    {
    }

    ~HtmlParserPrivate() {
      xmlFreeDoc(htmlDoc);
      xmlCleanupParser();
      xmlMemoryDump();
    }

    htmlDocPtr htmlDoc;
  };

  HtmlParser::HtmlParser()
    : d(new HtmlParserPrivate())
  {
  }

  HtmlParser::~HtmlParser() {
    delete d;
  }

  xmlpp::Node* HtmlParser::convert(const std::string &html_file_path, const std::string &encoding) const {
    std::string buf;
    std::ifstream fin(html_file_path, std::ios::in);
    std::stringstream sout;

    int is_replaced = 0;
    const static std::regex re("^<html.*$");
    while (std::getline(fin, buf)) {
      if (is_replaced <= 2) {
        if (std::string::npos != buf.find("<!DOCTYPE")) {
          is_replaced++;
          continue;
        }
        if (std::string::npos != buf.find("<html")) {
          is_replaced++;
          buf = "<html>";
        }
      }
      sout << buf << std::endl;
    }
    fin.close();

    d->htmlDoc = htmlReadMemory(sout.str().c_str(), sout.str().length(), "", encoding.c_str(), HTML_PARSE_NOBLANKS | HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING | HTML_PARSE_NONET | HTML_PARSE_NODEFDTD);
    if (!d->htmlDoc)
      return NULL;
    xmlpp::Document *doc = new xmlpp::Document(d->htmlDoc);
    return doc->get_root_node();
  }
}
