#pragma once

#include <string>
#include <memory>
#include <pqxx/pqxx>
#include <hiredis/hiredis.h>
#include <beanstalk/beanstalk.hpp>
#include <reloadable.hpp>
#include <configuration.hpp>

namespace pikia {
    class connection_bundle : public reloadable {
        public:
            connection_bundle (std::shared_ptr<configuration> _config);
            ~connection_bundle ();

        private:
            void setup ();
            void close ();

        public:
            std::shared_ptr<configuration> config;

            pqxx::lazyconnection* sql;
            Beanstalk::Client* bean;
            redisContext* redis;
    };
}
