#ifndef SERVER_HPP
# define SERVER_HPP

# include "irc.hpp"
# include "User.hpp"
# include "Reply.hpp"
# include "Channel.hpp"

# define BUF_SIZE 1024
# define SERVER_NAME "SADOMAZO"

# define PASS_VERIFIED 1
# define NICK_VERIFIED 2
# define USER_VERIFIED 4
# define ALL_VERIFIED 7

class Server {
	private:
		int					_port;
		std::string			_password;
		int					_serverSocket;
		struct sockaddr_in	_serverAddress;
		char				_message[BUF_SIZE];
		struct timeval		_timeout;
		fd_set				_reads;
		pollFdVector		_poll_fds;
		userVector			_serverUser;
		channelVector		_channelList;

		void		acceptClient(void);

		void		sendMessage(User* user, std::string str);
		void		sendMessage(User* sender, User* receiver, std::string str);
		void		sendMessageBroadcast(int mode, Channel* ch, User* sender, std::string str);

		void		receiveFirstClientMessage(int clientFd);
		void		receiveClientMessage(int clientSocket);
		std::string	concatMessage(int clientSocket);
		void		parseMessageStream(User* user, const std::string& fullMsg);
		void		setUserDisconnectByFd(int client_fd);
		void		deleteChannel();
		void		disconnectClients();

		void		commandPASS(User* user, stringVector& parameters);
		void		commandNICK(User* user, stringVector& parameters);
		void		commandUSER(User* user, stringVector& parameters);
		void		commandPING(User* user, stringVector& parameters);
		void		commandJOIN(User* user, stringVector& parameters);
		void		commandNAMES(User* user, stringVector& parameters);
		void		commandMSG(User* user, stringVector& parameters);
		void		commandNOTICE(User* user, stringVector& parameters);
		void		commandMODE(User* user, stringVector& parameters);
		void		commandPART(User* user, stringVector& parameters);
		void		commandQUIT(User* user, stringVector& parameters);
		void		commandKICK(User* user, stringVector& parameters);

		bool		isChannel(std::string channelName);
		bool		isServerUser(std::string nickname);
		bool		isServerUser(int clientFd);
		Channel*	findChannel(std::string channelName);
		User*		findUser(std::string nickname);
		User*		findUser(int clientFd);
		void		checkIsVerified(User* user);
		void		kickUserFromChannel(Channel* ch, User* user, std::string message);

	public:
		Server(char* port, char* password);
		~Server();

		void		serverOn(void);
		void		serverOff(void);
};

#endif
