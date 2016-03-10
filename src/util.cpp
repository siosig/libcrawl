#include "crawl/util.hpp"
#include <stdio.h>
#include <sstream>
#include <vector>
#include <regex>
#include <libxml/HTMLparser.h>
#include <libxml++/libxml++.h>

namespace crawl {
  std::string get_attr_value(const xmlpp::Node *node, const std::string &path) {
    xmlpp::NodeSet set = node->find(path);
    if (set.size()) {
      const xmlpp::Attribute *nodeAttr =  dynamic_cast<const xmlpp::Attribute*>(set.at(0));
      if (nodeAttr) {
        return nodeAttr->get_value();
      }
    }
    return "";
  }

  std::string get_content_value(const xmlpp::Node *node, const std::string &path, uint32_t index) {
    xmlpp::NodeSet set = node->find(path);
    if (set.size()) {
      const xmlpp::ContentNode* nodeContent = dynamic_cast<const xmlpp::ContentNode*>(set.at(index));
      if (nodeContent) {
        return nodeContent->get_content();
      }
    }
    return "";
  }


  std::string current_datetime_str() {
    boost::posix_time::ptime current_ptime = boost::posix_time::second_clock::local_time();
    return boost::posix_time::to_iso_string(current_ptime);
  }

  std::string current_date_str() {
    boost::posix_time::ptime current_ptime = boost::posix_time::second_clock::local_time();
    return boost::posix_time::to_iso_string(current_ptime).substr(0, 8);
  }

  std::string current_time_str() {
    boost::posix_time::ptime current_ptime = boost::posix_time::second_clock::local_time();
    return boost::posix_time::to_iso_string(current_ptime).substr(9);
  }

  std::string to_date_string (boost::posix_time::ptime t) {
    return boost::posix_time::to_iso_string(t).substr(0, 8);
  }

  int32_t cstoi(const std::string &str) {
    uint32_t ret = 0;
    std::stringstream tmp;
    char c = 0;
    for(std::string::const_iterator it = str.begin(); it != str.end(); it++){
      c = *it;
      if (c== '.')
        break;
      if (c == '-' || (c >= '0' && c <= '9')) {
        tmp << c;
      }
    }
    const std::string s = tmp.str();
    if (!s.length())
      return 0;
    c = s.at(s.length() - 1 );
    if ('0' <= c && c <= '9') {
      return std::stoi(s.c_str());
    }
    return 0;
  }

  float cstof(const std::string &str) {
    float ret = 0;
    std::stringstream tmp;
    char c = 0;
    for(std::string::const_iterator it = str.begin(); it != str.end(); it++){
      c = *it;
      if (c == '-' || c == '.' || (c >= '0' && c <= '9')) {
        tmp << c;
      }
    }
    const std::string s = tmp.str();
    if (!s.length())
      return 0;
    c = s.at(s.length() - 1 );
    if ('0' <= c && c <= '9') {
      return std::stof(s.c_str());
    }
    return 0;
  }

  std::string str_replace(const std::string &str, const std::string &key, const std::string &val) {
    std::string ret = str;
    std::string::size_type  pos(ret.find(key));
    while(pos != std::string::npos) {
      ret.replace( pos, key.length(), val );
      pos = ret.find( key, pos + val.length());
    }
    return ret;
  }
}
