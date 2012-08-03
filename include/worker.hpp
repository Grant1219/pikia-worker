#pragma once

#include <pqxx/pqxx>
#include <atomic>
#include <hiredis/hiredis.h>
#include <beanstalk/beanstalk.hpp>

namespace pikia {
    class worker {
        public:
            worker (std::string _sqlString, std::string _redisHost, uint16_t _redisPort, std::string _beanHost, uint16_t _beanPort);
            ~worker ();

            void do_work ();
            void shutdown () { this->done = true; }

        private:
            std::atomic_bool done;

            pqxx::connection sql;
            Beanstalk::Client bean;
            redisContext* redis;
    };
}
