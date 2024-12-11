#include "Server.hpp"

/*

·i: Set/remove Invite-only channel
			When enabled, other users will not be 
			able to see you on a WHO output unless they 
			are in the channel themselves.
 · t: Set/remove the restrictions of the TOPIC command to channel operators.
			When enabled, users who are not opped or voices cannot 
			modify the channel topic.
· k <key>: Set/remove the channel key (password)
			Adds or removes a channel key, aka a password; 
			users will not be able to join the channel 
			without providing the key.
 · o <nickname>: Give/take channel operator privilege
	Adds or removes the operator status from a nickname.
· l <count>: Set/remove the user limit to channel
	Adds, removes or updates the maximum amount of 
	users that may join the channel.

*/

// MODE <your nick>|<channel> <modeString> [<modeParam>]
void Server::mode(std::string buf, int fd, int index)
{
	std::cout << "buf: " << buf <<std::endl;
	std::cout << "--------------- MODE -----------------" << std::endl;
	if (!validateClientRegistration(fd, index))
	{
		std::cout << "Authentication error" << std::endl;
		return ;
	}
	std::istringstream iss(buf);
	std::string command, chName, nickname, modeString, modeParam, keyPassword, count;
	iss >> command >> chName >> modeString;
	if (modeString[1] != 'i' || modeString[1] != 't')
	{
		std::getline(iss, modeParam);
		if (modeString[1] == 'k')
			keyPassword = modeParam;
		if (modeString[1] == 'o')
			nickname = modeParam.substr(1);
		if (modeString[1] == 'l')
			count = modeParam;
	}
	Channel *channel = findChannel(chName);
	if (chName.empty() || !channel || channel->isAdmin(_clients[index]) == -1)
	{
		if (chName.empty())
			sendError("461 MODE :Not enough parameters for MODE", fd);
		if (!channel)
			sendError("403 MODE :No such channel", fd);
		else
			sendError("482 MODE :You're not channel admin", fd);
		std::cout << "Channel error" << std::endl;
		return ;
	}

	//client not in the channel
	bool plussign = true;
	char mode = modeString[0];
	if (mode == '+')
		plussign = true;
	else if (mode == '-')
		plussign = false;
	std::shared_ptr<Client> target = getClient(nickname);
	for (char mode : modeString)
	{
		switch (mode)
		{
			case 'i':
				channel->setInviteOnly(plussign);
				sendReply("324 MODE: " + _clients[index]->getNickname() + " " + chName + " +" + mode, fd);
				break;
			case 't':
				channel->setTopicRestrictions(plussign);
				sendReply("324 MODE: " + _clients[index]->getNickname() + " " + chName + " +" + mode, fd);
				break;
			case 'k':
				if (plussign)
				{
					if (!channel->setChannelKey(plussign, keyPassword))
						sendError("461 MODE: Channel key is missing.", fd);
				}
				else
					channel->setChannelKey(plussign, "");
				sendReply("324 MODE: " + _clients[index]->getNickname() + " " + chName + " +" + mode + " " + keyPassword, fd);
				break;
			case 'o':
				if (!target)
				{
    				std::cout << "Error: Client with nickname '" << nickname << "' not found." << std::endl;
    				sendError("432 MODE :Invalid or unusable name", fd);
    				return;
				}
				else
    				std::cout << "Found client: " << target->getNickname() << std::endl;
				if (channel->isClient(target) == -1)
				{
    				std::cout << "Error: Target client is not in the channel." << std::endl;
    				sendError("441 MODE :They aren't on that channel", fd);
    				return;
				}
				else
    				std::cout << "Target client is in the channel: " << target->getNickname() << std::endl;
				if (target != nullptr)
				{
					if (plussign && channel->isAdmin(target) == -1)
						channel->addAdmin(target);
					else if (!plussign && channel->isAdmin(target) >= 0)
						channel->removeAdmin(target);
				}
				else
				{
					sendError("432 MODE: Invalid or unusable name", fd);
					std::cout << "Invalid or unusable name" << std::endl;
				}
				sendReply("324 MODE: " + _clients[index]->getNickname() + " " + chName + " +" + mode + " " + nickname, fd);
				break;
			case 'l':
				if (plussign)
				{
					if (count.empty() || std::stoi(count) < 0)
					{
						if (count.empty())
							sendError("461 MODE :Not enough parameters for MODE", fd);
						else
							sendError("461 MODE :Invalid parameter for MODE", fd);
						std::cout << "Invalid parameter" << std::endl;
						return ;
					}
					channel->setUserLimit(std::stoi(count));
				}
				else
					channel->setUserLimit(-1);
				sendReply("324 MODE: " + _clients[index]->getNickname() + " " + chName + " +" + mode + " " + nickname, fd);
				break;
			default:
			{
				sendError("472 MODE :Unknown MODE flag", fd);
				std::cout << "Invalid mode" << std::endl;
				break;
			}
		}
	}
	std::cout << *channel << std::endl;
}




