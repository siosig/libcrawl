#pragma once

#include <list>
#include <string>

namespace crawl {
  class HTTP {
  public:
    HTTP();
    ~HTTP();
    void setTimeout(uint32_t timeout);
    std::string download(const std::string &url);
  private:
    class HTTPPrivate;
    HTTPPrivate *d;
  };
}
