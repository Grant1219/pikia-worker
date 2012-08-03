#include <iostream>
#include <boost/thread.hpp>
#include <worker.hpp>

namespace pikia {
    worker::worker (std::string _sqlString, std::string _redisHost, uint16_t _redisPort, std::string _beanHost, uint16_t _beanPort)
    : done (false), sql (_sqlString), bean (_beanHost, _beanPort) {
        this->redis = redisConnect (_redisHost.c_str (), _redisPort);

        if (this->redis->err) {
            throw std::runtime_error (this->redis->errstr);
        }
    }

    worker::~worker () {
        std::cout << "Worker at " << this << " shutting down!" << std::endl;
        redisFree (this->redis);
    }

    void worker::do_work () {
        std::cout << "Worker at " << this << " doing work!" << std::endl;

        Beanstalk::Job job;

        while (!done) {
            // reserve a job from beanstalk
            if (this->bean.reserve (job, 0) ) {
                // TODO handle whatever the job is

                this->bean.del (job.id () );
            }
            else
                boost::this_thread::yield ();
        }
    }
}
