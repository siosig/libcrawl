#pragma once

#include "crawl/executer.hpp"
#include <string>

namespace crawl {
  class Downloader : public Executer {
  public:
    Downloader(const std::string &log4cpp_config_path);
    virtual ~Downloader();

  protected:
    uint32_t doExecute();
    uint32_t enqueueRequest(const std::string &url, const std::string &savepath);
    void wait();

    virtual uint32_t downloadData() { return 0; }
  private:
    class DownloaderPrivate;
    DownloaderPrivate *d;
  };
}
