#pragma once

#include "Client.hpp"
#include "Channel.hpp"
#include "Socket.hpp"
#include "Poll.hpp"
#include <csignal> // signal()
#include <iostream>
#include <cstring> // memset() and others

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
		void join(std::string response, int fd);
		void nick(std::string buf, std::string response, int fd, int index);
		void user(std::string response, int fd, int index);
		void invite(std::string buf, int fd);
		void privmsg(std::string buf, std::string response, int fd, int index);

	private:
		static bool signal;
		bool _isRunning;

		const std::string _host;
		const std::string _port;
		const std::string _password;

		Socket _socket;
		std::vector<Client *> _clients;
		Poll _poll; // Stores pollfd structures for poll()
};
