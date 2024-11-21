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
		Server & operator=(Server const &);

		void run();
		bool isRunning() const;
		void handleClient(Client);
		void clearClient(int);


	private:
		bool _isRunning;
		int _socket;

		const std::string _host;
		const std::string _port;
		const std::string _password;

		std::vector<Client> _clients;
		// add channels
		// add users, using vector

};

#endif