#pragma once

#include "ft_irc.hpp"
// #include "Client.hpp"
#include "Channel.hpp"
#include "Socket.hpp"
#include "Poll.hpp"

class Server
{
	public:
		Server(const std::string &, const std::string &);
		Server(Server const &);
		~Server();
		Server &operator=(Server const &);

		static void handle_signal(int sig);
		void run();
		bool isRunning() const;
		void clearClient(int);
		int searchByNickname(std::string nick);
		
		void startServer();

	private:
		bool _isRunning;
		static bool signal;
		Socket _socket;

		const std::string _host;
		const std::string _port;
		const std::string _password;

		std::vector<Client *> _clients;
		Poll _poll; // Stores pollfd structures for poll()

		// New helper method
		void handlePollEvent(size_t index);
};
