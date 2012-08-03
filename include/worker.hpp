#pragma once

#include <pqxx/pqxx>
#include <hiredis/hiredis.h>
#include <beanstalk/beanstalk.hpp>

namespace pikia {
    class worker {
        public:
            worker (int _workerId, std::string _sqlString, std::string _redisHost, uint16_t _redisPort, std::string _beanHost, uint16_t _beanPort);
            ~worker ();

            void do_work ();

        private:
            bool done;
            int workerId;

            pqxx::connection sql;
            Beanstalk::Client bean;
            redisContext* redis;
    };
}
