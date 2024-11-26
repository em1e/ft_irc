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

		std::string getName() const { return _name; }
		Client *getAdmin() const { return _admin; }

		void setAdmin(Client *client) { _admin = client; }

		// Channel actions
		void addClient(Client *client);
		void removeClient(Client *client);

	private:
		std::string _name;
		Client *_admin;
		std::vector<Client *> _clients;
};
