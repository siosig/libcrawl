#pragma once

#include <boost/filesystem.hpp>

namespace crawl {
  class Tidy {
  public:
    Tidy();
    ~Tidy();
    std::string convert(const boost::filesystem::path &htmlFile, const std::string &encoding = "utf8") const;
  private:
    class TidyPrivate;
    TidyPrivate *d;
  };
}
