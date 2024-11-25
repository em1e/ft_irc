#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "ft_irc.hpp"
#include <string>

class Client;
class Server;

class Channel
{
	public:
		Channel();
		Channel(std::string const &name);
		Channel(Channel const &);
		~Channel();
		Channel & operator=(Channel const &);

		// Getters
		std::string getName() const;
		std::string getTopic() const;
		Client *getAdmin() const;

		// Setters
		void setAdmin(Client *client);

		// channel actions
		void addClient(Client *client);
		void removeClient(Client *client);

	private:
		std::string _name;
		std::string _topic;
		Client *_admin;
		std::vector<Client *> _clients;
		std::map<Client *, bool> _admins; //client -> isAdmin
		std::vector<Client *> _invite_list;
};

#endif