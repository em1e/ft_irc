#pragma once

// #include "ft_irc.hpp"
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

		static void handle_signal(int sig);
		void run();
		void clearClient(int);
		int searchByNickname(std::string nick);
		bool isRunning() const { return _isRunning; }
		void createNewClient();
		void handleNewData(int fd, int index);
		
		void startServer();
		
		void capLs(std::string buf, std::string response, int fd);
		void join(std::string response, int fd);
		void nick(std::string buf, std::string response, int fd, int index);
		void user(std::string response, int fd, int index);
		void invite(std::string buf, int fd);
		void privmsg(std::string buf, std::string response, int fd, int index);

	private:
		bool _isRunning;
		static bool signal;
		Socket _socket;

		const std::string _host;
		const std::string _port;
		const std::string _password;

		std::vector<Client *> _clients;
		Poll _poll; // Stores pollfd structures for poll()
};
