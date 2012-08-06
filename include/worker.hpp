#pragma once

#include <atomic>
#include <string>
#include <boost/shared_ptr.hpp>
#include <connection_bundle.hpp>
#include <job_buffer.hpp>
#include <job_dispatcher.hpp>

namespace pikia {
    class worker {
        public:
            worker (const std::string& _configFile);
            ~worker ();

            void do_work ();
            void reload ();
            void shutdown ();

        private:
            void setup_worker ();
            void close_worker ();

        private:
            int realmId;
            std::string configFile;
            std::atomic_bool done;
            std::atomic_bool reloading;

            boost::shared_ptr<connection_bundle> bundle;
            boost::shared_ptr<job_dispatcher> dispatcher;
    };
}
