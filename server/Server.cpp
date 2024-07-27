#include "Server.hpp"

Server::Server() : _password("DefaultPassword"), _port(0) , _createDate(std::time(NULL))
{

}

Server::Server(const Server &obj)
{
	if (this != &obj)
	{
		*this = obj;
	}
}

Server::Server(const std::string &password, const std::string &port) : _password("DefaultPassword"), _port(0), _createDate(std::time(NULL))
{
	int	port_nbr;

	if (!isValidPassword(password))
		throw PasswordNotAcceptedException();
	port_nbr = convertStringToInt(port);
	if (!isValidPort(port_nbr))
		throw PortNotAcceptedException;
	setPassword(password);
	setPort(port_nbr);

}

const Server &Server::operator=(const Server &obj)
{
	if (this != &obj)
	{
		this->_password = obj._password;
		this->_port = obj._port;
		this->_createDate = obj._createDate;
	}
	return (*this);
}

Server::~Server()
{

}

void Server::runServer(const std::string &password, const std::string &port)
{
	try
	{
		int	readyFds;
		Server serv(password, port);
		serv.openSocket();
		serv.bindSocket();
		serv.listenSocket();
		signal(SIGPIPE, SIG_IGN);
		std::cout << "---------Server is Running successfully In port "<< port << " with password {"<< password << "}---------" << std::endl;
		while (1)
		{
			try
			{
				readyFds = serv.checkFdsForNewEvents();
				if (readyFds == 0)
					continue ;
				if (serv._socketsFds[0].revents & POLLIN)
				{
					serv.acceptClientSocket();
					serv.saveClientData();
					readyFds--;
					if (readyFds <= 0)
						continue ;
				}
				for (size_t i = 1; i < serv._socketsFds.size() && readyFds >= 0; i++)
				{
					if (!(serv._socketsFds[i].revents & (POLLRDNORM)))
						continue ;
					serv.clientWithEvent(i);
					readyFds--;
				}
			}
			catch (std::exception &e)
			{
				std::cerr << "Error: " << std::endl;
				std::cerr << e.what() << std::endl;
			};
		}
	}
	catch (std::exception &e)
	{
		std::cerr << "Error: " << std::endl;
		std::cerr << e.what() << std::endl;
	};
}

void Server::sendMsg(const std::string &msg, int fd) const
{
	std::string message;
	message = msg + "\r\n";
	if (send(fd, message.c_str(), message.length(), 0) == -1)
	{
		std::cout << "Error :" << std::endl;
		std::cerr << "thier is an error occured while trying to send a message" << std::endl;
	}
}

void Server::sendReply(const std::string &numeric, const std::string &parameter, const Client &client) const
{
	std::string reply = ":" + client.serverName + " " + numeric + " " + client.nickName + " :" + parameter;
	this->sendMsg(reply, client.fd);
}

void Server::addData(int fd, const Client &client)
{
	std::pair<int, Client> data;
	data.first = fd;
	data.second = client;
	this->_data.insert(data);
}

bool	hasSpace(const std::string &str)
{
	size_t i;

	i = 0;
	while (i < str.length())
	{
		if (std::isspace(static_cast<unsigned char>(str[i])))
			return (true);
		i++;
	}
	return (false);
}

bool	Server::isValidPassword(const std::string &password) const
{
	if (password.length() < 4)
		return (false); /*password has to be more than 4 char in length */
	if (hasSpace(password))
		return (false); /*password does not accept whitespaces*/
	return (true);
}

int	convertStringToInt(const std::string &str)
{
	std::stringstream sstream(str);
	int nbr;

	sstream >> nbr;
	if (sstream.fail() || sstream.bad())
		return (-1);
	return (nbr);
}

bool Server::isValidPort(const int port) const
{
	if (port >= 1024 && port <= 65535)
		return (true); /*port has to be between 1024 and 65535*/
	return (false);
}

/******************************** setters **********************************/

void	Server::setPassword(const std::string &password)
{
	this->_password = password;
}

void	Server::setPort(const int &port)
{
	this->_port = port;
}

/******************************** getters **********************************/

const std::string &Server::getPassword(void) const
{
	return (this->_password);
}

const int &Server::getPort(void) const
{
	return (this->_port);
}

const int &Server::getClientFd(void) const
{
   return (this->_clientFd);
}

const std::time_t &Server::getCreateDate(void) const
{
    return (this->_createDate);
}

std::map<int, Client> &Server::getData(void)
{
    return (this->_data);
}

