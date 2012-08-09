#pragma once

#include <atomic>
#include <string>
#include <memory>
#include <reloadable.hpp>
#include <configuration.hpp>
#include <connection_bundle.hpp>
#include <job_buffer.hpp>
#include <job_dispatcher.hpp>
#include <script_manager.hpp>

namespace pikia {
    class worker : public reloadable {
        public:
            worker (std::shared_ptr<configuration> _config);
            ~worker ();

            void do_work ();
            void reload ();
            void shutdown ();

        private:
            void setup ();
            void close ();

        private:
            uint32_t realmId;
            std::atomic_bool done;
            std::atomic_bool reloading;

            std::shared_ptr<configuration> config;
            std::shared_ptr<connection_bundle> bundle;
            std::shared_ptr<job_dispatcher> dispatcher;
            std::shared_ptr<script_manager> scriptManager;
    };
}
