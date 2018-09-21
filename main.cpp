#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/program_options.hpp>
#include "server.h"

namespace po = boost::program_options;

int main(int argc, char* argv[])
{
    try
    {
        // Check command line arguments.
        if (argc != 7)
        {
            std::cerr << "Usage: SimpleHTTPServer -h <ip> -p <port> -d <directory>\n";
            std::cerr << "  For IPv4, try:\n";
            std::cerr << "    receiver 0.0.0.0 80 .\n";
            std::cerr << "  For IPv6, try:\n";
            std::cerr << "    receiver 0::0 80 .\n";
            return 1;
        }

        po::options_description desc("General options");
        desc.add_options()
                ("address,h", po::value<std::string>(), "IP address")
                ("port,p", po::value<std::size_t>(), "port")
                ("directory,d", po::value<std::string>(), "directory");

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        std::cout << vm["address"].as<std::string>() << std::endl;
        std::cout << vm["port"].as<std::size_t>() << std::endl;
        std::cout << vm["directory"].as<std::string>() << std::endl;
        // Initialise the server.
        http::server::server s(vm["address"].as<std::string>(),
                vm["port"].as<std::size_t>(),
                vm["directory"].as<std::string>());

        // Run the server until stopped.
        s.run();
    }
    catch (std::exception& e)
    {
        std::cerr << "exception: " << e.what() << "\n";
    }

    return 0;
}