void Server::openSocket(void)
{
	int	option;

	option = 1;
	this->_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_serverSocket == -1)
		throw OpenServerSocketException();
	if (fcntl(_serverSocket, F_SETFL, O_NONBLOCK) == -1)
		throw NonBlockServerSocketException();
	if (setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)) == -1)
		throw SocketCouldNotReuseAddrException();
}

int Server::bindSocket(void)
{
	_serverLen = sizeof(_serverAddr);
	bzero(&_serverAddr, _serverLen);
	_serverAddr.sin_family = AF_INET;
	_serverAddr.sin_port = htons(_port);
	_serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(_serverSocket, (sockaddr *) &_serverAddr, _serverLen) == -1)
		throw CouldNotBindServerSocketException();
    return 0;
}

int Server::listenSocket(void)
{
	pollfd tmp;

	tmp.fd = _serverSocket;
	tmp.events = POLLIN | POLLRDNORM;
	tmp.revents = 0;
	if (listen(_serverSocket, OPEN_MAX) == -1)
		throw CouldNotListenServerSocketException();
	_socketsFds.push_back(tmp);
    return 0;
}

int Server::checkFdsForNewEvents(void)
{
	int readyFds;

	readyFds = poll(_socketsFds.begin().base(), _socketsFds.size(), 0);
	if (readyFds == -1)
		throw PollCheckFdsEventsException();
    return (readyFds);
}

int Server::acceptClientSocket(void)
{
	_clientLen = sizeof(_clientAddr);
	bzero(&_clientAddr, _clientLen);
	_clientFd = accept(_serverSocket, (sockaddr *) &_clientAddr, &_clientLen);
	if (_clientFd == -1)
		throw NewClientNotAcceptedException();
	if (_socketsFds.size() >= OPEN_MAX)
		std::cerr << "Error Too many Clients "<< std::endl;
    return 0;
}

int Server::saveClientData(void)
{
	Client client;
	pollfd tmp;
	char hostname[_SC_HOST_NAME_MAX + 1];

	tmp.fd = _clientFd;
	tmp.events = POLLIN | POLLRDNORM;
	tmp.revents = 0;
	if (gethostname(hostname, _SC_HOST_NAME_MAX))
	{
		std::cout << ":" << inet_ntoa(_clientAddr.sin_addr) << " QUIT :Client disconnected" << std::endl;
		close(_clientFd);
		return (1);
	}
	client.fd = _clientFd;
	client.hostName = hostname;
	client.ip = inet_ntoa(_clientAddr.sin_addr);
	addData(_clientFd, client);
	_socketsFds.push_back(tmp);
	std::cout << ":" << client.ip << " Client Connected" << std::endl;
    return 0;
}

int	Server::deleteClientFd(std::vector<pollfd>::iterator position)
{
	this->_socketsFds.erase(position);
	return (0);
}

void	Server::deleteClient(int fd)
{
	this->getData().erase(fd);
}

void	Server::applyQuitCommand(int clientIndex)
{
	std::map<int, Client>::iterator it = this->getData().find(this->_socketsFds[clientIndex].fd);
	if (it == this->getData().end())
		return ;
	Client	&client = it->second;
	sendReply("ERROR", "QUIT : closing connection...", client);
	close(client.fd);
	QUIT_COMMAND(client, this->_channels);
	deleteClient(client.fd);
	deleteClientFd(this->_socketsFds.begin() + clientIndex);
}

void Server::clientCloseConnextion(const int clientIndex)
{
	std::map<int, Client>::iterator it = this->getData().find(this->_socketsFds[clientIndex].fd);

	if (it == this->getData().end())
		return ;
	Client	&client = it->second;
	std::cout << "Client " << client.ip << " with nickname ["<< client.nickName <<"] has closed the connection"  << std::endl;
	applyQuitCommand(clientIndex);
}

bool Server::recieveMsg(const int fd, std::string &line)
{
	char		msg[1024];

	bzero(msg, 1024);
	if (recv(fd, msg, 1024, MSG_DONTWAIT) == -1 && (errno != EAGAIN && errno != EWOULDBLOCK))
	{
		std::cerr << "Error : thier is an error while recieving a message with errno = " << errno << " from file desc nbr = " << fd << std::endl;
		return (false);
	}
	line = msg;
	return (true);
}

