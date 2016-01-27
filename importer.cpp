#include "importer.hpp"

namespace crawl {
  Importer::Importer() {
  }

  Importer::~Importer() {
  }

  void Importer::execute() {
    boost::filesystem::path target(this->target);
    if (! boost::filesystem::exists(target)) {
      return;
    }
    std::list<Table*> rows;
    if (boost::filesystem::is_directory(target)) {
      for ( boost::filesystem::recursive_directory_iterator end, dir(target); dir != end; ++dir ) {
        std::list<Table*> tmp = this->_parse(dir->path());
        rows.insert(rows.end(), tmp.begin(), tmp.end());
      }
    } else {
      std::list<Table*> tmp = this->_parse(target);
      rows.insert(rows.end(), tmp.begin(), tmp.end());
    }

    this->_execute(rows);
  }
}
