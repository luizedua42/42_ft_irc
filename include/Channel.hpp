#pragma once
#ifndef CHANNEL_HPP
# define CHANNEL_HPP
# include "includes.hpp"

class User;

class Channel {
	private:
		int								_userLimit;
		std::string						_name;
		std::string						_topic;
		std::string						_password;
		std::map<std::string, bool>		_modes;
		std::map<std::string, User*>	_Users;
		std::map<std::string, User*>	_operators;

	public:
		int								getUserLimit() const;
		bool							getModes(std::string mode) const;
		void							setMode(std::string mode, bool value);
		void							addUser(User* user);
		void							setName(const std::string& name);
		void							promoteToOperator(std::string userNickname); //add to operator, remove from user
		void							demoteFromOperator(std::string userNickname); //add to user, remove from operator
		void							listUsers() const;
		void							listOperators() const;
		void							setMode(const std::string& mode);
		void							setPassword(std::string& password);
		void							setTopic(const std::string& topic);
		void							setUserLimit(int limit);
		std::string 					getName() const;
		std::string						getPassword() const;
		std::string						getTopic() const;
		std::map<std::string, User*>	getOperators() const;
		std::map<std::string, User*>	getNonOperators() const;

		Channel(const std::string& name);
		~Channel();
};

#endif