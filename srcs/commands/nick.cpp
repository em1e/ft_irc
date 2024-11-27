#include "Server.hpp"

void Server::nick(std::string buf, std::string response, int fd, int index)
{
	// make sure the nickname isn't already in use
	std::cout << "--------------- NICK -----------------" << std::endl;
	std::string nick = buf.substr(5);
	nick.replace(nick.find("\r"), 1, "");
	nick.replace(nick.find("\n"), 1, "");
	std::cout << "Client " << fd << " set nickname to: " << nick << std::endl;
	response += "your nickname is now " + nick + "\r\n";
	send(fd, response.c_str(), response.length(), 0);
	std::cout << "index = " << index << std::endl;
	_clients[index - 1]->setNickname(nick);
	std::cout << "index = " << index << std::endl;
	std::cout << "Nickname = " << _clients[index - 1]->getNickname() << std::endl;
	if (searchByNickname("bob") != -1)
		std::cout << "FOUND FUCKING BOB" << std::endl;
	
}