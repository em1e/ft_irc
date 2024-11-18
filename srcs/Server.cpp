#include "Server.hpp"

Server::Server(){}
Server::Server(const std::string &port, const std::string &password)
	: _port(port), _password(password){
		// add smth here
}

Server::Server(Server const &){}
Server::~Server(){}
Server &Server::operator=(Server const &a){ return *this; }