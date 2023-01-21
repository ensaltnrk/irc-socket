# include "../includes/Server.hpp"

void	Server::commandPASS(User* user, stringVector& parameters) {
	if (parameters.size() != 2) throw std::runtime_error(Error(ERR_NEEDMOREPARAMS, CMD_PASS));
	if (_password != parameters[1]) throw std::runtime_error(Error(ERR_PASSWDMISMATCH));

	user->setIsVerified(PASS_VERIFIED);
	std::cout << "Please type your nickname {NICK ...}" << std::endl;
}

void	Server::commandNICK(User* user, stringVector& parameters) {
	userIter	iter;

	if (!(user->getIsVerified() & PASS_VERIFIED)) throw std::runtime_error(Error(ERR_NOTREGISTERED));
	if (parameters.size() != 2) throw std::runtime_error(Error(ERR_NEEDMOREPARAMS, CMD_NICK));
	if (parameters[1].length() <= 0 || parameters[1].length() > 9 || !ft_isValidNickname(parameters[1]))
		throw std::runtime_error(Error(ERR_ERRONEUSNICKNAME, parameters[1]));

	for (iter = _serverUser.begin(); iter < _serverUser.end(); iter++) {
		if (!(*iter)->getNickname().compare(parameters[1]))
			throw std::runtime_error(Error(ERR_NICKNAMEINUSE, parameters[1]));
	}
	user->setNickname(parameters[1]);
	user->setIsVerified(NICK_VERIFIED);
	checkIsVerified(user);
}

void	Server::commandUSER(User* user, stringVector& parameters) {
	if (!(user->getIsVerified() & PASS_VERIFIED))
		throw std::runtime_error(Error(ERR_NOTREGISTERED));
	if (parameters.size() < 3 || parameters[1].length() <= 0)
		throw std::runtime_error(Error(ERR_NEEDMOREPARAMS, CMD_NICK));

	user->setUsername(parameters[1]);
	user->setHostname(parameters[2]);
	user->setIsVerified(USER_VERIFIED);
	checkIsVerified(user);
}

void	Server::checkIsVerified(User* user) {
	char	checkBuf[1024] = {0, };

	if (user->getIsVerified() != ALL_VERIFIED) return;
	if (user->getIsWelcomed()) return;
	if (recv(user->getSocketFd(), checkBuf, 1024, 0) == 0) return;
	sendMessage(user,
		"001 " + user->getNickname() + " :\033[1;32mWelcome to the " + SERVER_NAME + "\e[0m " + \
		user->getNickname() + "." + user->getUsername() + "@" + user->getHostname()
	);
	sendMessage(user,
		"002 " + user->getNickname() + " :\033[1;32mYour host is " + SERVER_NAME + ", " + "running version 0.1\e[0m"
	);
	sendMessage(user,
		"003 " + user->getNickname() + " :\033[1;32m" + SERVER_NAME + " 0.1\e[0m"
	);
	user->setIsWelcomed(true);
}
