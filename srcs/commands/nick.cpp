#include "Server.hpp"

bool Server::nicknameExist(const std::string &nick)
{
	for (size_t i = 0; i < _clients.size(); i++)
	{
		if (_clients[i]->getNickname() == nick)
			return true;
	}
	return false;
}

void Server::nick(std::string buf, int fd, int index)
{
	std::cout << "--------------- NICK -----------------" << std::endl;

	std::string nick = buf.substr(5);
	nick.replace(nick.find("\r"), 1, "");
	nick.replace(nick.find("\n"), 1, "");

	if (nicknameExist(nick))
	{	sendResponse("the nickname : " + nick + " is taken. Choose another one", fd); return;}

	std::cout << "Client " << fd << " set nickname to: " << nick << std::endl;
	sendResponse("your nickname is now " + nick, fd);
	_clients[index - 1]->setNickname(nick);
}