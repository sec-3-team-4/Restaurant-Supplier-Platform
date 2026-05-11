#include "server.h"
#include <iostream>
#include <array>
#include <nlohmann/json.hpp>

using boost::asio::ip::tcp;

Server::Server(boost::asio::io_context& io, int port)
    : acceptor_(io, tcp::endpoint(tcp::v4(), port))
{
    std::cout << "Server started on port " << port << std::endl;
    startAccept();
}

void Server::startAccept()
{
    auto socket = std::make_shared<tcp::socket>(acceptor_.get_executor());

    acceptor_.async_accept(*socket,
        [this, socket](boost::system::error_code ec)
        {
            if (!ec)
            {
                std::cout << "Client connected" << std::endl;
                handleClient(socket);
            }

            startAccept();
        });
}

void Server::handleClient(std::shared_ptr<tcp::socket> socket)
{
    auto buffer = std::make_shared<std::array<char, 1024>>();

    socket->async_read_some(
        boost::asio::buffer(*buffer),
        [this, socket, buffer](boost::system::error_code ec, std::size_t length)
        {
            if (!ec)
            {
                std::string msg(buffer->data(), length);

                std::cout << "\nReceived:\n" << msg << std::endl;

                try
                {
                    auto json = nlohmann::json::parse(msg);

                    std::cout << "Type: " << json.value("type", "") << std::endl;
                    std::cout << "Sender: " << json.value("sender", "") << std::endl;
                    std::cout << "Receiver: " << json.value("receiver", "") << std::endl;
                }
                catch (...)
                {
                    std::cout << "Invalid JSON" << std::endl;
                }

                handleClient(socket);
            }
            else
            {
                std::cout << "Client disconnected" << std::endl;
            }
        });
}
