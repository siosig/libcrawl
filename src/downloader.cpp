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
    , savepath(_savepath) {
    callback_ref = boost::bind(&MultiHTTPClient::callback, this, _1, _2);
    setCallbackFunction(callback_ref);
  }
  ~MultiHTTPClient(){}

  void callback(uint32_t id, const std::string &content) {
    std::cout << savepath.c_str() << std::endl;
    std::cout << content << std::endl;
    std::ofstream fp;
    fp.open(savepath, std::ios::out);
    fp << content;
    fp.close();
  }
  std::string savepath;

protected:
  std::string run() {
    return http.download(url);
  }

  std::string url;
  crawl::HTTP http;
private:
  boost::function<void (uint32_t, const std::string&)> callback_ref;
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
