#pragma once
#include <string>
#include <libxml++/nodes/node.h>

namespace crawl {
  class HtmlParser {
  public:
    HtmlParser();
    ~HtmlParser();
    xmlpp::Node* convert(const std::string &html_file_path, const std::string &encoding = "UTF-8") const;
  private:
    class HtmlParserPrivate;
    HtmlParserPrivate *d;
  };
}
