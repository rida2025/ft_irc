#include "./Authenticator.hpp"
#include "Authenticator.hpp"

Authenticator::Authenticator(/* args */) : _nick(""), _user(""), _realName("")
{

}

Authenticator::~Authenticator()
{
}

std::stringstream	&Authenticator::pushLineToStream(const std::string &line)
{
	this->_stringStream.clear();
	this->_stringStream.str("");
	this->_stringStream.str(line);
	return (this->_stringStream);
}

void Authenticator::toUpper(std::string &str)
{
	int i;

	i = 0;
	while (str[i] != '\0')
	{
		if (islower(static_cast<int> (str[i])))
			str[i] = toupper(static_cast<int> (str[i]));
		i++;
	}
}

void	Authenticator::parsePass(std::string &cmd, std::string &password, std::string &third)
{
	std::getline(this->_stringStream, cmd, ' ');
	skipSpaces(this->_stringStream, password);
	skipSpaces(this->_stringStream, third);
}

int Authenticator::checkPassword(const Server &server)
{
	std::string pass;
	std::string password;
	std::string thirdParam;

	parsePass(pass, password, thirdParam);
	this->toUpper(pass);
	if (password.length() == 0)
	{
		server.sendReply("461", "not enough parameters", this->_client);

		return (false);
	}
	if (server.getPassword().compare(password) == 0)
		return (true);
	server.sendReply("464", "password incorect", this->_client);
	return (false);
}

bool hasUnacceptedChars(const std::string &nick)
{
	for (size_t i = 0; i < nick.size(); i++)
	{
		if (isspace(static_cast<int>(nick[i])))
			return (true);
		if (nick[i] == ':')
			return (true);
		if (nick[i] == '#')
			return (true);
		if (nick[i] == '@')
			return (true);
		if (nick[i] == '&')
			return (true);
	}
	return (false);
}

bool	isNickNameInUse(const std::map<int, Client> &data, const std::string &nick)
{
	std::map<int, Client>::const_iterator  it = data.begin();
	while (it != data.end())
	{
		if (it->second.nickName.compare(nick) == 0)
			return (true);
		it++;
	}
	return (false);
}

/// @brief skip the spaces and asign the text from a ' ' space to another into str parameter
/// @param sstream  a stream of characters 
/// @param str a string to asign the first occurs of non space characters
void skipSpaces(std::stringstream &sstream, std::string &str)
{
	std::getline(sstream, str, ' ');
	while(str.compare("") == 0 && !sstream.fail())
	{
		std::getline(sstream, str, ' ');
	}
}

void	Authenticator::parseNick(std::string &cmd, std::string &nickName, std::string &third)
{
	std::getline(this->_stringStream, cmd, ' ');
	skipSpaces(this->_stringStream, nickName);
	skipSpaces(this->_stringStream, third);
}

int Authenticator::checkNick(Server &server)
{
	std::string firstParam;
	std::string secondParam;
	std::string thirdParam;

	parseNick(firstParam, secondParam, thirdParam);
	if (hasUnacceptedChars(secondParam))
	{
		server.sendReply("432", secondParam + " : Erroneus nickname", this->_client);
		return (false);
	}
	if (secondParam.length() == 0)
	{
		server.sendReply("431", "no nickname given", this->_client);
		return (false);
	}
	if (isNickNameInUse(server.getData(), secondParam))
	{
		server.sendReply("433", secondParam + " : nickname already in use", this->_client);
		return (false);
	}
	this->setNick(secondParam);
	return (true);
}

void	Authenticator::parseUser(std::string &cmd, std::string &user, std::string &hostName, std::string &servName, std::string &realName, std::string &sixthParam)
{
	std::getline(this->_stringStream, cmd, ' ');
	skipSpaces(this->_stringStream, user);
	skipSpaces(this->_stringStream, hostName);
	skipSpaces(this->_stringStream, servName);
	std::getline(this->_stringStream, realName, '\0');
	this->pushLineToStream(realName);
	if (realName.find(':') != std::string::npos && (realName.find(':') == 0 || isspace(realName[realName.find(':') - 1])))
	{
		std::getline(this->_stringStream, realName, ':');
		std::getline(this->_stringStream, realName, '\0');
		std::getline(this->_stringStream, sixthParam, ':');
	}
	else
	{
		skipSpaces(this->_stringStream, realName);
		skipSpaces(this->_stringStream, sixthParam);
	}
}

