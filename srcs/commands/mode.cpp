#include "Server.hpp"

void Server::mode(std::string buf, int fd)
{
	std::cout << "--------------- MODE -----------------" << std::endl;
	std::cout << "buffer before : |" << buf << "|"<< std::endl;
	buf.replace(buf.find("INVITE"), 6, "");
	std::cout << "buffer after : |" << buf << "|" << std::endl;

	std::cout << "Client " << fd << " has invited" << std::endl;
}