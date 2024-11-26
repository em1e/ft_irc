#include "Server.hpp"

void Server::capLs(std::string buf, std::string response, int fd)
{
	std::cout << "--------------- CAP LS -----------------" << std::endl;
	response += "CAP LS :\r\n";
	send(fd, response.c_str(), response.length(), 0);
	std::cout << "Client " << fd << " sent CAP LS command." << std::endl;
	std::cout << "buffer: " << buf << std::endl;
	if (buf.find("CAP LS 302") != std::string::npos)
		buf.replace(buf.find("CAP LS 302"), 10, "");
	if (buf.find("\n") != std::string::npos)
		buf.replace(buf.find("\n"), 1, "");
	if (buf.find("\r") != std::string::npos)
		buf.replace(buf.find("\r"), 1, "");
	response = ":localhost 001 A Message Flooder was here! ";
	response += buf;
	std::cout << "response: " << response;
	std::cout << "buffer: " << buf << std::endl;
	if (buf.find("JOIN") == 0)
	{
		response = ":localhost 001 A Message Flooder was here! JOIN :\r\n";
		send(fd, response.c_str(), response.length(), 0);
		std::cout << "Client " << fd << " sent join msg" << std::endl;
	}
}