#ifndef CHANNEL_HPP
# define CHANNEL_HPP
# include "includes.hpp"

class Channel {
	public:
		int					_number;
		std::string			_topic;
		std::string 		_mode; //enum?
		std::list<User>		_users;
		std::list<User>		_operators;

	private:
		void				promoteToOperator(User user); //add to operator, remove from user
		void				demoteFromOperator(User user); //add to user, remove from operator
		std::list<User>		getOperators();
		std::list<User>		getNonOperators();
};

#endif