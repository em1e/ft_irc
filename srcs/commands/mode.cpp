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
	
	bool plussign = true;
	char modeSign = modeString[0];
	if (modeSign == '+')
		plussign = true;
	else if (modeSign == '-')
		plussign = false;
	else
	{
		sendError("461 " + nick + " MODE :Not enough parameters", fd);
		return ;
	}
	
	if ((modeString[0] == '+' && (modeString[1] == 'k' || modeString[1] == 'l')) || modeString[1] == 'o')
	{
		std::getline(iss, modeParam);
		modeParam = modeParam.substr(1);
	}

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

	std::shared_ptr<Client> target = getClient(modeParam);
	for (size_t i = 1; i < modeString.length(); ++i)
	{
		char mode = modeString[i];
		switch (mode)
		{
			case 'i':
				if (plussign != channel->getInviteOnly())
				{
					channel->setInviteOnly(plussign);
					sendResponse(":localhost 324 " + _clients[index]->getNickname() + " " + chName + " " + modeSign + mode, fd);
				}
				break;
			case 't':
				channel->setTopicRestrictions(plussign);
				sendResponse(":localhost 324 " + _clients[index]->getNickname() + " " + chName + " " + modeSign + mode, fd);
				break;
			case 'k':
				if (modeParam.empty() && plussign)
				{
					sendError("461 " + nick + " MODE :Not enough parameters", fd);
					return;
				}
				channel->setChannelKey(plussign, modeParam);
				sendResponse(":localhost 324 " + _clients[index]->getNickname() + " " + chName + " " + modeSign + mode, fd);
				break;
			case 'o':
				if (modeParam.empty())
				{
					sendError("461 " + nick + " MODE :Not enough parameters", fd);
					return;
				}
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
				sendResponse(":localhost 324 " + _clients[index]->getNickname() + " " + chName + " " + modeSign + mode + " " + modeParam, fd);
				break;
			case 'l':
				if ((plussign && modeParam.empty()) || (!modeParam.empty() && std::stoi(modeParam) < 0))
				{
					sendError("461 " + nick + " MODE :Not enough parameters", fd);
					return;
				}
				if (plussign)
					channel->setUserLimit(std::stoi(modeParam));
				else
					channel->setUserLimit(-1);
				sendResponse(":localhost 324 " + _clients[index]->getNickname() + " " + chName + " " + modeSign + mode, fd);
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




