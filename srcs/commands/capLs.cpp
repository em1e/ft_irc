#include "Server.hpp"

void Server::capLs(int fd, int index)
{
	std::cout << "--------------- CAP LS -----------------" << std::endl;
	if (!validateClientRegistration(fd, index))
		return ;
	sendResponse(":localhost CAP * LS :cap-notify", fd);
}