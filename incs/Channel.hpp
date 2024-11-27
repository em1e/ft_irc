#pragma once

#include "Client.hpp"
#include <string>
#include <iostream>

class Channel
{
	public:
		Channel(const std::string &name);
		~Channel();

		std::string getName() const { return _name; }
		int getUserLimit() const { return _userLimit; }
		std::string getTopic() const { return _topic; }

		// Channel actions
		void addClient(Client *client);
		void removeClient(Client *client);

		// Admin actions
		bool isAdmin(Client *client) const;
		void addAdmin(Client *admin);
		void removeAdmin(Client *admin);

		void broadcast(const std::string &msg, Client *sender);
		void broadcastAdmin(const std::string &msg);

		void setTopic(const std::string &topic, Client *admin);
		void changeMode(const std::string &mode, Client *admin);


	private:
		bool _inviteOnly;
		bool _topicRestrictions;
		bool _channelPassword;
		int _userLimit;

		std::string _name;
		std::string _topic;
		std::string	_password;
		std::vector<Client *> _admins;
		std::vector<Client *> _clients;
		std::vector<Client *> _invited;
};
