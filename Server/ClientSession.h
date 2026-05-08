#ifndef CLIENTSESSION_H
#define CLIENTSESSION_H

#include <boost/asio.hpp>
#include <memory>
#include <string>
#include <iostream>

class UserManager;
class MessageRouter;

using boost::asio::ip::tcp;

class ClientSession : public std::enable_shared_from_this<ClientSession> {
public:
    ClientSession(tcp::socket socket, UserManager& um, MessageRouter& router)
        : socket_(std::move(socket)), userManager_(um), router_(router) {}

    void start() {
        read();
    }

    void send(const std::string& msg) {
        boost::asio::async_write(socket_,
            boost::asio::buffer(msg + "\n"),
            [](auto, auto) {});
    }

private:
    void read() {
        auto self = shared_from_this();

        boost::asio::async_read_until(socket_, buffer_, '\n',
            [this, self](auto ec, std::size_t) {
                if (!ec) {
                    std::istream is(&buffer_);
                    std::string msg;
                    std::getline(is, msg);

                    handle(msg);
                    read();
                }
            });
    }

    void handle(const std::string& raw);

    tcp::socket socket_;
    boost::asio::streambuf buffer_;

    std::string username_;
    UserManager& userManager_;
    MessageRouter& router_;
};

#endif

