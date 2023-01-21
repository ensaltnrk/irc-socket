#ifndef IRC_HPP
# define IRC_HPP

# include <iostream>
# include <cstring>
# include <cctype>
# include <cstdlib>
# include <string>
# include <vector>
# include <sstream>
# include <unistd.h>

# include <sys/socket.h>
# include <sys/types.h>
# include <sys/poll.h>
# include <sys/stat.h>
# include <arpa/inet.h>
# include <netinet/in.h>

# define CMD_PASS "PASS"
# define CMD_NICK "NICK"
# define CMD_USER "USER"
# define CMD_MODE "MODE"
# define CMD_JOIN "JOIN"
# define CMD_NAMES "NAMES"
# define CMD_MSG "PRIVMSG"
# define CMD_PART "PART"
# define CMD_PING "PING"
# define CMD_PONG "PONG"
# define CMD_QUIT "QUIT"
# define CMD_KICK "KICK"
# define CMD_NOTICE "NOTICE"

# define NICK_AVAILABLE_CHAR "[]\\`_^{}|"
# define CHANNEL_AVAILABLE_CHAR "&#+!"

typedef	std::vector<std::string>				stringVector;
typedef	std::vector<std::string>::iterator		stringIter;
typedef	std::vector<struct pollfd>				pollFdVector;
typedef	std::vector<struct pollfd>::iterator	pollFdIter;

stringVector	ft_split(std::string input, char delimiter);
void			ft_replaceStr(std::string& str, std::string src, std::string des);
bool			ft_isValidNickname(const std::string& str);
bool			ft_isValidChannelName(const std::string& str);
std::string 	ft_getMsgString(stringVector parameters, int idx);
std::string		ft_upperCaseString(const std::string& str);


#endif
