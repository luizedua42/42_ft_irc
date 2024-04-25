#ifndef CHANNEL_HPP
# define CHANNEL_HPP
# include "includes.hpp"

class Channel {
	private:
		int					_number;
		std::string			_topic;
		std::string 		_mode; //enum?
		std::map<User>		_users;
		std::map<User>		_operators;

	public:
		void				promoteToOperator(User user); //add to operator, remove from user
		void				demoteFromOperator(User user); //add to user, remove from operator
		std::map<User>		getOperators();
		std::map<User>		getNonOperators();
		Channel();
		Channel(std::string name);
		~Channel();
};

#endif