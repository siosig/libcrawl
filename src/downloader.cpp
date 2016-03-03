#include "crawl/executer.hpp"
#include "crawl/downloader.hpp"
#include "crawl/thread.hpp"
#include "crawl/http.hpp"
#include <log4cpp/Category.hh>
#include <log4cpp/PropertyConfigurator.hh>
#include <fstream>

class MultiHTTPClient : public crawl::thread<std::string> {
public:
  MultiHTTPClient(){
  }
  MultiHTTPClient(const std::string &_url, const std::string &_savepath)
    : url(_url)
    , callback_ref(std::bind(&MultiHTTPClient::callback, this, std::placeholders::_1, std::placeholders::_2))
    , savepath(_savepath)
  {
    setCallbackFunction(callback_ref);
    http.setTimeout(120);
  }
  ~MultiHTTPClient(){}

  void callback(uint32_t id, const std::string &content) {
    std::ofstream fp;
    fp.open(savepath, std::ios::out);
    fp << content;
    fp.close();
  }
  std::string savepath;

protected:
  std::string run() {
    std::string content = http.download(url);
    return content;
  }

  std::string url;
  crawl::HTTP http;
private:
  std::function<void (uint32_t, const std::string&)> callback_ref;
};

namespace crawl {
  class Downloader::DownloaderPrivate{
  public:
    DownloaderPrivate(Downloader *_parent)
      : parent(_parent)
    {
    }
    thread_group<std::string> grp;

    Downloader *parent;
  };

  Downloader::Downloader(const std::string &log4cpp_config_path)
    :Executer(log4cpp_config_path)
    , d(new DownloaderPrivate(this))
  {
  }

  Downloader::~Downloader()
  {
    delete d;
  }

  uint32_t Downloader::doExecute() {
    return downloadData();
  }

  uint32_t Downloader::enqueueRequest(const std::string &url, const std::string &savepath) {
    return d->grp.add(new MultiHTTPClient(url, savepath), true);
  }

  void Downloader::wait() {
    d->grp.join();
  }
}
