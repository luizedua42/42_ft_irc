#ifndef CHANNEL_HPP
# define CHANNEL_HPP
# include "includes.hpp"

class Channel {
	private:
		std::string			_name;
		std::string			_topic;
		std::string 		_mode;
		std::map<std::string, User*> _Users;
    	std::map<std::string, User*> _operators;

	public:
		void 							addUser(User* user);
		std::map<std::string, User*>	getOperators() const;
		std::map<std::string, User*>	getNonOperators() const;
		std::string 					getName() const;
		void 							setName(const std::string& name);
		void							promoteToOperator(std::string userNickname); //add to operator, remove from user
		void							demoteFromOperator(std::string userNickname); //add to user, remove from operator
		void 							listUsers() const;
		void 							listOperators() const;

		Channel(const char* name);
		~Channel();
};

#endif