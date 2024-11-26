#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "ft_irc.hpp"
#include "Client.hpp"

class Channel
{
	public:
		Channel();
		Channel(std::string);
		Channel(Channel const &);
		~Channel();
		Channel & operator=(Channel const &);

		// Getters
		std::string getName() const;
		Client *getAdmin() const;

		// Setters
		void setAdmin(Client *client);

		// channel actions
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

#endif