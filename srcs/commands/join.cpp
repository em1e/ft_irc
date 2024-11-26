#include "Server.hpp"

void Server::join(std::string response, int fd)
{
	std::cout << "--------------- JOIN -----------------" << std::endl;
			std::cout << "Client " << fd << " sent join message" << std::endl;
			response += "JOIN :\r\n";
			send(fd, response.c_str(), response.length(), 0);
}