#pragma once

#include <string>
#include <pqxx/pqxx>
#include <hiredis/hiredis.h>
#include <beanstalk/beanstalk.hpp>

namespace pikia {
    class connection_bundle {
        public:
            connection_bundle (std::string _sqlString, std::string _redisHost, uint16_t _redisPort, std::string _beanHost, uint16_t _beanPort) {
                this->sql = new pqxx::connection (_sqlString);
                this->redis = redisConnect (_redisHost.c_str (), _redisPort);
                this->bean = new Beanstalk::Client (_beanHost, _beanPort);

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

        public:
            pqxx::connection* sql;
            Beanstalk::Client* bean;
            redisContext* redis;
    };
}
