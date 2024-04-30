/**======================
*            42sp
* @file      : Oper.cpp
* @author    : luizedua
* @email     : luizedua@student.42sp.org.br
* @createdOn : 25/04/2024
*========================**/

#include "../include/includes.hpp"

std::vector<std::string> Server::parseOptions(std::string str) {
	std::string word;
	std::stringstream ss(str);
	std::vector<std::string> splitted;
	while (std::getline(ss, word, ' ')) {
		if(word.find('\r') != std::string::npos)
			splitted.push_back(word.substr(0, word.find('\r')));
		else
			splitted.push_back(word);
	}
	return splitted;
}

std::vector<std::string> splitBuff(std::string buff) {
	std::vector<std::string> splittedBuff;
	std::string word;
	std::stringstream ss(buff);
	while (std::getline(ss, word, '\n')) {
		splittedBuff.push_back(word);
	}
	return splittedBuff;
}

void Server::selectOptions(std::string buff, int clientFd) {
	std::vector<std::string> splittedBuff = splitBuff(buff);
	
	std::string requests[] = {"CAP", "USER", "NICK", "JOIN", "PRIVMSG", "QUIT", "OPER", "MODE", "TOPIC", "INVITE", "KICK"};
	
	do{
		int i = 0;
		std::string option = splittedBuff[0].substr(0, splittedBuff[0].find_first_of(" "));
		std::cout << "buff: " << splittedBuff[0] << std::endl;
		for(; i < 10; i++) {
			if(option == requests[i])
				break;
		}

		std::string parsedOptions = splittedBuff[0].substr(splittedBuff[0].find_first_of(" ") + 1);
		std::cout << "option: " << option << std::endl;
		switch (i) {
			case 0:
				cap(clientFd);
				break;
			case 1:
				user(parseOptions(parsedOptions), clientFd);
				break;
			case 2:
				nick(parseOptions(parsedOptions), clientFd);
				break;
			case 3:
				join(parseOptions(parsedOptions), clientFd);
				break;
			case 4:
				privmsg(parseOptions(parsedOptions), clientFd);
				break;
			case 5:
				quit(parseOptions(parsedOptions), clientFd);
				break;
			case 6:
				oper(parseOptions(parsedOptions), clientFd);
				break;
			case 7:
				mode(parseOptions(parsedOptions), clientFd);
				break;
			case 8:
				topic(parseOptions(parsedOptions), clientFd);
				break;
			case 9:
				invite(parseOptions(parsedOptions), clientFd);
				break;
			case 10:
				kick(parseOptions(parsedOptions), clientFd);
				break;
			default:
				std::cerr << "Invalid request: " << option << std::endl;
				break;
		}
		splittedBuff.erase(splittedBuff.begin());
		} while (!splittedBuff.empty());
}

void Server::cap(int clientFd) {
	(void)clientFd;
	
	std::cout << "CAP" << std::endl;
	// send(clientFd, "\r\n", 2, 0);
}
void Server::join(std::vector<std::string> options, int clientFd) {
	Client& user = Server::getClient(clientFd);
	std::string channel = options[0];
	std::cout << user.getNickName() << " Joining channel: " << channel << std::endl;
}

void Server::privmsg(std::vector<std::string> options, int clientFd) {
	std::cout << "Sending message to channel: " << options[0] << " - " << std::endl;\
	(void)clientFd;

}

void Server::quit(std::vector<std::string> options, int clientFd) {
	std::string channel= options[0];
	Client& test = Server::getClient(clientFd);
	
	std::cout << test.getNickName() <<":" <<"Quittin" << channel << std::endl;
}

void Server::nick(std::vector<std::string> option, int clientFd) {
	Client& user = Server::getClient(clientFd);

	std::string nickname = option[0].substr(0, option[0].find('\r'));

	user.setNickName(nickname);

	std::string response;
	response = ":ft.irc 001 " + user.getNickName() + " :Welcome to the Internet Relay Chat " + user.getNickName() + "!" + user.getRealName() + "@*\r\n";
	send(clientFd, response.c_str(), response.size(), 0);
}

void Server::user(std::vector<std::string> option, int clientFd) {
	Client& user = Server::getClient(clientFd);

	std::string username = option[0].substr(0, option[0].find(' '));

	user.setNickName(username);
}


void Server::oper(std::vector<std::string> option, int clientFd) {
	(void)clientFd;
	std::cout << "Opering user: " << option[0] << std::endl;
}

void Server::mode(std::vector<std::string> option, int clientFd) {
	std::string channel = option[0];
	std::string mode = option[1];
	(void)clientFd;

	std::cout << "Setting mode: " << mode << " in channel: " << channel << std::endl;
}

void Server::topic(std::vector<std::string> option, int clientFd) {
	(void)clientFd;
	std::cout << "Setting topic in channel: " << option[0] << std::endl;
}

void Server::invite(std::vector<std::string> option, int clientFd) {
	(void)clientFd;
	std::cout << "Inviting user to channel: " << option[0] << std::endl;
}

void Server::kick(std::vector<std::string> option, int clientFd) {
	(void)option;
	(void)clientFd;
//	std::string channel = option[0].substr(0, option[0].find('\r'));
//	std::string user = option[1];
// std::cout << "Kicking " << user << " from channel: " << channel << std::endl;
}