#pragma once

#include "Client.hpp"
#include <string>

class Channel
{
	public:
		Channel();
		Channel(std::string);
		Channel(Channel const &);
		~Channel();
		Channel & operator=(Channel const &);

		std::string getName() const { return _name; }
		Client *getAdmin() const { return _admin; }

		void setAdmin(Client *client) { _admin = client; }

		// Channel actions
		void addClient(Client *client);
		void removeClient(Client *client);
		// void broadcastMsg(const std::string &message, Client *sender);
		// void setTopic(onst std::string &topic, Client *op);
		// void kickClient(Client *operator, Client *clientToKick);
	private:
		std::string _name;
		// std::string _topic;
		Client *_admin;
		std::vector<Client *> _clients;
		// std::vector<Client *> _ops; //operators
		// int	_maxmembers;
		// std::string	_password;
};
