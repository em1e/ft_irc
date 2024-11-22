#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "ft_irc.hpp"
#include "Channel.hpp"

class Client
{
	public:
		Client();
		Client(int, sockaddr_in);
		Client(Client const &);
		~Client();
		Client & operator=(Client const &);

		// getters
			// get channel
			// get nickname
			// get username
		int getSocket();
		// setters
		void setSocket(int clientFd);
		void setIpAddress(sockaddr_in ip);
		void setBuffer(std::string buff);

		// channel actions
			// write
			// reply
			// join
			// leave
	private:
		std::string _nickname;
		std::string _username;
		std::string _buffer;
		int _socket;
		sockaddr_in _IPaddress;

		// Channel *_channel;
};

#endif