#pragma once

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

	private:
		std::string _name;
		Client *_admin;
		std::vector<Client *> _clients;
};
