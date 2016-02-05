#include "crawl/executer.hpp"
#include "crawl/updater.hpp"
#include <log4cpp/Category.hh>
#include <log4cpp/PropertyConfigurator.hh>

namespace crawl {
  class Updater::UpdaterPrivate {
  public:
    UpdaterPrivate()
    {
    }
    ~UpdaterPrivate() {
    }
    crawl::DB db;
  };

  Updater::Updater(const std::string &log4cpp_config_path)
    :Executer(log4cpp_config_path)
    , d(new UpdaterPrivate)
  {
  }

  Updater::~Updater()
  {
    delete d;
  }

  crawl::DB &Updater::getDB(const std::string &schema) {
    d->db.setSchema(schema);
    return d->db;
  }

  uint32_t Updater::doExecute() {
    std::vector<std::vector<std::string>> rows = collectData();
    uint32_t exec_cnt = 0;
    d->db.begin();
    exec_cnt = updateData(rows);
    d->db.commit();
    return exec_cnt;
  }
}
