#pragma once

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <reloadable.hpp>

namespace pikia {
    class configuration : public reloadable {
        public:
            configuration (std::string _configFile);

            template <typename T>
            T get (std::string _prop) {
                return this->pt.get<T> (_prop);
            }

        private:
            void setup ();

        private:
            std::string configFile;
            boost::property_tree::ptree pt;
    };
}
