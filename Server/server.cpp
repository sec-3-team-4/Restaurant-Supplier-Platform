#include "server.h"
#include "UserManager.h"
#include "JsonValidator.h"
#include "MessageRouter.h"

#include <iostream>
#include <array>
#include <memory>
#include <string>
#include <map>
#include <nlohmann/json.hpp>

#include <boost/asio.hpp>

using boost::asio::ip::tcp;
using boost::asio::awaitable;
using boost::asio::co_spawn;
using boost::asio::detached;
using boost::asio::use_awaitable;
using boost::asio::as_tuple;

// managers
UserManager userManager;
JsonValidator validator;
MessageRouter router;

// store connected clients
std::map<std::string, std::shared_ptr<tcp::socket>> clients;

// handle each client
awaitable<void> handleClient(std::shared_ptr<tcp::socket> socket)
{
    char data[1024];

    while (true)
    {
        // read message from client
        auto [ec, length] = co_await socket->async_read_some(
            boost::asio::buffer(data),
            as_tuple(use_awaitable)
        );

        if (ec)
        {
            std::cout << "Client disconnected\n";
            co_return;
        }

        std::string msg(data, length);

        std::cout << "\nReceived:\n" << msg << std::endl;

        // validate json
        if (!validator.isValid(msg))
        {
            std::cout << "INVALID MESSAGE\n";
            continue;
        }

        std::cout << "VALID MESSAGE\n";

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

            // login request
            if (type == "login_request")
            {
                userManager.addUser(sender);
                clients[sender] = socket;

                std::cout << "LOGIN SUCCESS: " << sender << std::endl;

                nlohmann::json response;
                response["type"] = "login_response";
                response["status"] = "success";
                response["message"] = "Login successful";

                // send login response
                co_await boost::asio::async_write(
                    *socket,
                    boost::asio::buffer(response.dump() + "\n"),
                    use_awaitable
                );

                std::cout << "LOGIN RESPONSE SENT\n";
            }

            // logout request
            else if (type == "logout")
            {
                userManager.removeUser(sender);
                clients.erase(sender);

                std::cout << "LOGOUT: " << sender << std::endl;
            }

            // chat message
            else if (type == "chat_message")
            {
                std::cout << "CHAT from " << sender << " to " << receiver << std::endl;

                if (clients.find(receiver) != clients.end())
                {
                    std::string forwardMsg = sender + ": " + text;

                    // send message to receiver
                    co_await boost::asio::async_write(
                        *clients[receiver],
                        boost::asio::buffer(forwardMsg + "\n"),
                        use_awaitable
                    );

                    std::cout << "Message forwarded to " << receiver << std::endl;
                }
                else
                {
                    std::cout << "Receiver not online: " << receiver << std::endl;
                }
            }

            // online check
            if (userManager.isOnline(sender))
                std::cout << sender << " is ONLINE" << std::endl;
        }
        catch (const std::exception& e)
        {
            std::cout << "JSON ERROR: " << e.what() << std::endl;
        }
    }
}

// accept clients and start handler
awaitable<void> listener(tcp::acceptor acceptor)
{
    auto ex = co_await boost::asio::this_coro::executor;

    std::cout << "Server started on port 1234\n";

    while (true)
    {
        auto socket = std::make_shared<tcp::socket>(ex);

        auto [ec] = co_await acceptor.async_accept(*socket, as_tuple(use_awaitable));

        if (!ec)
        {
            std::cout << "Client connected\n";
            co_spawn(ex, handleClient(socket), detached);
        }
    }
}

// start server
Server::Server(boost::asio::io_context& io, int port)
    : acceptor_(io, tcp::endpoint(tcp::v4(), port))
{
    co_spawn(io, listener(std::move(acceptor_)), detached);
}
