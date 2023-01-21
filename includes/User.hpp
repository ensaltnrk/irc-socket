#ifndef USER_HPP
# define USER_HPP

# include "irc.hpp"

class User {
	private:
		int					_clientFd;
		struct sockaddr_in*	_clientAddress;
		socklen_t*			_clientAddressSize;
		std::string			_nickname;
		std::string			_username;
		std::string			_hostname;
		stringVector		_channelList;
		short				_isVerified;
		bool				_isDisconnected;
		std::string			_userMode;
		bool				_isWelcomed;
		bool				_isSigquit;

	public:
		User();
		User(const User& instance);
		~User();
		User&				operator=(const User& instance);

		int					getSocketFd() const;
		struct sockaddr_in*	getAddressPtr() const;
		socklen_t*			getAddressSizePtr() const;
		std::string			getNickname() const;
		std::string			getUsername() const;
		std::string			getHostname() const;
		short				getIsVerified() const;
		bool				getIsDisconnected() const;
		std::string			getUserMode() const;
		stringVector&		getChannelList();
		bool				getIsWelcomed() const;
		bool				getIsSigquit() const;

		void				setNickname(std::string nickname);
		void				setUsername(std::string username);
		void				setHostname(std::string hostname);
		void				setIsVerified(short what);
		void				setSocketFd(int clientFd);
		void				setIsDisconnected(bool isDisconnected);
		void				setUserMode(std::string userMode);
		void				setIsWelcomed(bool isWelcomed);
		void				setIsSigquit(bool isSigquit);

		void				addJoinedChannel(std::string channelName);
		void				deleteJoinedChannel(std::string channelName);

};

std::ostream&	operator<<(std::ostream& out, const User& instance);

typedef	std::vector<User*>				userVector;
typedef	std::vector<User*>::iterator	userIter;

#endif
