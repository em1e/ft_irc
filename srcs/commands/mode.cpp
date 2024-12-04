#include "Server.hpp"

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
		else if (modeString[1] == 'o')
			nickname = modeParam.substr(1);
		else if (modeString[1] == 'l')
			count = modeParam;
	}
	Channel *channel = findChannel(chName);
	if (chName.empty() || !channel || !channel->isAdmin(_clients[index]))
	{
		if (chName.empty())
			sendError("461 MODE :Not enough parameters for MODE", fd);
		if (!channel)
			return ;
		else
			sendError("482 MODE :You're not channel admin", fd);
		std::cout << "Channel error" << std::endl;
		return ;
	}
	
	//handle multiple modes
	//client not in the channel
	bool plussign = true;
	char mode = modeString[0];
	if (mode == '+')
		plussign = true;
	else if (mode == '-')
		plussign = false;
	// std::cout << "command: " << command << "\n channel: " << channel << "\n modeString: " << modeString << "\n modeParam: " << modeParam <<std::endl;
	// std::cout << "chName: " << chName << "\n target: " << target <<std::endl;
	// exit(0);
	Client *target = getClient(nickname);
	for (char mode : modeString)
	{
		switch (mode)
		{
			// ·i: Set/remove Invite-only channel

			// When enabled, other users will not be 
			// able to see you on a WHO output unless they 
			// are in the channel themselves.
			case 'i':
				channel->setInviteOnly(plussign);
				break;
			//  · t: Set/remove the restrictions of the TOPIC command to channel operators.
			
			// When enabled, users who are not opped or voices cannot 
			// modify the channel topic.
			case 't':
				channel->setTopicRestrictions(plussign);
				break;
			// · k <key>: Set/remove the channel key (password)

			// Adds or removes a channel key, aka a password; 
			// users will not be able to join the channel 
			// without providing the key.
			case 'k':
				if (plussign && !modeParam.empty())
					channel->setChannelKey(keyPassword);
				else
					channel->setChannelKey("");
				break;
			//  · o <nickname>: Give/take channel operator privilege

			// Adds or removes the operator status from a nickname.
			case 'o':
				if (!target)
				{
    				std::cout << "Error: Client with nickname '" << nickname << "' not found." << std::endl;
    				sendError("432 MODE :Invalid or unusable name", fd);
    				return;
				}
				else
    				std::cout << "Found client: " << target->getNickname() << std::endl;
				if (channel->isClient(target) < 0)
				{
    				std::cout << "Error: Target client is not in the channel." << std::endl;
    				sendError("441 MODE :They aren't on that channel", fd);
    				return;
				}
				else
    				std::cout << "Target client is in the channel: " << target->getNickname() << std::endl;
				if (target != nullptr)
				{
					if (plussign && !channel->isAdmin(target))
						channel->addAdmin(target);
					else if (!plussign && channel->isAdmin(target))
						channel->removeAdmin(target);
				}
				else
				{
					sendError("432 MODE: Invalid or unusable name", fd);
					std::cout << "Invalid or unusable name" << std::endl;
				}
				break;
			// · l <count>: Set/remove the user limit to channel

			// Adds, removes or updates the maximum amount of 
			// users that may join the channel.
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
				break;
			default:
			{
				sendError("421 MODE :Unknown MODE flag", fd);
				std::cout << "Invalid mode" << std::endl;
				break;
			}
		}
	}
	std::cout << *channel << std::endl;
}




