#ifndef CHANNEL_HPP
# define CHANNEL_HPP
# include "includes.hpp"

class Channel {
	private:
		int			_number;
		std::string	_topic;
		std::string _mode; //enum?
		&User		_operators[];
}

#endif