void Server::clientWithEvent(const int clientIndex)
{
	std::string line;
	std::map<int, Client>::iterator it = this->getData().find(this->_socketsFds[clientIndex].fd);

	if (it == this->getData().end())
		return ;
	Client		&triggeredClient = it->second;
	if (triggeredClient.fd == -2)
		return ;
	if (!recieveMsg(triggeredClient.fd, line))
		return ;
	if (!this->handleCtrlD(line, triggeredClient.bufferString))
		return ;
	if (line.length() > 512)
	{
		this->sendReply("417", "Input line was too long", triggeredClient);
		return ;
	}
	else if (line.length() <= 0)
	{
		this->clientCloseConnextion(clientIndex);
		return ;
	}
	handleMultiLineFeed(triggeredClient, line, clientIndex);
}

bool Server::isAuthenticationCommand(std::string line)
{
	for (int i = 0; i < 4 && line[i] != '\0'; i++)
		line[i] = toupper(static_cast<int>(line[i]));
	if (line.compare(0, 5, "PASS ", 0, 5) == 0)
		return (true);
	if (line.compare(0, 5, "NICK ", 0, 5) == 0)
		return (true);
	if (line.compare(0, 5, "USER ", 0, 5) == 0)
		return (true);
	if (line.compare("PASS") == 0)
		return (true);
	if (line.compare("NICK") == 0)
		return (true);
	if (line.compare("USER") == 0)
		return (true);
	return (false);	
}

bool Server::isQuitCommand(std::string line) const
{
	for (int i = 0; i < 4 && line[i] != '\0'; i++)
		line[i] = toupper(static_cast<int>(line[i]));
	if (line.compare(0, 5, "QUIT ", 0, 5) == 0)
		return (true);
	if (line.compare("QUIT") == 0)
		return (true);
	return (false);
}

std::stringstream	&Server::pushLineToStream(const std::string &line)
{
	this->_stringStream.clear();
	this->_stringStream.str("");
	this->_stringStream.str(line);
	return (this->_stringStream);
}

void Server::handleMultiLineFeed(Client &client, std::string &line, int clientIndex)
{
	int countNewLine;

	countNewLine = countNewLines(line);
	this->pushLineToStream(line);
	while (countNewLine-- >= 1)
	{
		line.clear();
		std::getline(_stringStream, line, '\n');
		if (line.find('\r') != line.npos)
			line.erase(line.find('\r'));
		if (line.compare("\n") == 0 || line.length() == 0)
			continue;
		if (client.isAuthenticated == false)
			Authenticator::checkClientAuthentication(*this, client, line);
		else
		{
			if (isAuthenticationCommand(line))
				this->sendReply("462", "You may not reregister", client);
			else if (isQuitCommand(line))
				clientCloseConnextion(clientIndex);
			else
				IS_COMMAND_VALID(client.fd, line, this->getData(), this->_channels);
		}
	}
}


int	countNewLines(const std::string &line)
{
	int	nbr;

	nbr = 0;
	for (size_t i = 0; i < line.length(); i++)
	{
		if (line[i] == '\n')
			nbr++;
	}
	return (nbr);
}

bool Server::handleCtrlD(std::string &line, std::string &bufferString)
{
	if (line.length() == 0)
		return (true);
	if (line.find('\n') == std::string::npos)
	{
		bufferString.append(line);
		return (false);
	}
	if(bufferString.length() > 0)
	{
		bufferString.append(line);
		line.clear();
		line = "";
		line.append(bufferString);
		bufferString.clear();
		bufferString = "";
	}
	return (true);
}

const std::string	getHourMinute(const time_t &t)
{
	std::string hour;
	std::string minute;
	const std::tm* localTime;

	localTime = std::localtime(&t);
	hour = convertNbrToSting<int>(localTime->tm_hour);
	minute = convertNbrToSting<int>(localTime->tm_min);

	return (hour + ":" + minute);
}

const std::string	getYearMounthDay(const time_t &t)
{
	std::string year;
	std::string month;
	std::string day;
	const std::tm* localTime;

	localTime = std::localtime(&t);
	year = convertNbrToSting<int>(localTime->tm_year + 1900);
	month = convertNbrToSting<int>(localTime->tm_mon + 1);
	day = convertNbrToSting<int>(localTime->tm_mday);
	return (day + " " + month + " " + year);
}

const std::string	getDateTime(const time_t &t)
{
	return (getYearMounthDay(t) + " at " + getHourMinute(t));
}

/********************************Exceptions*********************************/

const char *Server::PortNotAcceptedException::what() const throw()
{
	return "Port not accepted , the port has to be an integer between 1024 and 65535.";
}
