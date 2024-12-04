#include "Server.hpp"

void Server::list(std::string buf, int fd)
{
	std::cout << "--------------- LIST -----------------" << std::endl;
	std::istringstream iss(buf);
	std::string command, chName;
	iss >> command >> chName;

	if (chName.empty())
	{
		for (size_t i = 0; i < _channels.size(); ++i)
		{
			std::string response = _channels[i]->getName() + " " + std::to_string(_channels[i]->getUserCount()) + " " +_channels[i]->getTopic();
			sendResponse(response, fd);
		}
		return;
	}

	do {
		for (size_t i = 0; i < _channels.size(); ++i)
		{
			if (_channels[i]->getName() == chName)
			{
				std::string response = _channels[i]->getName() + " " + std::to_string(_channels[i]->getUserCount()) + " " +_channels[i]->getTopic();
				sendResponse(response, fd);
			}
		}
	} while (iss >> chName);
}

//  End of /LIST