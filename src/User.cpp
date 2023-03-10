# include "../includes/User.hpp"

User::User(void) {
	_clientAddress = new sockaddr_in();
	_clientAddressSize = new socklen_t(sizeof(_clientAddress));
	_isVerified = 0b000;
	_isDisconnected = false;
	std::cout << "User created (" << this << ")\n";
	_isWelcomed = false;
	_isSigquit = false;
}

User::~User(void) {
	delete _clientAddress;
	delete _clientAddressSize;
	std::cout << *this << " User deleted (" << this << ")\n";
}

User::User(const User& instance) {
	*this = instance;
}

User&	User::operator=(const User& instance) {
	_clientFd = instance.getSocketFd();
	_clientAddress = new sockaddr_in(*instance.getAddressPtr());
	_clientAddressSize = new socklen_t(*instance.getAddressSizePtr());
	_nickname = instance.getNickname();
	_username = instance.getUsername();
	_hostname = instance.getHostname();
	_isDisconnected = instance.getIsDisconnected();
	_isVerified = instance.getIsVerified();
	_isWelcomed = instance.getIsWelcomed();
	_isSigquit = instance.getIsSigquit();
	std::cout << *this << " user copied (" << this << ")\n";
	return *this;
}

int	User::getSocketFd() const {
	return _clientFd;
}

struct sockaddr_in*	User::getAddressPtr() const {
	return _clientAddress;
}

socklen_t*	User::getAddressSizePtr() const {
	return _clientAddressSize;
}

std::string	User::getNickname() const {
	return _nickname;
}

std::string	User::getUsername() const {
	return _username;
}

std::string	User::getHostname() const {
	return _hostname;
}

short	User::getIsVerified() const {
	return _isVerified;
}

bool	User::getIsDisconnected() const {
	return _isDisconnected;
}

std::string	User::getUserMode() const {
	return _userMode;
}

stringVector&	User::getChannelList() {
	return _channelList;
}

bool	User::getIsWelcomed() const {
	return _isWelcomed;
}

bool	User::getIsSigquit() const {
	return _isSigquit;
}

void	User::setNickname(std::string nickname) {
	_nickname = nickname;
}

void	User::setUsername(std::string username) {
	_username = username;
}

void	User::setHostname(std::string hostname) {
	_hostname = hostname;
}

void	User::setSocketFd(int clientFd) {
	_clientFd = clientFd;
}

void	User::setIsVerified(short what) {
	_isVerified |= what;
}

void	User::setIsDisconnected(bool isDisconnected) {
	_isDisconnected = isDisconnected;
}

void	User::setUserMode(std::string userMode) {
	_userMode = userMode;
}

void	User::setIsWelcomed(bool isWelcomed) {
	_isWelcomed = isWelcomed;
}

void	User::setIsSigquit(bool isSigquit) {
	_isSigquit = isSigquit;
}

void	User::addJoinedChannel(std::string channelName) {
	_channelList.push_back(channelName);
}

void	User::deleteJoinedChannel(std::string channelName) {
	stringIter iter;

	for (iter = _channelList.begin(); iter < _channelList.end(); iter++) {
		if (*iter == channelName)
			_channelList.erase(iter);
	}
}

std::ostream& operator<<(std::ostream& out, const User& instance) {
	out << "User: [" << instance.getNickname() << "] (" << instance.getUsername() << ")";
	return out;
}
