#pragma once

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>

namespace pikia {
    class task_executor {
        public:
            task_executor (size_t _hint): io_service (_hint), work (io_service) {
                for (size_t n = 0; n < _hint; n++)
                    this->pool.create_thread (boost::bind (&boost::asio::io_service::run, &io_service) );
            }

            ~task_executor () {
                this->io_service.stop ();
                this->pool.join_all ();
            }

            template<typename T> void submit (T task) {
                this->io_service.post (task);
            }

        private:
            boost::thread_group pool;
            boost::asio::io_service io_service;
            boost::asio::io_service::work work;
    };
}
