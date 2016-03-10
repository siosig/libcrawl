#pragma once
#include <string>
#include <libxml++/nodes/node.h>
#include <log4cpp/Category.hh>
#include <boost/date_time.hpp>

#define DEBUG(_STR_) (log4cpp::Category::getRoot()).debug((boost::format("%s %s %s") % __FILE__ % __LINE__ % (_STR_)).str())

#define INFO(_STR_) (log4cpp::Category::getRoot()).info((boost::format("%s %s %s") % __FILE__ % __LINE__ % (_STR_)).str())

#define WARN(_STR_) (log4cpp::Category::getRoot()).warn((boost::format("%s %s %s") % __FILE__ % __LINE__ % (_STR_)).str())

#define ERR(_STR_) (log4cpp::Category::getRoot()).error((boost::format("%s %s %s") % __FILE__ % __LINE__ % (_STR_)).str())

namespace crawl {
  std::string get_attr_value(const xmlpp::Node *node, const std::string &path);
  std::string get_content_value(const xmlpp::Node *node, const std::string &path, uint32_t index = 0);
  std::string current_datetime_str();
  std::string current_date_str();
  std::string current_time_str();
  std::string to_date_string (boost::posix_time::ptime t);
  int32_t cstoi(const std::string &str);
  float cstof(const std::string &str);
  std::string str_replace(const std::string &str, const std::string &key, const std::string &val);
}
