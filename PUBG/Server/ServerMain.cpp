#include "stdafx.h"
#include "Server.h"

int main(int argc, char* argv[])
{
    try
    {
        //if (argc < 2)
        //{
        //    std::cerr << "Usage: Server <port>\n";
        //}

        boost::asio::io_context ioContext;
        tcp::endpoint endpoint(tcp::v4(), std::atoi("8253")/*std::atoi(argv[1])*/);
        Server s(&ioContext, endpoint);
        ioContext.run();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << '\n';
    }

    return 0;
}
