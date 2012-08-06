#include <iostream>
#include <boost/thread.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <job_context.hpp>
#include <worker.hpp>

namespace pikia {
    worker::worker (const std::string& _configFile)
        : configFile (_configFile), done (false), reloading (false),
        bundle (new connection_bundle () ), dispatcher (new job_dispatcher () ) {
        this->setup_worker ();
        // TEST
        /*
        job_buffer buf;
        buf.write_int<uint32_t> (1);
        buf.write_int<int32_t> (8);
        buf.write_string ("I'm from a job!");
        job_context context;
        context.id = buf.read_int<uint32_t> ();
        context.buf = buf;
        this->dispatcher->dispatch_job (context);
        */
    }

    worker::~worker () {
        std::cout << "Worker at " << this << " shutting down!" << std::endl;
    }

    void worker::do_work () {
        std::cout << "Worker at " << this << " doing work!" << std::endl;

        Beanstalk::Job job;

        while (!this->done) {
            // done do anything while this worker is reloading
            while (this->reloading)
                boost::this_thread::yield ();

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

    void worker::reload () {
        this->reloading = true;

        this->close_worker ();
        this->dispatcher->reload ();
        this->setup_worker ();

        this->reloading = false;
    }

    void worker::shutdown () {
        done = true;
    }

    void worker::setup_worker () {
        std::cout << "Loading config file..." << std::endl;

        // read the config file values
        boost::property_tree::ptree pt;
        boost::property_tree::ini_parser::read_ini (this->configFile, pt);

        std::string sqlString = "host=" + pt.get<std::string> ("postgres.host") \
                                 + " user=" + pt.get<std::string> ("postgres.user") \
                                 + " password=" + pt.get<std::string> ("postgres.pass");

        std::string redisHost = pt.get<std::string> ("redis.host");
        uint16_t redisPort = pt.get<uint16_t> ("redis.port");
        std::string beanHost = pt.get<std::string> ("beanstalk.host");
        uint16_t beanPort = pt.get<uint16_t> ("beanstalk.port");

        this->realmId = pt.get<int> ("realm.id");

        // connection to the servers
        std::cout << "Connecting to beanstalk, postgres, redis..." << std::endl;
        this->bundle->connect (sqlString, redisHost, redisPort, beanHost, beanPort);

        // setup the beanstalk tubes to use and watch
        this->bundle->bean->ignore ("default");
        this->bundle->bean->watch ("worker:" + std::to_string (this->realmId) );
        this->bundle->bean->use ("realm:" + std::to_string (this->realmId) );
    }

    void worker::close_worker () {
        this->bundle->disconnect ();
    }
}
