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
    void startAccept();
    void handleClient(std::shared_ptr<boost::asio::ip::tcp::socket> socket);

    boost::asio::ip::tcp::acceptor acceptor_;

    std::map<std::string, std::shared_ptr<boost::asio::ip::tcp::socket>> clients;
};

#endif
