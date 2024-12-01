#include "Server.hpp"

/*
	Authenticates user and makes sure the password matches
*/
void Server::pass(std::string buf, int fd, int index)
{
	std::cout << "--------------- PASS -----------------" << std::endl;
	
	if (!_clients[index])
	{
		std::cerr << "Error: Client " << fd << " not found." << std::endl;
		return;
	}

	std::istringstream iss(buf);
	std::string command, password;
	iss >> command >> password;

	if (password != _password)
	{
		std::cout << "Password incorrect" << std::endl;
		sendError("464 : Password incorrect", fd);
		return ;
	}

	std::cout << "client : |" << _clients[index]->getNickname() << "| was authenticated" << std::endl;
	_clients[index]->setIsAuthenticated(true);
}