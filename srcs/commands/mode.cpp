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
	std::cout << "--------------- MODE -----------------" << std::endl;
	if (!validateClientRegistration(fd, index))
	{
		std::cout << "Authentication error" << std::endl;
		return ;
	}
	std::istringstream iss(buf);
	std::string command, chName, modeString, modeParam;
	iss >> command >> chName >> modeString;
	
	std::cout << "command" << command << std::endl;
	std::cout << "chName" << chName << std::endl;
	std::cout << "modeString" << modeString << std::endl;

	std::string nick = _clients[index]->getNickname();
	if (modeString.empty())
		return;
	
	Channel *channel = findChannel(chName);
	if (chName.empty() || (chName[0] == '#' && !channel) || (channel && channel->isAdmin(_clients[index]) == -1))
	{
		if (chName.empty())
			sendError("461 " + nick + " MODE :Not enough parameters", fd);
		if (!channel)
			sendError("403 " + nick + " " + chName + " :No such channel", fd);
		else
			sendError("482 " + nick + " " + chName + " :You're not channel operator", fd);
		return ;
	}
	if (chName[0] != '#')
		return ;
	
	bool plussign = true;
	char modeSign = modeString[0];
	if (modeSign == '+')
		plussign = true;
	else if (modeSign == '-')
		plussign = false;
	else
	{
		sendError("502 " + nick + " :Cant change mode for other users", fd);
		return ;
	}
	
	if ((modeString[0] == '+' && (modeString[1] == 'k' || modeString[1] == 'l')) || modeString[1] == 'o')
	{
		std::getline(iss, modeParam);
		if (!modeParam.empty())
			modeParam = modeParam.substr(1);
		else
		{	
			if (modeString[1] != 'k')
				sendError("461 " + nick + " MODE :Not enough parameters", fd);
			return ;
		}
	}

	std::shared_ptr<Client> target = getClient(modeParam);
	std::string resMsg = ":localhost 324 " + nick + " " + chName + " " + modeSign;
	for (size_t i = 1; i < modeString.length(); ++i)
	{
		char mode = modeString[i];
		switch (mode)
		{
			case 'i':
				if (plussign != channel->getInviteOnly())
				{
					channel->setInviteOnly(plussign);
					channel->broadcast(resMsg + mode + "\r\n", nullptr, 0);
				}
				break;
			case 't':
				if (plussign != channel->getTopicRestrictions())
				{
					channel->setTopicRestrictions(plussign);
					channel->broadcast(resMsg + mode + "\r\n", nullptr, 0);
				}
				break;
			case 'k':
				channel->setChannelKey(plussign, modeParam);
				if (!plussign)
					modeParam = "*";
				channel->broadcast(resMsg + mode  + " " + modeParam + "\r\n", nullptr, 0);
				break;
			case 'o':
				if (!target)
				{
					std::cout << "Error: Client with nickname '" << modeParam << "' not found." << std::endl;
					sendError("401 " + nick + " " + modeParam + " :No such nick/channel", fd);
					return;
				}
				if (channel->isClient(target) == -1)
				{
					std::cout << "Error: Target client is not in the channel." << std::endl;
					sendError("441 " + nick + " " + modeParam + " " + chName + " :They aren't on that channel", fd);
					return;
				}
				if (plussign && channel->isAdmin(target) == -1)
					channel->addAdmin(target);
				else if (!plussign && channel->isAdmin(target) >= 0)
					channel->removeAdmin(target);
				channel->broadcast(resMsg + mode + " " + modeParam + "\r\n", nullptr, 0);
				break;
			case 'l':
				if (!modeParam.empty() && std::stoi(modeParam) < 1)
					return ;
				if (plussign)
					channel->setUserLimit(std::stoi(modeParam));
				else
					channel->setUserLimit(-1);
				channel->broadcast(resMsg + mode + " " + modeParam + "\r\n", nullptr, 0);
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




