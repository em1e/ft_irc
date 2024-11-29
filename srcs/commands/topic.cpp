#include "Server.hpp"

void Server::topic(std::string buf, int fd)
{
	std::cout << "--------------- TOPIC -----------------" << std::endl;
	std::cout << "buffer : |" << buf << "|"<< std::endl;
	(void)fd;
}