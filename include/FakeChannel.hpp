#ifndef FAKECHANNEL_HPP
# define FAKECHANNEL_HPP
# include "includes.hpp"

class FakeChannel {
	public:
		int						_number;
		std::string				_topic;
		std::string 			_mode; //enum?
		std::vector<FakeUser>	_users;
		std::vector<FakeUser>	_operators;
};

#endif