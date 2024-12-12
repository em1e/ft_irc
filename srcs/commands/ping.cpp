#include "Server.hpp"

/*
	Keeps the connection to clients alive
*/
void Server::ping(std::string buf, int fd, int index)
{
	std::cout << "--------------- PING -----------------" << std::endl;
	
	if (!_clients[index])
	{
		std::cerr << "Error: Client " << fd << " not found." << std::endl;
		return;
	}
	std::string ip = buf.substr(5);
	sendResponse("PONG " + ip, fd);
}
