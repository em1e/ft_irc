#include "Server.hpp"

void Server::list(std::string buf, int fd, int index)
{
	std::cout << "--------------- LIST -----------------" << std::endl;
	std::istringstream iss(buf);
	std::string command, chName;
	iss >> command >> chName;

	sendResponse(":localhost 321 " + _clients[index]->getNickname() + " Channel :Users  Name", fd);
	if (chName.empty())
	{
		for (size_t i = 0; i < _channels.size(); ++i)
		{
			std::string response = ":localhost 322 " + _clients[index]->getNickname() + " " +  _channels[i]->getName() + " " + std::to_string(_channels[i]->getUserCount()) + " :" +_channels[i]->getTopic();
			sendResponse(response, fd);
		}
		sendResponse(":localhost 321 " + _clients[index]->getNickname() + " :End of /LIST", fd);
		return;
	}
}
