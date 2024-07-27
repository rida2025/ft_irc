#ifndef AUTHENTICATOR_HPP
# define AUTHENTICATOR_HPP
# include "./Server.hpp"
# include "./Client.hpp"
# include <iostream>

class Server;

class Authenticator
{
	private:
		/* data */
		std::stringstream _stringStream;
		std::string _nick;
		std::string _user;
		std::string _realName;
		Client _client;
	/****************************Methods*****************************/
	public:
		Authenticator(/* args */);
		~Authenticator();
        static int checkClientAuthentication(Server &server, Client &client, std::string &line);
	private:
        std::stringstream &pushLineToStream(const std::string &line);
		void		toUpper(std::string &str);
        void 		parsePass(std::string &cmd, std::string &password, std::string &third);
        int			checkPassword(const Server &obj);
        void		parseNick(std::string &cmd, std::string &nickName, std::string &third);
		int			checkNick(Server &obj);
        void 		parseUser(std::string &cmd, std::string &user, std::string &hostName, std::string &servName, std::string &realName, std::string &sixthParam);
		int			checkUser(const Server &obj);
		void		welcomeMsg(const Server &server);

		/*setters*/
		void		setNick(const std::string &nick);
		void		setUser(const std::string &user);
		void		setRealName(const std::string &realName);

		/*getters*/
};
bool	isNickNameInUse(const std::map<int, Client> &data, const std::string &nick);
void	skipSpaces(std::stringstream &sstream, std::string &str);
bool	hasUnacceptedChars(const std::string &nick);

#endif
