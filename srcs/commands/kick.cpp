#include "Server.hpp"

void Server::kick(std::string buf, int fd)
{
	std::cout << "--------------- KICK -----------------" << std::endl;
	Client *admin = getClient(getNickname(fd));
	if (!admin)
		throw std::invalid_argument("Error: Client not found.");

	std::cout << "admin : |" << admin->getNickname() << "|" << std::endl;
	std::cout << "buf : |" << buf << "|" << std::endl;

	buf.replace(buf.find("\r"), 1, "");
	buf.replace(buf.find("\n"), 1, "");

	std::istringstream iss(buf);
	std::string command, kick, channel, reason;
	iss >> command >> channel >> kick >> reason;
	std::cout << "kick user : |" << kick << "|" << std::endl;

	if (kick.empty() || searchByNickname(kick) == -1)
	{
		std::cout << "error, No such nickname found" << std::endl;
		throw std::invalid_argument("Error: No such nickname.");
	}

	Channel* chan = findChannel(channel);
	if (chan == nullptr)
	{
		std::cout << "error, channel is null" << std::endl;
		throw std::invalid_argument("Error: No such channel.");
	}

	if (!chan->isAdmin(_clients[searchByNickname(admin->getNickname())]))
	{
		std::cout << "you're not an admin of the channel" << std::endl;
		throw std::invalid_argument("Error: You are not an admin of this channel.");
	}

	if (!chan->isClient(_clients[searchByNickname(kick)]))
	{
		std::cout << "client is not in the channel" << std::endl;
		throw std::invalid_argument("Error: client is not in the channel.");
	}
	
	Client *user;
	for (size_t i = 1; i < chan->getClients().size(); ++i)
	{
		user = chan->getClient(i);
		if (user && user->getNickname() == kick)
		{	chan->removeClient(user); break;}
	}
}