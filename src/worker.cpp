#include <iostream>
#include <boost/thread.hpp>
#include <worker.hpp>

namespace pikia {
    worker::worker (int _workerId, std::string _sqlString, std::string _redisHost, uint16_t _redisPort, std::string _beanHost, uint16_t _beanPort)
    : done (false), workerId (_workerId), sql (_sqlString), bean (_beanHost, _beanPort) {
        this->redis = redisConnect (_redisHost.c_str (), _redisPort);

        if (this->redis->err) {
            throw std::runtime_error (this->redis->errstr);
        }
    }

    worker::~worker () {
        redisFree (this->redis);
    }

    void worker::do_work () {
        std::cout << "Worker in thread " << boost::this_thread::get_id () << " doing work!" << std::endl;

        redisReply* reply;
        Beanstalk::Job job;

        do {
            // reserve a job from beanstalk
            if (this->bean.reserve (job, 0) ) {
                // TODO handle whatever the job is

                this->bean.del (job.id () );
            }
            else
                boost::this_thread::yield ();

            // check if this worker needs to shut down
            reply = (redisReply*)redisCommand (this->redis, "EXISTS worker:%i:shutdown", this->workerId);
            if (reply->type == REDIS_REPLY_INTEGER && reply->integer == 1) {
                std::cout << "Worker in thread " << boost::this_thread::get_id () << " got shutdown signal!" << std::endl;
                done = true;
            }
            freeReplyObject (reply);
        } while (!done);
    }
}
