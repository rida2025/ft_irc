#ifndef CLIENT_HPP
# define CLIENT_HPP
# include <iostream>
# include <vector>
# include <map>

class Client
{
    private:
        /* data */
    public:
        std::vector<std::string> inside_channel;
        bool        isAuthenticated;
        bool		_isPassSet;
		bool		_isNickSet;
		bool		_isUserSet;
        std::string bufferString;
        std::string nickName;
        std::string userName;
        std::string realName;
        std::string hostName;
        std::string serverName;
        std::string ip;
        int fd;

        Client(/* args */);
        Client(const Client &obj);
        const Client &operator=(const Client &obj);
        ~Client();
};
#endif
