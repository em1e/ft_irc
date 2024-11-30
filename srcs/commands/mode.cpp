#include "Server.hpp"

/********* Mandatory MODES **********

 ·i: Set/remove Invite-only channel

When enabled, other users will not be 
able to see you on a WHO output unless they 
are in the channel themselves.

 · t: Set/remove the restrictions of the TOPIC command to channel
 operators

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
	std::string command, chName, modeString, modeParam;
	iss >> command >> chName >> modeString;

	
	Channel *channel = findChannel(chName);
	if (chName.empty() || !channel || !channel->isAdmin(_clients[index]))
	{
		if (chName.empty())
			sendError("461 :Not enough parameters for MODE", fd);
		if (!channel)
			return ;
		else
			sendError("482 :You're not channel admin", fd);
		return ;
	}

	std::getline(iss, modeParam);
	bool plussign = true;
	(void)plussign;
	for (char mode : modeString)
	{
		if (mode == '+')
		{ plussign = true; continue; }
		else if (mode == '-')
		{ plussign = false; continue; }
		switch (mode)
		{
			case 'i':
				//Set/remove Invite-only channel
				break;
			case 't':
				//Set/remove the restrictions of the TOPIC command to channel operators
				break;
			case 'k':
				//Set or remove the channel key (password)
				break;
			case 'o':
				//Give or take channel operator privilege
				break;
			case 'l':
				//Set or remove the user limit to channel
				break;
			default:
				//invalid mode
				break;
		}
	}
	std::cout << '\n' << *channel << std::endl;
}




