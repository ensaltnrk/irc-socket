# include "../includes/Server.hpp"

int main(int argc, char* argv[]) {
	if (argc != 3) {
		std::cerr << "Usage : <port> <password>" << std::endl;
		return (1);
	}
	std::cout << "Welcome To The SadoMazo IRC server." << std::endl;
	std::cout << "To enter the server please type the password {PASS ...}" << std::endl;
	try {
		Server server = Server(argv[1], argv[2]);
		server.serverOn();
	} catch (std::exception& e) {
		std::cout << "ircserv: Error: " << e.what() << "\n";
		return (1);
	}
	return (0);
}
