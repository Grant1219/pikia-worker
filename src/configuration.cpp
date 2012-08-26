#include <iostream>
#include <configuration.hpp>

namespace pikia {
    configuration::configuration (const std::string _configFile)
        : configFile (_configFile) {
        this->setup ();
    }

    void configuration::setup () {
        std::cout << "Loading config file..." << std::endl;

        // read the config file values
        boost::property_tree::ptree pt;
        boost::property_tree::ini_parser::read_ini (this->configFile, pt);
        this->pt = pt;
    }
}
