#include <iostream>
#include <string>
#include <csignal>
#include <boost/thread.hpp>
#include <boost/signal.hpp>
#include <worker.hpp>

boost::signal<void ()> shutdownSignal;

void shutdown (int _param) {
    shutdownSignal ();
}

int main (int argc, char** argv) {
    signal (SIGTERM, shutdown);
    signal (SIGINT, shutdown);

    boost::thread_group group;
    size_t hwThreads = boost::thread::hardware_concurrency ();

    for (size_t n = 0; n < hwThreads; n++) {
        boost::shared_ptr<pikia::worker> worker (new pikia::worker ("host=localhost user=pikia password=illutia.is.gay", "localhost", 6379, "localhost", 11300) );

        shutdownSignal.connect (boost::bind (&pikia::worker::shutdown, worker) );
        group.create_thread (boost::bind (&pikia::worker::do_work, worker) );
    }

    group.join_all ();

    return 0;
}
