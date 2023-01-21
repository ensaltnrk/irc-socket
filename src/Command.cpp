# include "../includes/Server.hpp"

void	Server::commandPING(User* user, stringVector& parameters) {
	if (user->getIsVerified() != ALL_VERIFIED) throw std::runtime_error(Error(ERR_NOTREGISTERED));
	if (parameters.size() < 2) throw std::runtime_error(Error(ERR_NOORIGIN));
	else if (parameters.size() > 2) throw std::runtime_error(Error(ERR_NOSUCHSERVER, parameters[1]));

	sendMessage(user, "PONG 127.0.0.1 :" + parameters[1]);
}

void	Server::commandQUIT(User* user, stringVector& parameters) {
	std::string		quitUserHostname = user->getHostname();
	std::string		quitUserNickname = user->getNickname();
	std::string		quitMsg = ft_getMsgString(parameters, 0);
	stringVector&	channelList = user->getChannelList();

	if (user->getIsVerified() && !user->getIsSigquit())
		sendMessage(user, ErrorReply(":Closing Link:", quitUserHostname, "(Quit: " + quitMsg + ")"));
	for (stringIter iter = channelList.begin(); iter < channelList.end(); iter++) {
		if (!isChannel(*iter)) continue;
		Channel* ch = findChannel(*iter);
		sendMessageBroadcast(1, ch, user, "QUIT :" + quitMsg);
		if (ch->isOperator(quitUserNickname)) {
			if (ch->deleteOperatorUser(user->getNickname())) {
				sendMessageBroadcast(1, ch, user, "MODE " + ch->getChannelName() + " +o :" + (*ch->getOperatorVector().begin())->getNickname());
			} else {
				ch->setIsDeleted(true);
			}
		}
		else ch->deleteNormalUser(quitUserNickname);
	}
	user->setIsDisconnected(true);
}
