#pragma once
#ifndef CHANNEL_HPP
# define CHANNEL_HPP
# include "mainHeader.hpp"

class User;

class Channel {
	private:
		int								_userLimit;
		std::string						_name;
		std::string						_topic;
		std::string						_password;
		std::vector<std::string>		_inviteList;
		std::map<std::string, bool>		_modes;
		std::map<std::string, User*>	_users;
		std::map<std::string, User*>	_operators;

	public:
		int								getUserLimit() const;
		bool							getModes(std::string mode) const;
		bool							isOperator(std::string userNickname) const;
		bool							isUserInvited(const std::string userNickname) const;
		bool							removeUser(const std::string& nickname);
		void							addUser(User* user);
		void							setMode(std::string mode, bool value);
		void							setName(const std::string& name);
		void							promoteToOperator(std::string userNickname); //add to operator, remove from user
		void							demoteFromOperator(std::string userNickname); //add to user, remove from operator
		void							listUsers() const;
		void							listOperators() const;
		void							setMode(const std::string& mode);
		void							setPassword(std::string& password);
		void							setTopic(const std::string& topic);
		void							setUserLimit(int limit);
		std::vector<User*>				getAllUsers() const;
		std::string 					getName() const;
		std::string						getPassword() const;
		std::string						getTopic() const;
		std::map<std::string, User*>	getOperators() const;
		std::map<std::string, User*>	getNonOperators() const;

		Channel(const std::string& name);
		~Channel();
};

#endif