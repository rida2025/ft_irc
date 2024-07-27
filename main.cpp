#include "./server/Server.hpp"

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cerr << "Error:"<< std::endl;
        std::cerr << "The valid format is : (./ircserv <port> <password>)." << std::endl;
        return (1);
    }
    Server::runServer(argv[2], argv[1]);
    return (0);
}
