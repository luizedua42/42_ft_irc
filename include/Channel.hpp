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
		void				promoteToOperator(User user);
		void				demoteFromOperator(User user);
		std::list<User>		getOperators();
		std::list<User>		getNonOperators();
};

#endif