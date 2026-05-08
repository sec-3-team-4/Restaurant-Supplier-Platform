#include <iostream>
#include <boost/asio.hpp>
#include "AsioServer.h"

int main() {
    std::cout << "Server starting...\n";

    boost::asio::io_context io;
    AsioServer server(io, 12345);

    io.run();
}
