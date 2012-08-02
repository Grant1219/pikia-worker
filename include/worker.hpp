#pragma once

#include <string>
#include <pqxx/pqxx>
#include <hiredis/hiredis.h>
#include <beanstalk/beanstalk.hpp>
#include <task_executor.hpp>

namespace pikia {
    class worker {
        public:
            worker (const std::string& _sqlConnectionString, redisContext* _redis, const Beanstalk::Client& _bean);
            ~worker ();

            void do_work ();
            const bool is_done () const { return done; }

        private:
            bool done;

            task_executor executor;

            pqxx::connection sql;
            redisContext* redis;
            Beanstalk::Client bean;
    };
}
