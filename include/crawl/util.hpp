#pragma once

#include <string>
#include <vector>
#include <libxml++/libxml++.h>
#include <libxml++/nodes/node.h>
#include <log4cpp/Category.hh>
#include <boost/date_time.hpp>
#include <boost/foreach.hpp>

#define DEBUG(_STR_) (log4cpp::Category::getRoot()).debug((boost::format("%s %s %s") % __FILE__ % __LINE__ % (_STR_)).str())

#define INFO(_STR_) (log4cpp::Category::getRoot()).info((boost::format("%s %s %s") % __FILE__ % __LINE__ % (_STR_)).str())

#define WARN(_STR_) (log4cpp::Category::getRoot()).warn((boost::format("%s %s %s") % __FILE__ % __LINE__ % (_STR_)).str())

#define ERR(_STR_) (log4cpp::Category::getRoot()).error((boost::format("%s %s %s") % __FILE__ % __LINE__ % (_STR_)).str())

inline static std::string get_attr_value(const xmlpp::Node *node, const std::string &path) {
  xmlpp::NodeSet set = node->find(path);
  if (set.size()) {
    const xmlpp::Attribute *nodeAttr =  dynamic_cast<const xmlpp::Attribute*>(set.at(0));
    if (nodeAttr) {
      return nodeAttr->get_value();
    }
  }
  return "";
}

inline static std::string get_content_value(const xmlpp::Node *node, const std::string &path, uint32_t index = 0) {
  xmlpp::NodeSet set = node->find(path);
  if (set.size()) {
    const xmlpp::ContentNode* nodeContent = dynamic_cast<const xmlpp::ContentNode*>(set.at(index));
    if (nodeContent) {
      return nodeContent->get_content();
    }
  }
  return "";
}


inline static std::string to_date_string (boost::posix_time::ptime t) {
  return boost::posix_time::to_iso_string(t).substr(0, 8);
}

inline static uint32_t stoui(const std::string &str) {
  uint32_t ret = 0;
  BOOST_FOREACH(char c, str) {
    if (c == ',')
      continue;
    if (c == '.')
      break;
    ret = ret * 10 + atoi(&c);
  }
  return ret;
}

inline std::string str_replace(const std::string &str, const std::string &key, const std::string &val) {
  std::string ret = str;
  std::string::size_type  pos(ret.find(key));
  while(pos != std::string::npos) {
    ret.replace( pos, key.length(), val );
    pos = ret.find( key, pos + val.length());
  }
  return ret;
}

inline static void narrow(const std::wstring &src, std::string &dest) {
  char *mbs = new char[src.length() * MB_CUR_MAX + 1];
  wcstombs(mbs, src.c_str(), src.length() * MB_CUR_MAX + 1);
  dest = mbs;
  delete [] mbs;
}
