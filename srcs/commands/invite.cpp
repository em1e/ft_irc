#include "Server.hpp"

void Server::invite(std::string buf, int fd, int index)
{
	std::cout << "--------------- INVITE -----------------" << std::endl;
	if (!validateClientRegistration(fd, index))
		return ;
	
	std::istringstream iss(buf);
	std::string command, inviter, chName;
	iss >> command >> inviter >> chName;

	if (inviter.empty() || chName.empty() || searchByNickname(inviter) == -1)
	{
		if (inviter.empty() || chName.empty())
			sendError("461 :Not enough parameters for INVITE", fd);
		else
			sendError("401 :No such client found", fd);
		return;
	}

	std::cout << "buf : |" << buf << "|" << std::endl;
	std::cout << "inviter : |" << inviter << "|" << std::endl;
	std::cout << "inviter : |" << _clients[index]->getNickname() << "|" << std::endl;
	std::cout << "channel : |" << chName << "|" << std::endl;

	Channel* channel = findChannel(chName);
	if (chName.empty() || !channel || !channel->isAdmin(_clients[index]))
	{
		if (chName.empty())
			sendError("461 :Not enough parameters for TOPIC", fd);
		else if (!channel)
			sendError("442 :No such channel exist", fd);
		else
			sendError("482 " + chName + " :You're not channel operator", fd);
		return;
	}
	else if (!channel->getInviteOnly()
		|| (channel->getInviteOnly() && channel->isInvited(_clients[searchByNickname(inviter)])))
	{
		if (!channel->getInviteOnly())
			sendError("482 " + chName + " :You're not channel operator", fd);
		else
			sendError("482 :Client is already invited to the channel", fd);
		return;
	}

	std::string response = ":" + _clients[index]->getNickname() + " INVITED " + inviter + " to " + chName + "\r\n";
	std::cout << "response : |" << response << "|" << std::endl;
	
	channel->addInvited(_clients[searchByNickname(inviter)]);
	channel->broadcastAdmins(response);
}