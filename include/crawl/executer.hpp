#pragma once

#include <string>
#include <list>
#include <boost/program_options.hpp>

namespace crawl {
  class Executer {
  public:
    Executer(const std::string &log4cpp_config_path);
    virtual ~Executer();
    boost::program_options::variables_map &parseArg(uint32_t argc, char** argv);
    uint32_t execute();
    void setLogging(const std::string &log4cpp_config_path);
    void setOptions(const boost::program_options::options_description &options);
    void setOptionValues(const boost::program_options::variables_map &vm);
    const boost::program_options::variables_map &getOptionValues() const;

  protected:
    virtual uint32_t doExecute() { return 0; }

  private:
    class ExecuterPrivate;
    ExecuterPrivate *d;
  };
}
