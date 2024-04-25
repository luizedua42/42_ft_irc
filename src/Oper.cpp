/**======================
*            42sp
* @file      : Oper.cpp
* @author    : luizedua
* @email     : luizedua@student.42sp.org.br
* @createdOn : 25/04/2024
*========================**/

#include "../include/includes.hpp"


std::vector<std::string> Oper::parseOptions(std::string str) {
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

void Oper::selectOptions(std::string buff) {
	int i;

	void (Oper::*fct_ptr[10])(std::vector<std::string>) = { 
		&Oper::user, &Oper::nick, &Oper::join, &Oper::privmsg, &Oper::quit,
		&Oper::oper, &Oper::mode, &Oper::topic, &Oper::invite, &Oper::kick
	};

	std::string option = buff.substr(0, buff.find(" "));
	std::string requests[] = {"USER", "NICK", "JOIN", "PRIVMSG", "QUIT", "OPER", "MODE", "TOPIC", "INVITE", "KICK"};
	
	for(i = 0; i < 10 && requests[i] != option; i++);
	
	if(i == 10) {
		std::cerr << "Invalid request: " << option << std::endl;
		i = 0;
		return;
	}

	(this->*fct_ptr[i])(parseOptions(buff.substr(buff.find(" ") + 1)));
	i = 0;
}

void Oper::join(std::vector<std::string> options) {
	std::string channel = options[0];

	std::cout << "Joining channel: " << channel << std::endl;
}

void Oper::privmsg(std::vector<std::string> options) {
	std::cout << "Sending message to channel: " << options[0] << " - " << std::endl;
}

void Oper::quit(std::vector<std::string> options) {
	std::string channel= options[0];

	std::cout << "Quitting" << channel << std::endl;
}

void Oper::nick(std::vector<std::string> option) {
	std::string nickname = option[0].substr(0, option[0].find('\r'));

	std::cout << "Changing nickname to: " << nickname << std::endl;
}

void Oper::user(std::vector<std::string> option) {
	std::string username = option[0];

	std::cout << "Setting username to: " << username << std::endl;
}

void Oper::oper(std::vector<std::string> option) {
	std::cout << "Opering user: " << option[0] << std::endl;
}

void Oper::mode(std::vector<std::string> option) {
	std::string channel = option[0];
	std::string mode = option[1];

	std::cout << "Setting mode: " << mode << " in channel: " << channel << std::endl;
}

void Oper::topic(std::vector<std::string> option) {
	std::cout << "Setting topic in channel: " << option[0] << std::endl;
}

void Oper::invite(std::vector<std::string> option) {
	std::cout << "Inviting user to channel: " << option[0] << std::endl;
}

void Oper::kick(std::vector<std::string> option) {
	std::string channel = option[0].substr(0, option[0].find('\r'));
	std::string user = option[1];
	std::cout << "Kicking " << user << " from channel: " << channel << std::endl;
}