bool isRealNameValid(const std::string &realName)
{
	int	i;

	i = 0;
	while (realName[i])
	{
		if (!isspace(realName[i]) && !isalpha(realName[i]))
			return (false);
		i++;
	}
	return (true);
}

int Authenticator::checkUser(const Server &server)
{
	std::string firstParam;
	std::string secondParam;
	std::string thirdParam;
	std::string fourthParam;
	std::string fifthParam;
	std::string sixthParam;

	parseUser(firstParam, secondParam, thirdParam, fourthParam, fifthParam, sixthParam);
	this->toUpper(firstParam);
	if (!fifthParam.length() || !fourthParam.length() || !thirdParam.length() || !secondParam.length())
	{
		server.sendReply("461", firstParam + " : not enough parameters", this->_client);
		return (false);
	}
	if (hasUnacceptedChars(secondParam) || hasUnacceptedChars(thirdParam) || hasUnacceptedChars(fourthParam) || !isRealNameValid(fifthParam))
	{
		server.sendReply("468", firstParam + " : Your username is not valid", this->_client);
		return (false);
	}
	this->setUser(("~" + secondParam).c_str());
	this->setRealName(fifthParam);
    return (true);
}

void Authenticator::welcomeMsg(const Server &server)
{
	std::string rplWelcome;
	std::string rplYourhost;
	std::string rplCreated;
	std::string rplMyInfo;

	rplWelcome = "Welcome to FT_IRC  Network, ";
	rplWelcome += this->_client.nickName +"!"+ this->_client.userName + "@" + this->_client.ip;
	server.sendReply("001", rplWelcome, this->_client);
	rplYourhost = "your host is " + this->_client.serverName + ", runing version 1.1 ";
	server.sendReply("002", rplYourhost, this->_client);
	rplCreated = "This server was created " + getDateTime(server.getCreateDate());
	server.sendReply("003", rplCreated, this->_client);
	rplMyInfo = this->_client.ip + " " + this->_client.serverName +", Version: 1.1, User mode: none, Channel modes: o, t, k, i, l !";
	server.sendReply("004", rplMyInfo, this->_client);
}

int Authenticator::checkClientAuthentication(Server &server, Client &client, std::string &line)
{
	Authenticator auth;

	auth._client = client;
	if (line.find('\n') != line.npos)
		line.erase(line.find('\n'));
	auth.pushLineToStream(line);
	for (int i = 0; i < 4 && line[i] != '\0'; i++)
		line[i] = toupper(static_cast<int>(line[i]));
	if (line.compare(0, 5, "PASS ", 0, 5) != 0 && !client._isPassSet)
	{
		server.sendReply("464", "Password required", auth._client);
		return (false);
	}
	if (line.compare(0, 5, "PASS ", 0, 5) == 0)
		client._isPassSet = auth.checkPassword(server);
	else if (line.compare(0, 5, "USER ", 0, 5) == 0)
	{
		if (!client._isUserSet && auth.checkUser(server))
		{
			client._isUserSet = true;
			client.userName = auth._user;
			client.realName = auth._realName;
		}
	}
	else if (line.compare(0, 5, "NICK ", 0, 5) == 0)
	{
		if (!client._isNickSet && auth.checkNick(server))
		{
			client._isNickSet = true;
			client.nickName = auth._nick;
		}
	}
	else if (line.length() > 0)
	{
		server.sendReply("451", "you have not registered", auth._client);
	}
	auth._client = client;
	if (client._isNickSet && client._isPassSet && client._isUserSet)
	{
		client.isAuthenticated = true;
		auth.welcomeMsg(server);
		std::cout << ":" << client.ip << " Authenticated using nick name  :{" << client.nickName << "}" << std::endl;
	}
	return (true);
}

/******************setters*********************/

void Authenticator::setNick(const std::string &nick)
{
	this->_nick = nick;
}

void Authenticator::setUser(const std::string &user)
{
	this->_user = user;
}

void Authenticator::setRealName(const std::string &realName)
{
	this->_realName = realName;
}
