#pragma once

#include "crawl/executer.hpp"
#include "crawl/db.hpp"
#include <string>
#include <vector>

namespace crawl {
  class Updater : public Executer {
  public:
    Updater(const std::string &log4cpp_config_path);
    virtual ~Updater();

  protected:
    uint32_t doExecute();
    crawl::DB &getDB(const std::string &schema);

    virtual std::vector<std::vector<std::string> > collectData(){}
    virtual uint32_t updateData(const std::vector<std::vector<std::string> > &rows) { return 0; }
  private:
    class UpdaterPrivate;
    UpdaterPrivate *d;
  };
}
