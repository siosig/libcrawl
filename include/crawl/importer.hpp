#pragma once

#include "crawl/executer.hpp"
#include <string>
#include <list>

namespace crawl {
  class Importer : public Executer {
  public:
    Importer(const std::string &log4cpp_config_path);
    virtual ~Importer();

  protected:
    uint32_t doExecute();

    virtual std::vector<std::vector<std::string> > parseHtml(const std::string& html_file_path){}
    virtual uint32_t importData(const std::vector<std::vector<std::string> > &rows) { return 0; }
  private:
    class ImporterPrivate;
    ImporterPrivate *d;
  };
}
