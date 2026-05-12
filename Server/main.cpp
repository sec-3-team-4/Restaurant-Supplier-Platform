#include "server.h"
#include <boost/asio.hpp>

int main()
{
    boost::asio::io_context io;

    co_spawn(io, listener(io), boost::asio::detached);

    io.run();
}
