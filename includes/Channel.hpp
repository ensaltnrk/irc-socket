#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "irc.hpp"
# include "User.hpp"

class Channel {
	private:
		std::string		_channelName;
		userVector		_channelOperator;
		userVector		_channelUser;
		std::string		_channelMode;
		bool			_isDeleted;

	public:
		Channel(User* user, std::string name);
		~Channel();

		void			joinNewUser(User* user);
		bool			isOperator(std::string nickname);
		bool			isUserInChannel(std::string nickname);

		std::string		getUserList();
		userVector		getOperatorVector();
		userVector		getNormalUserVector();
		std::string		getChannelName();
		std::string		getChannelMode();
		bool			getIsDeleted();

		void			setChannelMode(std::string channelMode);
		void			setIsDeleted(bool isDeleted);

		void			deleteNormalUser(std::string nickname);
		int				deleteOperatorUser(std::string nickname);
};

typedef	std::vector<Channel*>			channelVector;
typedef	std::vector<Channel*>::iterator	channelIter;

#endif
