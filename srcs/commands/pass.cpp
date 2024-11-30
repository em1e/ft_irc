#include "Server.hpp"

/*
	Authenticates user and makes sure the password matches
*/
void Server::pass(std::string buf, int fd, int index)
{
	std::cout << "--------------- PASS -----------------" << std::endl;
	
	// check if client exists
	if (!_clients[index])
	{
		std::cerr << "Error: Client " << fd << " not found." << std::endl;
		return;
	}

	std::istringstream iss(buf);
	std::string command, password;
	iss >> command >> password;
	std::cout << "password : |" << password << "|" << std::endl;

	if (password != _password)
	{
		std::cout << "Password does not match" << std::endl;
		sendError("464: :Password incorrect", fd);
		return ;
	}

	std::cout << "client : |" << _clients[index]->getNickname() << "| was authenticated" << std::endl;
	_clients[index]->setIsAuthenticated(true);
}