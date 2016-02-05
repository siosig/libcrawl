#include "crawl/http.hpp"
#include "crawl/ualist.hpp"
#include <stdlib.h>
#include <iostream>
#include <map>
#include <curl/curl.h>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>


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

void _download(const std::string &url, const std::string &ua,uint32_t timeout, boost::promise<std::string> &p) {
  p.set_value(_download(url, ua, timeout));
}

typedef std::pair<boost::thread*, boost::promise<std::string>* > thpair;

namespace crawl {
  class HTTP::HTTPPrivate {
  public:
    HTTPPrivate()
    {
    }

    ~HTTPPrivate() {
    }

    const char* getUserAgent() {
      uint32_t index = rand() % ( sizeof(UALIST) / sizeof(std::string) );
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

#if 0
  std::list<std::string> HTTP::download(const std::list<std::string> &url_list, uint32_t thread_count) {
    std::list<std::string> chunk_list;
    uint32_t cnt = 0;
    boost::thread_group thr_grp;
    std::list<thpair> dlist;
    BOOST_FOREACH(const std::string &url, url_list){
      boost::promise<std::string> *p = new boost::promise<std::string>();
      boost::thread *th = thr_grp.create_thread(boost::bind(&_download, url, d->getUserAgent(), d->timeout, boost::ref(*p)));
      dlist.push_back(thpair(th, p));
      if (! (++cnt % thread_count)) {
        thr_grp.join_all();
        BOOST_FOREACH(thpair pair, dlist) {
          boost::unique_future<std::string> f = pair.second->get_future();
          chunk_list.push_back(f.get());
          thr_grp.remove_thread(pair.first);
          delete pair.second;
        }
        dlist.clear();
      }
    }
    thr_grp.join_all();
    BOOST_FOREACH(thpair pair, dlist) {
      boost::unique_future<std::string> f = pair.second->get_future();
      chunk_list.push_back(f.get());
      thr_grp.remove_thread(pair.first);
      delete pair.second;
    }

    return chunk_list;
  }
#endif

  std::string HTTP::download(const std::string &url) {
    return _download(url, d->getUserAgent(), d->timeout);
  }

};
