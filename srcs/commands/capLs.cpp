#include "Server.hpp"

void Server::capLs(std::string buf, int fd)
{
	std::cout << "--------------- CAP LS -----------------" << std::endl;
	sendResponse("CAP LS : ", fd);
	std::cout << "Client " << fd << " sent CAP LS command." << std::endl;
	std::cout << "buffer: " << buf << std::endl;
	if (buf.find("CAP LS 302") != std::string::npos)
		buf.replace(buf.find("CAP LS 302"), 10, "");
	if (buf.find("\n") != std::string::npos)
		buf.replace(buf.find("\n"), 1, "");
	if (buf.find("\r") != std::string::npos)
		buf.replace(buf.find("\r"), 1, "");
	// sendResponse(buf, fd);
	std::cout << "buffer: " << buf << std::endl;
	if (buf.find("JOIN") == 0)
	{
		sendResponse("JOIN", fd);
		std::cout << "Client " << fd << " sent join msg" << std::endl;
	}
}