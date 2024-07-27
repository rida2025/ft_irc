#ifndef SERVER_HPP
# define SERVER_HPP
# ifndef OPEN_MAX
#  define OPEN_MAX PF_MAX
# endif
# include <iostream>
# include <sstream>
# include <limits.h>
# include <iostream>
# include <unistd.h>
# include <cstring>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <netinet/in.h>
# include <netdb.h>
# include <fcntl.h>
# include <signal.h>
# include <ctime>
#include <cerrno>
# include <poll.h>
# include <vector>
# include <map>
# include "./Authenticator.hpp"
# include "./../exception/Exceptions.hpp"
# include "./../Commands/Commands.hpp"
# include "./Client.hpp"

class Server
{
	private:
		std::string				_password;
		int						_port;
		int						_serverSocket;
		int						_clientFd;
		std::vector<pollfd>		_socketsFds;
		std::map<int, Client>	_data;
		socklen_t				_clientLen, _serverLen;
		struct sockaddr_in		_clientAddr, _serverAddr;
		std::stringstream		_stringStream;
		std::time_t				_createDate;
		std::vector<Channels>	_channels;
	/************************* Methods ************************/
	public:
		static void	runServer(const std::string &password, const std::string &port);
		void sendMsg(const std::string &msg, int fd) const;
		void sendReply(const std::string &numeric, const std::string &parameter, const Client &client) const;
		/*getter member functions*/
		const std::string &getPassword(void) const;
		const int &getPort(void) const;
		const int &getClientFd(void) const;
		const std::time_t &getCreateDate(void) const;
		std::map<int, Client> &getData(void);
	protected:
		Server(const std::string &passwd, const std::string &port);
		~Server();
		void addData(int fd, const Client &client);
		/*setter member functions*/
		void setPassword(const std::string &);
		void setPort(const int &);
	private:
		Server();
		Server(const Server &obj); 
		const Server &operator=(const Server &obj);
		bool	isValidPassword(const std::string &) const;
		bool	isValidPort(const int) const;
		void	openSocket(void);
		int		bindSocket(void);
		int		listenSocket(void);
		/// @brief runing poll() system call to check if there are new ready file descriptors 
		/// @return the number of ready file descriptors
		int		checkFdsForNewEvents(void);
		int		acceptClientSocket(void);
		int		saveClientData(void);
        int		deleteClientFd(std::vector<pollfd>::iterator __position);
        void	deleteClient(int fd);
        void	applyQuitCommand(int clientIndex);
        void	clientCloseConnextion(const int clientIndex);
        bool	recieveMsg(const int fd, std::string &line);
		void	clientWithEvent(const int clientIndex);
		bool	isAuthenticationCommand(std::string line);
		bool	isQuitCommand(std::string line) const;
		std::stringstream &pushLineToStream(const std::string &line);
		void	handleMultiLineFeed(Client &client, std::string &line, int clientIndex);
		/// @brief
		/// @param line the message sended by a client
		/// @param bufferString the previous message sended by a client and it does not has a new line
		/// @return true if the line has a new line char on it
		bool	handleCtrlD(std::string &line, std::string &bufferString);
        /*EXCEPTIONS : INNER CLASSES*/
		class	PortNotAcceptedException : public std::exception
		{
			virtual const char *what() const throw();
		} PortNotAcceptedException;

};

int					convertStringToInt(const std::string &str);
bool				hasSpace(const std::string &str);
int					countNewLines(const std::string &line);
template <typename T>
const std::string convertNbrToSting(T nbr)
{
    std::stringstream sstream;

	sstream << nbr;
	return (sstream.str());
}
const std::string	getHourMinute(const time_t &t);
const std::string	getYearMounthDay(const time_t &t);
const std::string	getDateTime(const time_t &t);
#endif

