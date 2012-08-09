#include <iostream>
#include <boost/thread.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <job_context.hpp>
#include <worker.hpp>

namespace pikia {
    worker::worker (std::shared_ptr<configuration> _config)
        : done (false), reloading (false),
        config (_config),
        bundle (new connection_bundle (_config) ),
        dispatcher (new job_dispatcher () ),
        scriptManager (new script_manager (_config, dispatcher) ) {
        this->setup ();
    }

    worker::~worker () {
        std::cout << "Worker at " << this << " shutting down!" << std::endl;
        this->close ();
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

                if (this->dispatcher->dispatch_job (context) ) {
                    // TODO put the reply
                    this->bundle->bean->del (job.id () );
                }
                else {
                    // failed, release the job and let another worker try
                    this->bundle->bean->release (job);
                }
            }
            else
                boost::this_thread::yield ();
        }
    }

    void worker::reload () {
        this->reloading = true;

        this->close ();
        // TODO readd c++ event handlers?
        this->scriptManager->reload ();
        this->bundle->reload ();
        this->setup ();

        this->reloading = false;
    }

    void worker::shutdown () {
        done = true;
    }

    void worker::setup () {
        this->realmId = config->get<uint32_t> ("realm.id");

        // setup the beanstalk tubes to use and watch
        this->bundle->bean->ignore ("default");
        this->bundle->bean->watch ("worker:" + std::to_string (this->realmId) );
        this->bundle->bean->use ("realm:" + std::to_string (this->realmId) );
    }

    void worker::close () {
        // callbacks must be destroyed before lua is unloaded because they contain references to lua objects
        this->dispatcher->clear_handlers ();
    }
}
