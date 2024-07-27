#include "./Client.hpp"
#include "Client.hpp"

Client::Client(/* args */)
{
	isAuthenticated = false;
	_isPassSet = false;
	_isNickSet = false;
	_isUserSet = false;
	bufferString = "";
	nickName = "-";
	userName = "";
	realName = "";
	hostName = "";
	serverName = "ircserver";
	fd = -2;
	ip = "";
}

Client::Client(const Client &obj)
{
	if (this != &obj)
		*this = obj;
}

const Client &Client::operator=(const Client &obj)
{
	if (this != &obj)
	{
		this->inside_channel	= obj.inside_channel;
		this->_isPassSet		= obj._isPassSet;
		this->_isNickSet		= obj._isNickSet;
		this->_isUserSet		= obj._isUserSet;
		this->bufferString		= obj.bufferString;
		this->isAuthenticated	= obj.isAuthenticated;
		this->fd				= obj.fd;
		this->hostName  		= obj.hostName;
		this->ip				= obj.ip;
		this->nickName			= obj.nickName;
		this->realName			= obj.realName;
		this->serverName		= obj.serverName;
		this->userName			= obj.userName;
	}
	return (*this);
}

Client::~Client()
{
}