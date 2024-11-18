#ifndef SERVER_HPP
#define SERVER_HPP

#include "ft_irc.hpp"

class Server
{
	public:
		Server();
		Server(const std::string &, const std::string &);
		Server(Server const &);
		~Server();
		Server & operator=(Server const &);

	private:
		bool _isRunning;
		int _socket;

		const std::string _host;
		const std::string _port;
		const std::string _password;

		// add channels
		// add users

};

#endif