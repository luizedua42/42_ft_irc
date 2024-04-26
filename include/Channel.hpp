#ifndef CHANNEL_HPP
# define CHANNEL_HPP
# include "includes.hpp"

class Channel {
	private:
		int					_number;
		std::string			_name;
		std::string			_topic;
		std::string 		_mode; //enum?
		std::map<std::string, Client*> _clients; // Map client name to Client reference
    	std::map<std::string, Client*> _operators; // Map client name to Client reference

	public:
		void 							addClient(Client* client);
		std::map<std::string, Client*>	getOperators();
		std::map<std::string, Client*>	getNonOperators();
		void							promoteToOperator(std::string clientNickname); //add to operator, remove from user
		void							demoteFromOperator(std::string clientNickname); //add to user, remove from operator
		void 							listUsers() const;
		void 							listOperators() const;

		Channel(const char* name);
		Channel(const char* name, std::string topic);
		~Channel();
};

#endif