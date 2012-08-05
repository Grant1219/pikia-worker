#include <iostream>
#include <fstream>
#include <string>
#include <csignal>
#include <boost/program_options.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/thread.hpp>
#include <boost/signal.hpp>
#include <worker.hpp>

boost::signal<void ()> shutdownSignal;
boost::signal<void ()> reloadSignal;

void shutdown (int _param) {
    std::cout << "Received shutdown signal!" << std::endl;
    shutdownSignal ();
}

void reload (int _param) {
    std::cout << "Received reload signal!" << std::endl;
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
        boost::property_tree::ptree pt;
        boost::property_tree::ini_parser::read_ini (vm["config"].as<std::string> (), pt);

        std::string sqlString = "host=" + pt.get<std::string> ("postgres.host") \
                                 + " user=" + pt.get<std::string> ("postgres.user") \
                                 + " password=" + pt.get<std::string> ("postgres.pass");

        std::string redisHost = pt.get<std::string> ("redis.host");
        uint16_t redisPort = pt.get<uint16_t> ("redis.port");
        std::string beanHost = pt.get<std::string> ("beanstalk.host");
        uint16_t beanPort = pt.get<uint16_t> ("beanstalk.port");

        int realmId = pt.get<int> ("realm.id");

        signal (SIGTERM, shutdown);
        signal (SIGINT, shutdown);
        signal (SIGUSR1, reload);

        boost::thread_group group;
        size_t hwThreads = boost::thread::hardware_concurrency ();

        for (size_t n = 0; n < hwThreads; n++) {
            boost::shared_ptr<pikia::worker> worker (new pikia::worker (realmId, sqlString, redisHost, redisPort, beanHost, beanPort) );

            shutdownSignal.connect (boost::bind (&pikia::worker::shutdown, worker) );
            group.create_thread (boost::bind (&pikia::worker::do_work, worker) );
        }

        group.join_all ();
    }
    else {
        std::cout << "No config file was given (see worker --help)" << std::endl;
        return 1;
    }

    return 0;
}
