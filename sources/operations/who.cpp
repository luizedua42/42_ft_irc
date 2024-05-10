#include "../headers/mainHeader.hpp"

void Server::who(std::vector<std::string> options, int userFD) {
	(void)userFD;
	#define RPL_WHOREPLY(channel, user, nick, flags, realname)  (":ft.irc 352 " + channel + " " + user + " 42sp.org.br ft.irc " + nick + " " + flags + ":0 " + realname + "\r\n");
	std::string response;

	std::string channelName = options[0];
	Channel* channelPtr = getChannel(channelName);
	std::vector<User *> users = channelPtr->getAllUsers();
	for (size_t i = 0; i < users.size(); i++) {
		std::string nick = users[i]->getNickName();
		std::string user = users[i]->getRealName();
		std::string realName = ":realname";
		std::string flags = "H ";
		// if ((*it).isOperator(users[i]))
		flags += "@ ";
		response += RPL_WHOREPLY(channelName, user, nick, flags, realName);
	}
	std::cout << "Sending response: " << response << std::endl;
	response += ":ft.irc 315 " + channelName + " :End of /WHO list\r\n";
	std::cout << "Sending response: " << response << std::endl;
	if(send (userFD, response.c_str(), response.size(), 0) == -1)
		std::cerr << "Error sending message" << std::endl;
}
