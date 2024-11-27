#pragma once

#include "Client.hpp"
#include "Channel.hpp"
#include "Socket.hpp"
#include "Poll.hpp"
#include <csignal> // signal()
#include <iostream>
#include <cstring> // memset() and others
#include <string>

#define MAX_CONNECTIONS 10

class Server
{
	public:
		Server(const std::string &, const std::string &);
		~Server();
		
		void startServer();
		void run();
		static void handle_signal(int sig);
		bool isRunning() const { return _isRunning; }

		void clearClient(int);
		int searchByNickname(std::string nick);

		

		void createNewClient();
		void handleNewData(int fd, int index);

		void capLs(std::string buf, std::string response, int fd);
		void join(std::string response, int fd); // add joining into channels for this one
		void nick(std::string buf, std::string response, int fd, int index);
		void user(std::string response, int fd, int index);
		void invite(std::string buf, int fd);
		void privmsg(std::string buf, std::string response, int fd, int index);
		void topic(std::string buf, int fd);
		void kick(std::string buf, int fd);
		void mode(std::string buf, int fd);
		Channel *createChannel(const std::string &name, Client *creator);
		// Creates a new channel, assigns the creator as admin, and adds the creator to the member list.
		// add /list t show a list of existing channels
		// Channel *findChannel(const std::string &channelName); //Searches for a channel by name. Returns a pointer or nullptr if not found



	private:
		static bool signal;
		bool _isRunning;

		const std::string _host;
		const std::string _port;
		const std::string _password;

		std::vector<Client *> _clients;
		std::vector<Channel *> _channels;

		Socket _socket;
		Poll _poll; // Stores pollfd structures for poll()
};
