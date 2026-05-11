#include "server.h"
#include <boost/asio.hpp>

int main()
{
    boost::asio::io_context io;

    Server server(io, 12345);

    io.run();

    return 0;
}
