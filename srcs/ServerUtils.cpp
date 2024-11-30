#include "Server.hpp"

void Server::clearClient(int clientFd)
{
	for (size_t i = 1; i < _clients.size(); ++i)
	{
		if (_clients[i]->getSocket() == clientFd)
		{
			if (_clients[i])
				_clients.erase(_clients.begin() + i);
			break;
		}
	}

	for (size_t i = 0; i < _poll.getSize(); ++i)
	{
		if (_poll.getFd(i).fd == clientFd)
		{
			_poll.removeFd(i);
			break;
		}
	}
	close(clientFd);
}

bool Server::validateClientRegistration(int fd, int index)
{
	if (!_clients[index]) {
		std::cerr << "Error: Client " << fd << " not found." << std::endl;
		return false;
	}
	if (!_clients[index]->getIsRegistered()) {
		sendError("451 :You have not registered", fd);
		return true;
	}
	return true;
}