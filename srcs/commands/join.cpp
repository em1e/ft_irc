#include "Server.hpp"

void Server::join(std::string response, int fd)
{
	// handle /join command (check if !_clients[index].get_nickname().empty())
		// else handle it as client joining for the first time

	std::cout << "--------------- JOIN -----------------" << std::endl;
			std::cout << "Client " << fd << " sent join message" << std::endl;
			response += "JOIN :\r\n";
			send(fd, response.c_str(), response.length(), 0);
}