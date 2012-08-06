#include <iostream>
#include <boost/thread.hpp>
#include <job_context.hpp>
#include <worker.hpp>

namespace pikia {
    worker::worker (int _realmId, std::string _sqlString, std::string _redisHost, uint16_t _redisPort, std::string _beanHost, uint16_t _beanPort)
    : realmId (_realmId), done (false), bundle (new connection_bundle (_sqlString, _redisHost, _redisPort, _beanHost, _beanPort) ), dispatcher (new job_dispatcher (bundle) ) {
        // setup the beanstalk tubes to use and watch
        this->bundle->bean->ignore ("default");
        this->bundle->bean->watch ("worker:" + std::to_string (this->realmId) );
        this->bundle->bean->use ("realm:" + std::to_string (this->realmId) );

        // TEST
        job_buffer buf;
        buf.write_int<uint32_t> (1);
        buf.write_int<int32_t> (8);
        buf.write_string ("I'm from a job!");
        job_context context;
        context.id = buf.read_int<uint32_t> ();
        context.buf = buf;
        this->dispatcher->dispatch_job (context);
    }

    worker::~worker () {
        std::cout << "Worker at " << this << " shutting down!" << std::endl;
    }

    void worker::do_work () {
        std::cout << "Worker at " << this << " doing work!" << std::endl;

        Beanstalk::Job job;

        while (!done) {
            // reserve a job from beanstalk
            if (this->bundle->bean->reserve (job, 0) ) {
                // get the data out of the job into a buffer for parsing
                job_context context;
                job_buffer buf (job);

                context.id = buf.read_int<uint32_t> ();
                context.buf = buf;

                this->dispatcher->dispatch_job (context);

                // TODO put the reply

                this->bundle->bean->del (job.id () );
            }
            else
                boost::this_thread::yield ();
        }
    }
}
