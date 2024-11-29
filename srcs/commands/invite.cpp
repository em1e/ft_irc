#include "Server.hpp"

void Server::invite(std::string buf, int fd)
{
	std::cout << "--------------- INVITE -----------------" << std::endl;

	Client *inviter = getClient(getNickname(fd));
	if (!inviter)
	{	std::cerr << "Error: Client not found." << std::endl; return;}

	std::cout << "inviter : |" << inviter->getNickname() << "|" << std::endl;

	size_t pos;
	if ((pos = buf.find("\r")) != std::string::npos)
		buf.replace(pos, 1, "");
	if ((pos = buf.find("\n")) != std::string::npos)
		buf.replace(pos, 1, "");
	
	std::string invitee = buf.substr(buf.find(" ") + 1, buf.find(" ", buf.find(" ") + 1) - (buf.find(" ") + 1));
	std::cout << "invitee : |" << invitee << "|" << std::endl;
	if (invitee.empty() || searchByNickname(invitee) == -1)
		throw std::invalid_argument("Error: No such nickname.");

	std::cout << "buf : |" << buf << "|" << std::endl;
	std::cout << "invitee : |" << invitee << "|" << std::endl;

	std::string channel;
	channel = buf.substr(buf.find(" ", buf.find(" ") + 1) + 1);

	std::cout << "buf : |" << buf << "|" << std::endl;
	std::cout << "invitee : |" << invitee << "|" << std::endl;
	std::cout << "inviter : |" << inviter << "|" << std::endl;
	std::cout << "channel : |" << channel << "|" << std::endl;

	Channel* chan = findChannel(channel);
	if (chan == nullptr)
	{
		std::cout << "error, channel is null" << std::endl;
		throw std::invalid_argument("Error: No such channel.");
	}
	if (!chan->isAdmin(inviter))
	{
		std::cout << "you're not an admin of the channel" << std::endl;
		throw std::invalid_argument("Error: You are not an admin of this channel.");
	}
	if (!chan->getInviteOnly())
	{
		std::cout << "channel does not need an invitation to join" << std::endl;
		throw std::invalid_argument("channel does not need an invitation to join");
	}

	std::string response = ":" + inviter->getNickname() + " INVITED " + invitee + " to " + channel + "\r\n";
	std::cout << "response : |" << response << "|" << std::endl;
	
	chan->addInvited(_clients[searchByNickname(invitee)]);
	
	chan->broadcastAdmins(response);
}