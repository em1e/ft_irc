#pragma once

#include "Client.hpp"
#include <string>
#include <iostream>

class Channel
{
	public:
		Channel(const std::string &name);
		~Channel();

		bool getInviteOnly() const { return _inviteOnly; }
		bool getTopicRestrictions() const { return _topicRestrictions; }
		bool getIsChannelPassword() const { return _isChannelPassword; }
		int getUserLimit() const { return _userLimit; }

		std::string getName() const { return _name; }
		std::string getTopic() const { return _topic; }
		std::string getPassword() const { return _password; }

		// Channel actions
		void addClient(Client *client);
		void removeClient(Client *client);
		std::vector<Client *> getClients() const { return _clients; }
		Client *getClient(int i) const { return _clients[i]; }

		// Admin actions
		bool isAdmin(Client *client) const;
		void addAdmin(Client *admin);
		void removeAdmin(Client *admin);
		std::vector<Client *> getAdmins() const { return _admins; }

		// Invite actions
		void addInvited(Client *client);
		void removeInvited(Client *client);
		int isInvited(Client *client);
		std::vector<Client *> getInvited() const { return _invited; }

		void broadcast(const std::string &msg);
		void broadcastAdmins(const std::string &msg);

		void setTopic(const std::string &topic);
		void changeMode(const std::string &mode, Client *admin);
	
	private:
		bool _inviteOnly;
		bool _topicRestrictions;
		bool _isChannelPassword;
		int _userLimit;

		std::string _name;
		std::string _topic;
		std::string	_password;
		std::vector<Client *> _admins;
		std::vector<Client *> _clients;
		std::vector<Client *> _invited;
};

std::ostream& operator<<(std::ostream& os, const Channel& channel);

