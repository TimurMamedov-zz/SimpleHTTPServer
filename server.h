#ifndef HTTP_SERVER_HPP
#define HTTP_SERVER_HPP

#include <boost/asio.hpp>
#include <string>
#include "connection.h"
#include "connection_manager.h"
#include "request_handler.h"

#include "thread_pool.h"

namespace http {
namespace server {

/*!
    \brief The top-level class of the HTTP server.
*/
class server
{
public:
    server(const server&) = delete;
    server& operator=(const server&) = delete;

    /// Construct the server to listen on the specified TCP address and port, and
    /// serve up files from the given directory.
    explicit server(const std::string& address, const std::size_t& port,
                    const std::string& doc_root);

    /// Run the server's io_context loop.
    void run();

private:
    /// Perform an asynchronous accept operation.
    void do_accept();

    /// Wait for a request to stop the server.
    void do_await_stop();

    /// The io_context used to perform asynchronous operations.
    boost::asio::io_service io_context_;

    /// The signal_set is used to register for process termination notifications.
    boost::asio::signal_set signals_;

    /// Acceptor used to listen for incoming connections.
    boost::asio::ip::tcp::acceptor acceptor_;

    /// The connection manager which owns all live connections.
    connection_manager connection_manager_;

    /// The handler for all incoming requests.
    request_handler request_handler_;

    boost::asio::ip::tcp::socket socket;

    thread_pool& pool;
};

} // namespace server
} // namespace http

#endif // HTTP_SERVER_HPP
