#include "Server.hpp"

void Server::clearClient(int clientFd, int index)
{
	if (index < 0 || index >= (int)_clients.size()) return;

	_clients[index]->setIsAuthenticated(false);
	_clients[index]->setIsRegistered(false);

	for (size_t i = 0; i < _channels.size(); ++i)
	{
		if (isInChannel(_clients[index], _channels[i]))
		{
			if (_channels[i]->isAdmin(_clients[index]) != -1)
				_channels[i]->removeAdmin(_clients[index]);
			if (_channels[i]->isClient(_clients[index]) != -1)
				_channels[i]->removeClient(_clients[index]);
			if (_channels[i]->isInvited(_clients[index]) != -1)
				_channels[i]->removeInvited(_clients[index]);
			break;
		}
	}

	// std::cout << "CLIENT INDEX IS: " << index << std::endl;
	// std::cout << "SIZE BEFORE ERASE: " << _clients.size() << std::endl;

	_clients.erase(_clients.begin() + index);

	// std::cout << "SIZE AFTER ERASE: " << _clients.size() << std::endl;
	// std::cout << "FD SIZE BEFORE ERASE: " << _poll.getSize() << std::endl;
	for (size_t i = 0; i < _poll.getSize(); ++i)
	{
		if (_poll.getFd(i).fd == clientFd)
		{
			// std::cout << "REMOVING FD FROM THE CLIENT: " << clientFd << std::endl;
			_poll.removeFd(i);
			break;
		}
	}
	// std::cout << "FD SIZE AFTER ERASE: " << _poll.getSize() << std::endl;

	// if (_clients.empty()) {
	// 	std::cout << "The client list is now empty." << std::endl;
	// } else {
	// 	std::cout << "NAME OF FIRST CLIENT IS: " << _clients[0]->getNickname() << std::endl;
	// }

	close(clientFd);
}

bool Server::validateClientRegistration(int fd, int index)
{
	if (!_clients[index])
	{
		std::cerr << "Error: Client " << fd << " not found." << std::endl;
		return false;
	}
	if (!_clients[index]->getIsRegistered())
	{
		sendError("451 : You have not registered", fd);
		return false;
	}
	return true;
}