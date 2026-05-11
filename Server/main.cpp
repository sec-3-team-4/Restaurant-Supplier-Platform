#include "server.h"
#include <boost/asio.hpp>

int main()
{
    boost::asio::io_context io;

    Server server(io, 1234);

    io.run();

    return 0;
}
