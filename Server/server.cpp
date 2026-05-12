#include "server.h"
#include "UserManager.h"
#include "JsonValidator.h"
#include "MessageRouter.h"

#include <iostream>
#include <array>
#include <memory>
#include <string>
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
awaitable<void> handleClient(tcp::socket socket)
{
    char data[1024];

    while (true)
    {
        auto [ec, length] = co_await socket.async_read_some(
            boost::asio::buffer(data),
            as_tuple(use_awaitable)
        );

        if (ec)
        {
            std::cout << "Client disconnected" << std::endl;
            co_return;
        }

        std::string msg(data, length);

        std::cout << "\nReceived:\n" << msg << std::endl;

        if (!validator.isValid(msg))
        {
            std::cout << "INVALID MESSAGE\n";
            continue;
        }

        std::cout << "VALID MESSAGE\n";

        std::string result = router.route(msg);
        std::cout << result << std::endl;

        auto json = nlohmann::json::parse(msg);

        std::string type = json.value("type", "");
        std::string sender = json.value("sender", "");
        std::string receiver = json.value("receiver", "");

        std::string text = "";
        if (json.contains("data") && json["data"].contains("text"))
            text = json["data"]["text"];

        std::cout << "\nType: " << type << std::endl;
        std::cout << "Sender: " << sender << std::endl;
        std::cout << "Receiver: " << receiver << std::endl;

        if (!text.empty())
            std::cout << "Message: " << text << std::endl;

        // LOGIN
        if (type == "login_request")
        {
            userManager.addUser(sender);

            std::cout << "\nLOGIN SUCCESS: " << sender << std::endl;

            nlohmann::json response;
            response["type"] = "login_response";
            response["status"] = "success";
            response["message"] = "Login successful";

            std::string reply = response.dump() + "\n";

            co_await boost::asio::async_write(
                socket,
                boost::asio::buffer(reply),
                use_awaitable
            );

            std::cout << "Login response sent\n";
        }

        // LOGOUT
        else if (type == "logout")
        {
            userManager.removeUser(sender);
            std::cout << "LOGOUT: " << sender << std::endl;
        }

        // CHAT
        else if (type == "chat_message")
        {
            std::cout << "\nCHAT from " << sender << " to " << receiver << std::endl;

            std::string forwardMsg = sender + ": " + text;

            co_await boost::asio::async_write(
                socket,
                boost::asio::buffer(forwardMsg + "\n"),
                use_awaitable
            );

            std::cout << "Message forwarded\n";
        }

        if (userManager.isOnline(sender))
        {
            std::cout << sender << " is ONLINE" << std::endl;
        }
    }
}

awaitable<void> listener(boost::asio::io_context& io)
{
    tcp::acceptor acceptor(io, tcp::endpoint(tcp::v4(), 54321));

    std::cout << "Server running on port 54321\n";

    while (true)
    {
        tcp::socket socket = co_await acceptor.async_accept(use_awaitable);

        std::cout << "Client connected\n";

        co_spawn(io, handleClient(std::move(socket)), detached);
    }
}
