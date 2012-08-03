#include <iostream>
#include <string>
#include <boost/thread.hpp>
#include <worker.hpp>

int main (int argc, char** argv) {
    int workerId;
    if (argc == 2) {
        try {
            workerId = std::stoi (argv[1]);
        }
        catch (std::exception) {
            std::cout << "Error: The worker ID passed was invalid." << std::endl;
            return 1;
        }
    }
    else {
        std::cout << "Error: You must pass a worker ID" << std::endl;
        return 1;
    }

    boost::thread_group group;
    size_t hwThreads = boost::thread::hardware_concurrency ();

    for (size_t n = 0; n < hwThreads; n++) {
        boost::shared_ptr<pikia::worker> worker (new pikia::worker (workerId, "host=localhost user=pikia password=illutia.is.gay", "127.0.0.1", 6379, "127.0.0.1", 11300) );

        group.create_thread (boost::bind (&pikia::worker::do_work, worker) );
    }

    group.join_all ();

    return 0;
}
