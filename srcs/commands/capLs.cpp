#include "Server.hpp"

void Server::capLs(int fd, int index)
{
	std::cout << "--------------- CAP LS -----------------" << std::endl;
	if (!_clients[index]->getIsRegistered()) {
		sendError("451 :You have not registered", fd);
		return ;
	}
	sendResponse(":localhost CAP * LS :multi-prefix sasl away-notify", fd);
}