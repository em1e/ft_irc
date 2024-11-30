#include "Server.hpp"

void Server::invite(std::string buf, int fd, int index)
{
	std::cout << "--------------- INVITE -----------------" << std::endl;

	// check if client exists and is registerd
	if (!_clients[index] || !_clients[index]->getIsRegistered())
	{
		if (!_clients[index])
			std::cerr << "Error: Client: " << fd << " not found." << std::endl;
		else
			sendError("451 :You must register before using this command", fd);
		return;
	}

	// buf.replace(buf.find("\r"), 1, "");
	// buf.replace(buf.find("\n"), 1, "");
	
	std::istringstream iss(buf);
	std::string command, invitee, chName;
	iss >> command >> invitee >> chName;

	if (invitee.empty() || chName.empty() || searchByNickname(invitee) == -1)
	{
		if (invitee.empty() || chName.empty())
			sendError("461: Not enough parameters for INVITE", fd);
		else
			sendError("401: No such client found", fd);
		return;
	}

	std::cout << "buf : |" << buf << "|" << std::endl;
	std::cout << "invitee : |" << invitee << "|" << std::endl;
	std::cout << "inviter : |" << _clients[index]->getNickname() << "|" << std::endl;
	std::cout << "channel : |" << chName << "|" << std::endl;

	Channel* channel = findChannel(chName);
	if (chName.empty() || !channel || !channel->isAdmin(_clients[index]))
	{
		if (chName.empty())
			sendError("461: Not enough parameters for TOPIC", fd);
		else if (!channel)
			sendError("442: No such channel exist", fd);
		else
			sendError("482: You're not channel admin", fd);
		return;
	}
	else if (!channel->getInviteOnly()
		|| (channel->getInviteOnly() && channel->isInvited(_clients[searchByNickname(invitee)])))
	{
		if (!channel->getInviteOnly())
			sendError("482: Channel does not need invitations", fd);
		else
			sendError("482: Client is already invited to the channel", fd);
		return;
	}

	std::string response = ":" + _clients[index]->getNickname() + " INVITED " + invitee + " to " + chName + "\r\n";
	std::cout << "response : |" << response << "|" << std::endl;
	
	channel->addInvited(_clients[searchByNickname(invitee)]);
	channel->broadcastAdmins(response);
}