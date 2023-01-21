# include "../includes/Server.hpp"

void	Server::sendMessage(User* user, std::string str) {
	int			fd = user->getSocketFd();
	std::string	strToSend = ":" + user->getNickname() + "!" + user->getUsername()  + "@" + user->getHostname() + " " + str + "\r\n";

	if (send(fd, (strToSend).c_str(), strToSend.length(), 0) < 0)
		throw std::runtime_error(Error(ERR_MESSAGESENDFAILED));
}

void	Server::sendMessage(User* sender, User* receiver, std::string str) {
	int			fd = receiver->getSocketFd();
	std::string	strToSend = ":" + sender->getNickname() + "!" + sender->getUsername() + "@" + sender->getHostname() + " " + str + "\r\n";

	if (send(fd, (strToSend).c_str(), strToSend.length(), 0) < 0)
		throw std::runtime_error(Error(ERR_MESSAGESENDFAILED));
}

void	Server::sendMessageBroadcast(int mode, Channel* ch, User* sender, std::string str) {
	userIter	it;
	userVector	operUsers = ch->getOperatorVector();
	userVector	normUsers = ch->getNormalUserVector();

	for (it = operUsers.begin(); it < operUsers.end(); it++) {
		if (mode == 0 || (*it)->getNickname().compare(sender->getNickname()))
			sendMessage(sender, *it, str);
	}
	for (it = normUsers.begin(); it < normUsers.end(); it++) {
		if (mode == 0 || (*it)->getNickname().compare(sender->getNickname()))
			sendMessage(sender, *it, str);
	}
}
