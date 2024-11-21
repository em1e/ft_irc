#ifndef SERVER_HPP
#define SERVER_HPP

#include "ft_irc.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include <vector>
#include <poll.h>

class Server
{
	public:
		Server();
		Server(const std::string &, const std::string &);
		Server(Server const &);
		~Server();
		Server &operator=(Server const &);

		void run();
		bool isRunning() const;
		void clearClient(int);

	private:
		bool _isRunning;
		int _socket;

		const std::string _host;
		const std::string _port;
		const std::string _password;

		std::vector<Client> _clients;
		std::vector<pollfd> _pollFds; // Stores pollfd structures for poll()

		// New helper method
		void handlePollEvent(size_t index);
};

#endif