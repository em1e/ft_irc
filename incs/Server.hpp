#ifndef SERVER_HPP
#define SERVER_HPP

#include "ft_irc.hpp"
#include "Client.hpp"
#include "Channel.hpp"

class Server
{
	public:
		Server();
		Server(const std::string &, const std::string &);
		Server(Server const &);
		~Server();
		Server &operator=(Server const &);
		static void handle_signal(int sig);
		void run();
		bool isRunning() const;
		void clearClient(int);
		int searchByNickname(std::string nick);

	private:
		bool _isRunning;
		int _socket;
		static bool signal;

		const std::string _host;
		const std::string _port;
		const std::string _password;

		std::vector<Client *> _clients;
		std::vector<pollfd> _pollFds; // Stores pollfd structures for poll()

		// New helper method
		void handlePollEvent(size_t index);
};

#endif