#include "Server.hpp"

void Server::user(std::string response, int fd, int index)
{
	std::cout << "--------------- USER -----------------" << std::endl;
	std::cout << "Client " << fd << " sent USER command." << std::endl;
	if (_clients[index - 1]->getNickname().empty())
	{
		std::cout << "user doesn't have a nickname, unable to welcome them!" << std::endl;
		response = ":localhost 001 Error: you need to set a nickname first!\r\n";
		return ;
	}
	response =
		" - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n"
		"  _____   \n"
		" /     \\       (\\_/)\n"
		"/       \\     (o.o )  Welcome\n"
		"|  MAIL  |    ( :   \\  to our IRC\n"
		"|  BOX   |    (\\ /   )    server " + _clients[index - 1]->getNickname() + "!\n"
		"|________|    \n"
		" - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -";
	// response += ":localhost 001 " + _clients[index - 1].getNickname() + " :Welcome to " + serverNickname + " IRC server " + _clients[index].getNickname() + "!\r\n";
	response += "\r\n";
	// _clients[index - 1]->setUsername() // set clients username and realname here
	send(fd, response.c_str(), response.length(), 0);
}