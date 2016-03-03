#pragma once
#include <string>

namespace crawl {
  class Tidy {
  public:
    Tidy();
    ~Tidy();
    std::string convert(const std::string &html_file_path, const std::string &encoding = "utf8") const;
  private:
    class TidyPrivate;
    TidyPrivate *d;
  };
}
