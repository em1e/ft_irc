#include "Client.hpp"

Client::Client(){}

Client::Client(int client_socket, sockaddr_in client_addr)
	: _nickname(""), _username(""), _socket(client_socket), _IPaddress(client_addr){}

Client::Client(Client const &){}

Client::~Client(){}

Client &Client::operator=(Client const &a)
{
	if (this != &a)
	{
		_IPaddress = a._IPaddress;
		_socket = a._socket;
		_nickname = a._nickname;
		_username = a._username;
	}
	return *this;
}

int Client::getSocket(){ return _socket; }
int Client::getIpAddress(){ return _IPaddress.sin_addr.s_addr; }
std::string Client::getNickname(){ return _nickname; }
std::string Client::getUsername(){ return _username; }

void Client::setSocket(int clientFd){ _socket = clientFd; }
void Client::setIpAddress(sockaddr_in ip){ _IPaddress = ip; }
void Client::setBuffer(std::string buff){ _buffer += buff; }
void Client::setNickname(std::string nick){ _nickname = nick; }
void Client::setUsername(std::string user){ _username = user; }
