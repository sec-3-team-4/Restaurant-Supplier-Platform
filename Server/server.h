#ifndef SERVER_H
#define SERVER_H

#include <boost/asio.hpp>
#include <memory>

class Server
{
public:
    Server(boost::asio::io_context& io, int port);

private:
    void startAccept();
    void handleClient(std::shared_ptr<boost::asio::ip::tcp::socket> socket);

    boost::asio::ip::tcp::acceptor acceptor_;
};

#endif
