# include "../includes/Server.hpp"
# include <fcntl.h>

Server::Server(char* port, char* password) {
	struct pollfd	server_pollfd;

	_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (_serverSocket < 0)
		throw std::runtime_error(Error(ERR_SERVEROPENFAILED, "socket"));

	_port = atoi(port);
	if (_port <= 0)
		throw std::runtime_error("No such port available");
	_password = password;
	memset(&_serverAddress, 0, sizeof(_serverAddress));
	_serverAddress.sin_family = AF_INET;
	_serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	_serverAddress.sin_port = htons(_port);
	server_pollfd.fd = _serverSocket;
	server_pollfd.events = POLLIN;
	_poll_fds.push_back(server_pollfd);

	if (bind(_serverSocket, (struct sockaddr*)&_serverAddress, sizeof(_serverAddress)) < 0)
		throw std::runtime_error(Error(ERR_SERVEROPENFAILED, "bind"));
}

Server::~Server(void) {
	serverOff();
}

void	Server::serverOn(void) {
	pollFdIter	iter;

	if (listen(_serverSocket, 5) < 0)
		throw std::runtime_error(Error(ERR_SERVEROPENFAILED, "listen"));

	while (true) {
		if (!poll(_poll_fds.data(), _poll_fds.size(), 1000)) continue ;
		if (_poll_fds[0].revents & POLLIN) {
			acceptClient();
			continue;
		}
		for (iter = _poll_fds.begin() + 1; iter < _poll_fds.end(); iter++) {
			if (iter->revents & POLLHUP) {
				stringVector	closeParams;
				User*	quitUser = findUser(iter->fd);

				closeParams.push_back(":Connection closed");
				quitUser->setIsSigquit(true);
				commandQUIT(quitUser, closeParams);
				continue ;
			}
			else if (iter->revents & POLLIN) {
				if (isServerUser(iter->fd))
					receiveClientMessage(iter->fd);
				else
					receiveFirstClientMessage(iter->fd);
			}
			if (iter->revents & POLLNVAL) _poll_fds.erase(iter);
		}
		disconnectClients();
		deleteChannel();
	}
}

void	Server::serverOff(void) {
	close(_serverSocket);
	exit(0);
}

void	Server::acceptClient(void) {
	struct sockaddr_in	client_addr;
	int					clientSocket;
	struct pollfd		client_pollfd;
	size_t				client_len = sizeof(client_addr);

	try {
		clientSocket = accept(_serverSocket, (struct sockaddr*)&client_addr, (socklen_t*)&client_len);
		if (clientSocket < 0)
			throw std::runtime_error(Error(ERR_CLIENTCONNECTFAILED));
		fcntl(clientSocket, F_SETFL, O_NONBLOCK);
		client_pollfd.fd = clientSocket;
		client_pollfd.events = POLLIN;
		_poll_fds.push_back(client_pollfd);
	}
	catch (std::exception& e) {
		std::cout << e.what() << "\n";
		close(clientSocket);
	}
}

void	Server::receiveFirstClientMessage(int clientFd) {
	User*	user;

	try {
		user = new User();
		user->setSocketFd(clientFd);
		std::string	fullMsg = concatMessage(clientFd);
		parseMessageStream(user, fullMsg);
		_serverUser.push_back(user);
	}	catch (std::exception& e) {
		std::cout << e.what() << "\n";
		close(clientFd);
	}
}

void	Server::receiveClientMessage(int clientSocket) {
	try {
		std::string	fullMsg = concatMessage(clientSocket);
		User*		user = findUser(clientSocket);
		parseMessageStream(user, fullMsg);
	} catch (std::exception& e) {
		std::cout << e.what() << "\n";
	}
}

std::string	Server::concatMessage(int clientSocket) {
	int			message_length;
	std::string	fullMsg = "";

	if ((message_length = recv(clientSocket, _message, BUF_SIZE, 0)) < 0)
		throw std::runtime_error("Error on recv");
	_message[message_length] = 0;	
	fullMsg += _message;
	if (fullMsg.length() >= 2 && !fullMsg.substr(fullMsg.length() - 2).compare("\r\n"))
		ft_replaceStr(fullMsg, "\r", " ");
	return fullMsg;
}

void	Server::parseMessageStream(User* user, const std::string& fullMsg) {
	stringVector	commands = ft_split(fullMsg, '\n');
	stringIter		cmdIter;

	std::cout << "======Message======\n" << fullMsg << std::endl;

	for (cmdIter = commands.begin(); cmdIter != commands.end(); cmdIter++) {
		stringVector	parameters = ft_split(*cmdIter, ' ');
		std::string		command = ft_upperCaseString(parameters[0]);

		if (!command.compare(CMD_PASS)) commandPASS(user, parameters);
		else if (!command.compare(CMD_NICK)) commandNICK(user, parameters);
		else if (!command.compare(CMD_USER)) commandUSER(user, parameters);
		else if (!command.compare(CMD_PING)) commandPING(user, parameters);
		else if (!command.compare(CMD_MODE)) commandMODE(user, parameters);
		else if (!command.compare(CMD_JOIN)) commandJOIN(user, parameters);
		else if (!command.compare(CMD_MSG)) commandMSG(user, parameters);
		else if (!command.compare(CMD_NOTICE)) commandNOTICE(user, parameters);
		else if (!command.compare(CMD_NAMES)) commandNAMES(user, parameters);
		else if (!command.compare(CMD_PART)) commandPART(user, parameters);
		else if (!command.compare(CMD_QUIT)) commandQUIT(user, parameters);
		else if (!command.compare(CMD_KICK)) commandKICK(user, parameters);
		else sendMessage(user, Error(ERR_UNKNOWNCOMMAND, parameters[0]));
	}
}

void	Server::deleteChannel() {
	channelIter	channelIter = _channelList.begin();

	while (channelIter < _channelList.end()) {
		if ((*channelIter)->getIsDeleted()) {
			delete *channelIter;
			channelIter = _channelList.erase(channelIter);
		}
		channelIter++;
	}
}

void	Server::disconnectClients() {
	userIter	userIter = _serverUser.begin();
	pollFdIter	pollIter;

	while (userIter < _serverUser.end()) {
		if (!(*userIter)->getIsDisconnected()) {
			userIter++;
			continue;
		}
		close((*userIter)->getSocketFd());
		pollIter = _poll_fds.begin() + 1;
		while (pollIter < _poll_fds.end()) {
			if ((*userIter)->getSocketFd() == pollIter->fd) {
				_poll_fds.erase(pollIter);
				break ;
			}
			pollIter++;
		}
		delete *userIter;
		userIter = _serverUser.erase(userIter);
	}
}
