#pragma once

#include "Client.hpp"
#include "Channel.hpp"
#include "Socket.hpp"
#include "Poll.hpp"
#include <csignal> // signal()
#include <iostream>
#include <cstring> // memset() and others
#include <string>
#include <algorithm> //find()
#include <sstream> // for std::istringstream

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

		void sendResponse(std::string msg, int fd);

		void clearClient(int);
		int searchByNickname(std::string nick);
		std::string getNickname(int fd);
		Client *getClient(std::string nick);
		void createNewClient();

		void handleNewData(int fd, int index);
		bool nicknameExist(const std::string &nick);

		void capLs(std::string buf, std::string response, int fd);
		void join(std::string buf, int fd);
		void nick(std::string buf, int fd, int index);
		void user(std::string buf, int fd, int index);
		void invite(std::string buf, int fd);
		void privmsg(std::string buf, std::string response, int fd, int index);
		void topic(std::string buf, int fd);
		void kick(std::string buf, int fd);
		void mode(std::string buf, int fd);

		Channel *createChannel(const std::string &name, Client *creator);
		Channel *findChannel(const std::string &name);
		int isInChannel(Client *client);
		int getChannelIndex(std::string name);

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
