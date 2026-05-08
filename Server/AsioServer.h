#ifndef ASIOSERVER_H
#define ASIOSERVER_H

#include <boost/asio.hpp>
#include <memory>

#include "UserManager.h"
#include "MessageRouter.h"
#include "ClientSession.h"

using boost::asio::ip::tcp;

class AsioServer {
public:
    AsioServer(boost::asio::io_context& io, int port)
        : acceptor_(io, tcp::endpoint(tcp::v4(), port)),
          router_(userManager_)
    {
        accept();
    }

private:
    void accept() {
        acceptor_.async_accept([this](boost::system::error_code ec, tcp::socket socket) {
            if (!ec) {
                std::make_shared<ClientSession>(
                    std::move(socket),
                    userManager_,
                    router_
                )->start();
            }
            accept();
        });
    }

    tcp::acceptor acceptor_;

    UserManager userManager_;
    MessageRouter router_;
};

#endif

