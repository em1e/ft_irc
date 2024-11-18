#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "ft_irc.hpp"

class Channel
{
	public:
		Channel();
		Channel(std::string);
		Channel(Channel const &);
		~Channel();
		Channel & operator=(Channel const &);

		// getters

		// setters

		// channel actions
			// add client
			// rm client

	private:
		std::string _name;
		Client *_admin;
		// add clients
};

#endif