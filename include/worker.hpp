#pragma once

#include <atomic>
#include <boost/shared_ptr.hpp>
#include <connection_bundle.hpp>
#include <job_parser.hpp>
#include <job_dispatcher.hpp>

namespace pikia {
    class worker {
        public:
            worker (int _realmId, std::string _sqlString, std::string _redisHost, uint16_t _redisPort, std::string _beanHost, uint16_t _beanPort);
            ~worker ();

            void do_work ();
            void reload ();
            void shutdown () { this->done = true; }

        private:
            int realmId;
            std::atomic_bool done;

            boost::shared_ptr<connection_bundle> bundle;
            boost::shared_ptr<job_dispatcher> dispatcher;
    };
}
