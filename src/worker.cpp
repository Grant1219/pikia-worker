#include <iostream>
#include <boost/thread.hpp>
#include <worker.hpp>

namespace pikia {
    worker::worker (const std::string& _sqlConnectionString, redisContext* _redis, const Beanstalk::Client& _bean)
        : done (true), executor (boost::thread::hardware_concurrency () ), sql (_sqlConnectionString), bean (_bean) {
        this->redis = _redis;
    }

    worker::~worker () {
    }

    void worker::do_work () {
        // reserve a job from beanstalk
        Beanstalk::Job job;

        this->bean.reserve (job);

        if (job) {
            // TODO handle whatever the job is

            this->bean.del (job.id () );
        }
    }
}
