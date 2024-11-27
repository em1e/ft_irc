#include "Server.hpp"

void Server::invite(std::string buf, int fd)
{
	std::cout << "--------------- INVITE -----------------" << std::endl;
	std::cout << "buffer before : |" << buf << "|"<< std::endl;
	buf.replace(buf.find("INVITE"), 6, "");
	std::cout << "buffer after : |" << buf << "|" << std::endl;

	std::cout << "Client " << fd << " has invited" << std::endl;
}