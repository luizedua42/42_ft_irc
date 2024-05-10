/**======================
*            42sp
* @file      : Oper.cpp
* @author    : luizedua
* @email     : luizedua@student.42sp.org.br
* @createdOn : 25/04/2024
*========================**/

#include "../headers/mainHeader.hpp"

namespace mode {
	void setOp(Channel* channel, std::string user) {
		std::cout << "Setting operator: " << user << std::endl;
		channel->promoteToOperator(user);
	}
	void unsetOp(Channel* channel, std::string user) {
		std::cout << "Unsetting operator: " << user << std::endl;
		channel->demoteFromOperator(user);
	}
	void setTopic(Channel* channel) {
		channel->setMode("t", true);
	}
	void unsetTopic(Channel* channel) {
		channel->setMode("t", false);
	}
	void setInvite(Channel* channel) {
		channel->setMode("i", true);
	}
	void unsetInvite(Channel* channel) {
		channel->setMode("i", false);
	}
	void setKey(Channel* channel, std::string key) {
		channel->setPassword(key);
	}
	void unsetKey(Channel* channel) {
		std::string key = "";
		channel->setPassword(key);
	}
	void setLimit(Channel* channel, std::string param) {
		std::stringstream ss(param);
		int limit;
		ss >> limit;
		channel->setUserLimit(limit);
	}
	void unsetLimit(Channel* channel) {
		channel->setUserLimit(MAX_USERS);
	}
	void unknownCommand(std::string command, int userFD) {
		std::string response = command + ":Unknown command\r\n";
		//PROBLEM WITH THIS SEND, CLIENT DOES NOT GET THE RESPONSE
		if(send(userFD, response.c_str(), response.size(), 0) == -1)
			std::cerr << "Error sending message" << std::endl;
	}
}

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

void Server::selectOptions(std::string buff, int userFD) {
	std::vector<std::string> splittedBuff = splitBuff(buff);
	User* client = Server::getUser(userFD);
	std::string requests[] = {"CAP", "USER", "NICK", "JOIN", "PRIVMSG", "QUIT", "MODE", "TOPIC", "INVITE", "KICK", "WHO", "PASS", "PART"};

	do{
		int i = 0;
		std::string options = splittedBuff[0].substr(0, splittedBuff[0].find_first_of(" "));
		std::cout << "buff: " << buff << std::endl;
		for(; i < 13; i++) {
			if(options == requests[i])
				break;
		}
		if(client->getIsAuth() == false) {
			if (i != 0 && i != 1 && i != 2 && i != 11) {
				std::string response = "You have not registered\r\n";
				std::cout << "Sending response: " << response << std::endl;
				send(userFD, response.c_str(), response.size(), 0);
				return;
			}
		}
		std::string parsedOptions = splittedBuff[0].substr(splittedBuff[0].find_first_of(" ") + 1);
		std::cout << "options: " << options << std::endl;
		switch (i) {
			case 0:
				cap(userFD);
				break;
			case 1:
				user(parseOptions(parsedOptions), userFD);
				break;
			case 2:
				nick(parseOptions(parsedOptions), userFD);
				break;
			case 3:
				join(parseOptions(parsedOptions), userFD);
				break;
			case 4:
				privmsg(parseOptions(parsedOptions), userFD);
				break;
			case 5:
				quit(parseOptions(parsedOptions), userFD);
				break;
			case 6:
				mode(parseOptions(parsedOptions), userFD);
				break;
			case 7:
				topic(parseOptions(parsedOptions), userFD);
				break;
			case 8:
				invite(parseOptions(parsedOptions), userFD);
				break;
			case 9:
				kick(parseOptions(parsedOptions), userFD);
				break;
			case 10:
				who(parseOptions(parsedOptions), userFD);
				break;
			case 11:
				pass(parseOptions(parsedOptions), userFD);
				break;
			case 12:
				part(parseOptions(parsedOptions), userFD);
				break;
			default:
				mode::unknownCommand(options, userFD);
				break;
		}
		splittedBuff.erase(splittedBuff.begin());
		} while (!splittedBuff.empty());
}

std::string messageCat(std::vector<std::string> options) {
	std::string message;
	if (options[1].find(':') == 0) {
		message = options[1].substr(1) + " ";
		for (size_t i = 2; i < options.size(); i++) {
			message += options[i] + " ";
		}
		return message;
	}
	message = options[1].substr(0) + " ";
	for (size_t i = 2; i < options.size(); i++) {
		message += options[i] + " ";
	}
	return message;
}








