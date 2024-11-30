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
void Server::mode(std::string buf, int fd)
{
	std::cout << "--------------- MODE -----------------" << std::endl;
	Client *client = getClient(getNickname(fd));
	std::cout << "buf = " << buf << std::endl;

	if (!client)
	{	std::cerr << "Error: Client: " << fd << " not found." << std::endl; return;}

	// checks that client is registerd
	if (!client->getIsRegistered())
		return ;

    buf.replace(buf.find("\r"), 1, "");
	buf.replace(buf.find("\n"), 1, "");
	std::istringstream iss(buf);
    std::string command, target, modeString, modeParam;
    iss >> command >> target >> modeString;
    std::getline(iss, modeParam);
	Channel *channel = findChannel(target);
	if (!channel)
	{
		std::string errorMsg = "403 " + client->getNickname() + " " + target + " :No such channel\r\n";
		send(fd, errorMsg.c_str(), errorMsg.length(), 0);
		return;
	}
	if (!channel->isAdmin(client))
	{
		std::string errorMsg = "482 " + client->getNickname() + " " + target + " :You're not a channel admin\r\n";
		send(fd, errorMsg.c_str(), errorMsg.length(), 0);
		return;
	}
	bool plussign = true;
	(void)plussign;
	for (char mode : modeString) {
		if (mode == '+') {
			plussign = true;
			continue;
		} else if (mode == '-') {
			plussign = false;
			continue;
		}
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




