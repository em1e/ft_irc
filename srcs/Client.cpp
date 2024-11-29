#include "Client.hpp"

Client::Client(){}

Client::Client(int client_socket, sockaddr_in client_addr)
	: _nickname(""), _username(""), _socket(client_socket), _IPaddress(client_addr){}

// Client::Client(Client const &a)
// {
// 	_nickname = a._nickname;
// 	_username = a._username;
// 	_socket = a._socket;
// 	_IPaddress = a._IPaddress;
// }

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

