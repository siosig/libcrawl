#include "crawl/http.hpp"
#include "crawl/ualist.hpp"
#include <stdlib.h>
#include <iostream>
#include <map>
#include <curl/curl.h>
#include <thread>

size_t callbackWrite(char *ptr, size_t size, size_t nmemb, std::string *stream)
{
  uint32_t dataLength = size * nmemb;
  stream->append(ptr, dataLength);
  return dataLength;
}

std::string _download(const std::string &url, const std::string &ua,uint32_t timeout) {
  std::string chunk;
  CURL *curl = curl_easy_init();
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callbackWrite);
  curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);
  curl_easy_setopt(curl, CURLOPT_USERAGENT, ua.c_str());
  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &chunk);
  CURLcode ret = curl_easy_perform(curl);
  curl_easy_cleanup(curl);
  return chunk;
}

namespace crawl {
  class HTTP::HTTPPrivate {
  public:
    HTTPPrivate()
    {
    }

    ~HTTPPrivate() {
    }

    const char* getUserAgent() {
      uint32_t index = rand() % (sizeof(UALIST) / sizeof(std::string));
      return UALIST[index].c_str();
    }

    uint32_t timeout;
  };

  HTTP::HTTP()
    : d(new HTTPPrivate)
  {
  }

  HTTP::~HTTP() {
    delete d;
  }

  void HTTP::setTimeout(uint32_t timeout) {
    d->timeout = timeout;
  }

  std::string HTTP::download(const std::string &url) {
    return _download(url, d->getUserAgent(), d->timeout);
  }

};
