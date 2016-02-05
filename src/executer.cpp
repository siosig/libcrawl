#include "crawl/executer.hpp"
#include <iostream>
#include <log4cpp/Category.hh>
#include <log4cpp/PropertyConfigurator.hh>

namespace crawl {
  class Executer::ExecuterPrivate {
  public:
    ExecuterPrivate()
      : options("option")
    {
    }
    ~ExecuterPrivate() {

    }
    boost::program_options::options_description options;
    boost::program_options::variables_map vm;
  };

  Executer::Executer(const std::string &log4cpp_config_path)
    :d(new ExecuterPrivate)
  {
    setLogging(log4cpp_config_path);
  }

  Executer::~Executer()
  {
    delete d;
  }


  boost::program_options::variables_map &Executer::parseArg(uint32_t argc, char** argv) {
    boost::program_options::store(boost::program_options::parse_command_line(argc, argv, d->options), d->vm);
    boost::program_options::notify(d->vm);

    if(d->vm.count("help")) {
      std::cout << d->options << std::endl;
    }
    return d->vm;
  }

  uint32_t Executer::execute() {
    uint32_t ret = doExecute();
    (log4cpp::Category::getRoot()).shutdown();
    return ret;
  }

  void Executer::setLogging(const std::string &log4cpp_config_path) {
      log4cpp::PropertyConfigurator::configure(log4cpp_config_path);
  }

  void Executer::setOptions(const boost::program_options::options_description &options) {
    // d->options.clear();
    d->options.add(options);
  }

  const boost::program_options::variables_map &Executer::getOptionValues() const {
    return d->vm;
  }

  void Executer::setOptionValues(const boost::program_options::variables_map &vm) {
    d->vm = vm;
  }


}
