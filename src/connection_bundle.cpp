#include <connection_bundle.hpp>

namespace pikia {
    connection_bundle::connection_bundle (std::shared_ptr<configuration> _config)
        : config (_config) {
        this->setup ();
    }

    connection_bundle::~connection_bundle () {
        this->close ();
    }

    void connection_bundle::setup () {
        this->sql = new pqxx::lazyconnection ("host=" + this->config->get<std::string> ("postgres.host") \
                                              + " user=" + this->config->get<std::string> ("postgres.user") \
                                              + " password=" + this->config->get<std::string> ("postgres.pass") );

        this->bean = new Beanstalk::Client (this->config->get<std::string> ("beanstalk.host"),
                                            this->config->get<uint16_t> ("beanstalk.port") );

        this->redis = redisConnect (this->config->get<std::string> ("redis.host").c_str (),
                                    this->config->get<uint16_t> ("redis.port") );

        if (this->redis->err) {
            throw std::runtime_error (this->redis->errstr);
        }
    }

    void connection_bundle::close () {
        delete this->sql;
        delete this->bean;
        redisFree (this->redis);
    }
}
