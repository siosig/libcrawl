#include "crawl/executer.hpp"
#include "crawl/importer.hpp"
#include <log4cpp/Category.hh>
#include <log4cpp/PropertyConfigurator.hh>

namespace crawl {
  class Importer::ImporterPrivate {
  public:
    ImporterPrivate()
    {
    }
    ~ImporterPrivate() {
    }
  };

  Importer::Importer(const std::string &log4cpp_config_path)
    :Executer(log4cpp_config_path)
    ,d(new ImporterPrivate)
  {
  }

  Importer::~Importer()
  {
    delete d;
  }

  uint32_t Importer::doExecute() {
    boost::filesystem::path t(Executer::getOptionValues()["target"].as<std::string>());
    if (! boost::filesystem::exists(t)) {
      return -1;
    }
    std::vector<std::vector<std::string>> rows;
    if (boost::filesystem::is_directory(t)) {
      uint32_t cnt = 0;
      uint32_t exec_cnt = 0;
      for ( boost::filesystem::recursive_directory_iterator end, dir(t); dir != end; ++dir ) {
        if (boost::filesystem::is_directory( dir->path())) {
          continue;
        }
        std::vector<std::vector<std::string>> tmp = parseHtml(dir->path());
        rows.insert(rows.end(), tmp.begin(), tmp.end());
        if (!(++cnt % 100)) {
          exec_cnt += importData(rows);
          rows.clear();
        }
      }
      exec_cnt += importData(rows);
      return exec_cnt;
    } else {
      std::vector<std::vector<std::string>> tmp = parseHtml(t);
      rows.insert(rows.end(), tmp.begin(), tmp.end());
      return importData(rows);
    }
  }
}
