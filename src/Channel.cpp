# include "../includes/Channel.hpp"

Channel::Channel(User* user, std::string channelName) {
	_channelOperator.push_back(user);
	_channelName = channelName;
	_channelMode = "+nt";
	_isDeleted = false;
}

Channel::~Channel() {}

void	Channel::joinNewUser(User* user) {
	_channelUser.push_back(user);
}

bool	Channel::isOperator(std::string nickname) {
	userIter	iter;

	for (iter = _channelOperator.begin(); iter < _channelOperator.end(); iter++) {
		if (!((*iter)->getNickname().compare(nickname)))
			return true;
	}
	return false;
}

bool	Channel::isUserInChannel(std::string nickname) {
	userIter	iter;
	for (iter = _channelUser.begin(); iter < _channelUser.end(); iter++) {
		if (!(*iter)->getNickname().compare(nickname))
			return true;
	}
	return false;
}

std::string			Channel::getUserList() {
	std::string	userList;
	userIter	iter;

	userList = "= " + _channelName + " :";
	for (iter = _channelUser.begin(); iter < _channelUser.end(); iter++)
		userList += ((*iter)->getNickname() + " ");
	for (iter = _channelOperator.begin(); iter < _channelOperator.end(); iter++)
		userList += ("@" + (*iter)->getNickname() + " ");
	return userList;
}

userVector	Channel::getOperatorVector() {
	return _channelOperator;
}

userVector	Channel::getNormalUserVector() {
	return _channelUser;
}

std::string	Channel::getChannelName() {
	return _channelName;
}

std::string	Channel::getChannelMode() {
	return _channelMode;
}

bool	Channel::getIsDeleted() {
	return _isDeleted;
}


void	Channel::setChannelMode(std::string channelMode) {
	_channelMode = channelMode;
}

void	Channel::setIsDeleted(bool isDeleted) {
	_isDeleted = isDeleted;
}

void	Channel::deleteNormalUser(std::string nickname){
	userIter	iter;

	for (iter = _channelUser.begin(); iter < _channelUser.end(); iter++) {
		if (!(*iter)->getNickname().compare(nickname))
			_channelUser.erase(iter);
	}
}

int		Channel::deleteOperatorUser(std::string nickname) {
	userIter	iter;

	for (iter = _channelOperator.begin(); iter < _channelOperator.end(); iter++) {
		if (!(*iter)->getNickname().compare(nickname))
			_channelOperator.erase(iter);
	}

	if (_channelOperator.size() == 0) {
		if (!_channelUser.empty()) {
			User	*user = _channelUser[0];
			_channelOperator.push_back(user);
			_channelUser.erase(_channelUser.begin());
			return 1;
		} else {
			return 0;
		}
	}

	return (_channelOperator.size());
}
