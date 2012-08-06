#pragma once

#include <string>
#include <pqxx/pqxx>
#include <hiredis/hiredis.h>
#include <beanstalk/beanstalk.hpp>

namespace pikia {
    class connection_bundle {
        public:
            connection_bundle () : connected (false) {}
            connection_bundle (std::string _sqlString, std::string _redisHost, uint16_t _redisPort, std::string _beanHost, uint16_t _beanPort) : connected (true) {
                this->sql = new pqxx::lazyconnection (_sqlString);
                this->bean = new Beanstalk::Client (_beanHost, _beanPort);
                this->redis = redisConnect (_redisHost.c_str (), _redisPort);

                if (this->redis->err) {
                    throw std::runtime_error (this->redis->errstr);
                }
            }

            connection_bundle (const connection_bundle& _bundle) {
                this->sql = _bundle.sql;
                this->redis = _bundle.redis;
                this->bean = _bundle.bean;
            }

            ~connection_bundle () {
                delete this->sql;
                delete this->bean;
                redisFree (this->redis);
            }

            void connect (std::string _sqlString, std::string _redisHost, uint16_t _redisPort, std::string _beanHost, uint16_t _beanPort) {
                if (!this->connected) {
                    this->sql = new pqxx::lazyconnection (_sqlString);
                    this->bean = new Beanstalk::Client (_beanHost, _beanPort);
                    //this->bean->connect (_beanHost, _beanPort);
                    this->redis = redisConnect (_redisHost.c_str (), _redisPort);

                    if (this->redis->err) {
                        throw std::runtime_error (this->redis->errstr);
                    }
                }
            }

            void disconnect () {
                if (this->connected) {
                    delete this->sql;
                    delete this->bean;
                    //this->bean->disconnect();
                    redisFree (this->redis);
                }
            }

        public:
            bool connected;

            pqxx::lazyconnection* sql;
            Beanstalk::Client* bean;
            redisContext* redis;
    };
}
