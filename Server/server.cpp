#include "server.h"
#include <iostream>
#include <array>
#include <nlohmann/json.hpp>

#include <boost/asio.hpp>

using boost::asio::ip::tcp;
using boost::asio::awaitable;
using boost::asio::co_spawn;
using boost::asio::detached;
using boost::asio::use_awaitable;
using boost::asio::as_tuple;

// handle one client
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
    }
}

// accept connections
awaitable<void> listener()
{
    auto ex = co_await boost::asio::this_coro::executor;

    tcp::acceptor acceptor(ex, tcp::endpoint(tcp::v4(), 1234));

    std::cout << "Server started on port 1234" << std::endl;

    while (true)
    {
        tcp::socket socket(ex);

        auto [ec] = co_await acceptor.async_accept(socket, as_tuple(use_awaitable));

        if (!ec)
        {
            std::cout << "Client connected" << std::endl;
            co_spawn(ex, handleClient(std::move(socket)), detached);
        }
    }
}

// main entry
int main()
{
    boost::asio::io_context io;

    co_spawn(io, listener(), detached);

    io.run();
}
