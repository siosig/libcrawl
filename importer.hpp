#ifndef CRAWL_IMPORTER_H
#define CRAWL_IMPORTER_H

#include <string>
#include <list>
#include <boost/filesystem.hpp>
#include "table.hpp"

namespace crawl {
  class Importer {
  public:
    Importer();
    virtual ~Importer();
    void execute();
    std::string target;
  protected:
    virtual void _execute(std::list<Table*>){}
    virtual std::list<Table*> _parse(boost::filesystem::path){}
  private:

  };
}

#endif //CRAWL_IMPORTER_H
