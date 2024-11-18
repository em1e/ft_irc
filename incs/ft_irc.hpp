
#ifndef FT_IRC_H
# define FT_IRC_H

# include <iostream>
# include <cstring>

// bonus
# ifndef BONUS
#  define BONUS 0
# endif

enum struct Command {
	KICK,
	INVITE,
	TOPIC,
	MODE
};

typedef struct s_irc
{
	int i;
}	t_irc;

#endif