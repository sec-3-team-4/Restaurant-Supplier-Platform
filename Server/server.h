#ifndef SERVER_H
#define SERVER_H

#include <boost/asio.hpp>
#include <memory>
#include <map>
#include <string>

class Server
{
public:
    Server(boost::asio::io_context& io, int port);

private:
    boost::asio::ip::tcp::acceptor acceptor_;
};

#endif
