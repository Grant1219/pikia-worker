#include <csignal>
#include <memory>
#include <functional>
#include <iostream>
#include <string>
#include <boost/program_options.hpp>
#include <boost/thread.hpp>
#include <boost/signal.hpp>
#include <configuration.hpp>
#include <worker.hpp>

boost::signal<void ()> shutdownSignal;
boost::signal<void ()> reloadSignal;
std::shared_ptr<pikia::configuration> config;

void shutdown (int _param) {
    std::cout << "Received shutdown signal!" << std::endl;
    shutdownSignal ();
}

void reload (int _param) {
    std::cout << "Received reload signal!" << std::endl;
    config->reload ();
    reloadSignal ();
}

int main (int argc, char** argv) {
    boost::program_options::options_description desc ("Allowed options");
    desc.add_options ()
        ("help,h", "Show a help message")
        ("config,c", boost::program_options::value<std::string>(), "Pass a config file");

    boost::program_options::variables_map vm;
    boost::program_options::store (boost::program_options::parse_command_line (argc, argv, desc), vm);
    boost::program_options::notify (vm);

    if (vm.count ("help") ) {
        std::cout << desc << std::endl;
        return 1;
    }

    if (vm.count ("config") ) {
        signal (SIGTERM, shutdown);
        signal (SIGINT, shutdown);
        signal (SIGUSR1, reload);

        config = std::shared_ptr<pikia::configuration> (new pikia::configuration (vm["config"].as<std::string> () ) );

        boost::thread_group group;
        size_t hwThreads = boost::thread::hardware_concurrency ();

        for (size_t n = 0; n < hwThreads; n++) {
            boost::shared_ptr<pikia::worker> worker (new pikia::worker (config) );

            reloadSignal.connect (std::bind (&pikia::worker::reload, worker) );
            shutdownSignal.connect (std::bind (&pikia::worker::shutdown, worker) );
            group.create_thread (std::bind (&pikia::worker::do_work, worker) );
        }

        group.join_all ();
    }
    else {
        std::cout << "No config file was given (see worker --help)" << std::endl;
        return 1;
    }

    return 0;
}
