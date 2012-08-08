#pragma once

#include <atomic>
#include <string>
#include <memory>
#include <connection_bundle.hpp>
#include <job_buffer.hpp>
#include <job_dispatcher.hpp>
#include <script_manager.hpp>

namespace pikia {
    struct worker_config {
        uint32_t realmId;

        std::string sqlConnectionString;

        std::string redisHost;
        uint16_t redisPort;

        std::string beanHost;
        uint16_t beanPort;

        std::string scriptsPath;
    };

    class worker {
        public:
            worker (const std::string& _configFile);
            ~worker ();

            void do_work ();
            void reload ();
            void shutdown ();

        private:
            void load_config ();
            void setup_worker ();
            void close_worker ();

        private:
            std::string configFile;
            worker_config config;

            std::atomic_bool done;
            std::atomic_bool reloading;

            std::shared_ptr<connection_bundle> bundle;
            std::shared_ptr<job_dispatcher> dispatcher;
            std::shared_ptr<script_manager> scriptManager;

        private:
    };
}
