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
	(void)clientFd;
	std::string channel = options[0];
	std::cout << " Joining channel: " << channel << std::endl;
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

	user.setRealName(username);
}


void Server::oper(std::vector<std::string> option, int clientFd) {
	(void)clientFd;
	std::cout << "Opering user: " << option[0] << std::endl;
}

void Server::mode(std::vector<std::string> option, int clientFd) {
	if(option.size() < 3) {
		std::cerr << "Invalid number of arguments" << std::endl;
		return;
	}
	(void)clientFd;
	int i = 0;
	std::string modes[] = {"-i", "+i", "-t", "+t", "-k", "+k", "-o", "+o", "-l", "+l"};
	std::string channel = option[0];
	std::string mode = option[1].substr(0, option[1].find('\r'));
	std::string user = option[2].substr(0, option[2].find('\r'));
	std::cout << "Setting mode: " << mode << " in channel: " << channel << " to user: " << user << std::endl;
	for(; i < 10; i++) {
		if(mode == modes[i])
			break;
	}
	switch (i) {
		case 0:
			std::cout << "Removing invite only mode" << std::endl;
			break;
		case 1:
			std::cout << "Setting invite only mode" << std::endl;
			break;
		case 2:
			std::cout << "Removing topic protection mode" << std::endl;
			break;
		case 3:
			std::cout << "Setting topic protection mode" << std::endl;
			break;
		case 4:
			std::cout << "Removing key" << std::endl;
			break;
		case 5:
			std::cout << "Setting key" << std::endl;
			break;
		case 6:
			std::cout << "Removing operator status" << std::endl;
			break;
		case 7:
			std::cout << "Setting operator status" << std::endl;
			break;
		case 8:
			std::cout << "Removing user limit" << std::endl;
			break;
		case 9:
			std::cout << "Setting user limit" << std::endl;
			break;
		default:
			std::cerr << "Invalid mode: " << mode << std::endl;
			break;
	}

}

void Server::topic(std::vector<std::string> option, int clientFd) {
	(void)clientFd;
	std::string channel = option[0];
	std::string topic = option[1];
	if (option.size() == 1) {
		std::cout << "Getting topic of channel: " << channel << std::endl;
	} else if (option.size() == 2) {
		std::cout << "Setting topic of channel: " << channel << " to: " << topic << std::endl;
	} else {
		std::cerr << "Invalid number of arguments" << std::endl;
	}
}

void Server::invite(std::vector<std::string> option, int clientFd) {
	(void)clientFd;
	std::cout << "Inviting user to channel: " << option[0] << std::endl;
}

void Server::kick(std::vector<std::string> option, int clientFd) {
	(void)clientFd;
	std::string channel = option[0];
	std::string target = option[1];

}