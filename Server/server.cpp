#include "server.h"
#include "UserManager.h"
#include "JsonValidator.h"
#include "MessageRouter.h"

#include <iostream>
#include <array>
#include <memory>
#include <string>
#include <nlohmann/json.hpp>

using boost::asio::ip::tcp;

UserManager userManager;
JsonValidator validator;
MessageRouter router;

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
            if (ec)
            {
                std::cout << "Client disconnected" << std::endl;
                return;
            }

            std::string msg(buffer->data(), length);

            std::cout << "\nReceived:\n" << msg << std::endl;

            if (!validator.isValid(msg))
            {
                std::cout << "INVALID MESSAGE\n";
                handleClient(socket);
                return;
            }

            try
            {
                auto json = nlohmann::json::parse(msg);

                std::string type = json.value("type", "");
                std::string sender = json.value("sender", "");
                std::string receiver = json.value("receiver", "");

                std::string text;

                if (json.contains("data") && json["data"].contains("text"))
                    text = json["data"]["text"];

                std::cout << "\nType: " << type << std::endl;
                std::cout << "Sender: " << sender << std::endl;
                std::cout << "Receiver: " << receiver << std::endl;

                if (!text.empty())
                    std::cout << "Message: " << text << std::endl;

                if (type == "login_request")
                {
                    userManager.addUser(sender);
                    clients[sender] = socket;

                    std::cout << "\nLOGIN SUCCESS: " << sender << std::endl;

                    nlohmann::json response;
                    response["type"] = "login_response";
                    response["status"] = "success";
                    response["message"] = "Login successful";

                    std::string reply = response.dump() + "\n";

                    boost::asio::write(*socket, boost::asio::buffer(reply));

                    std::cout << "Login response sent\n";
                }
                else if (type == "logout")
                {
                    userManager.removeUser(sender);
                    clients.erase(sender);

                    std::cout << "LOGOUT: " << sender << std::endl;
                }
                else if (type == "chat_message")
                {
                    std::cout << "\nCHAT from " << sender << " to " << receiver << std::endl;

                    if (clients.find(receiver) != clients.end())
                    {
                        std::string forwardMsg = sender + ": " + text;

                        boost::asio::write(
                            *clients[receiver],
                            boost::asio::buffer(forwardMsg + "\n"));

                        std::cout << "Message forwarded to " << receiver << std::endl;
                    }
                    else
                    {
                        std::cout << "Receiver not online: " << receiver << std::endl;
                    }
                }

                if (userManager.isOnline(sender))
                    std::cout << sender << " is ONLINE" << std::endl;
            }
            catch (const std::exception& e)
            {
                std::cout << "JSON ERROR: " << e.what() << std::endl;
            }

            handleClient(socket);
        });
}
