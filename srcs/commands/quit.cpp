#include "Server.hpp"

void Server::quit(int fd, int index)
{
	std::cout << "--------------- QUIT -----------------" << std::endl;
	for (size_t i = 0; i < _channels.size(); ++i)
	{
		if (_channels[i]->isClient(_clients[index]) != -1)
		{
			std::cout << _channels[i]->getName() << std::endl;
			_channels[i]->broadcast(":" + _clients[index]->getNickname() + "!" + _clients[index]->getUsername() + "@localhost QUIT :Quit: leaving" + "\r\n", nullptr, 0);
		}
	}
	std::cout << "Client " << _clients[index]->getNickname() << " sent QUIT command." << std::endl;
	clearClient(fd, index);
}