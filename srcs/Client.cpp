#include "Client.hpp"

Client::Client(){}

Client::Client(int client_socket, const sockaddr_in& client_addr)
	: _nickname(""), _username(""), _socket(client_socket), _IPaddress(client_addr), _isRegistered(false), _isAuthenticated(false) {}

Client::Client(Client const &a)
{
	_nickname = a._nickname;
	_username = a._username;
	_socket = a._socket;
	_IPaddress = a._IPaddress;
}

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

void Client::sendMessage(const std::string &msg) {
	if (send(_socket, msg.c_str(), msg.length(), 0) == -1) {
		std::cerr << "Error: Failed to send message to client (" << _nickname << ")" << std::endl;
	} else {
		std::cout << "Message sent to " << _nickname << ": " << msg << std::endl;
	}
}
