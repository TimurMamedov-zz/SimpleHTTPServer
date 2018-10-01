#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/program_options.hpp>
#include <thread>
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

#if !defined(WIN32) && !defined(YOUR_IOS_MACRO_HERE)
        // создаем потомка
        auto pid = fork();

        if (pid == -1) // если не удалось запустить потомка
        {
            // выведем на экран ошибку и её описание
            std::cout << "Error: Start Daemon failed \n";
            return -1;
        }
        else if (!pid) // если это потомок
        {
            // данный код уже выполняется в процессе потомка
            // разрешаем выставлять все биты прав на создаваемые файлы,
            // иначе у нас могут быть проблемы с правами доступа
            umask(0);

            // создаём новый сеанс, чтобы не зависеть от родителя
            setsid();

            // переходим в корень диска, если мы этого не сделаем, то могут быть проблемы.
            // к примеру с размантированием дисков
            chdir("/");

            // Initialise the server.
            http::server::server s(vm["address"].as<std::string>(),
                    vm["port"].as<std::size_t>(),
                    vm["directory"].as<std::string>());

            // Run the server until stopped.
            std::thread t([&s](){ s.run(); });
            s.run();
            t.join();
        }
        else // если это родитель
        {
            // завершим процес, т.к. основную свою задачу (запуск демона) мы выполнили
            return 0;
        }
#elif defined(WIN32)
        // Initialise the server.
        http::server::server s(vm["address"].as<std::string>(),
                vm["port"].as<std::size_t>(),
                vm["directory"].as<std::string>());

        // Run the server until stopped.
        std::thread t([&s](){ s.run(); });
        s.run();
        t.join();
#endif
    }
    catch (std::exception& e)
    {
        std::cerr << "exception: " << e.what() << "\n";
    }

    return 0;
}
