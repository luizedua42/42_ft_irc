/**======================
*            42sp
* @file      : Oper.hpp
* @author    : luizedua
* @email     : luizedua@student.42sp.org.br
* @createdOn : 25/04/2024
*========================**/
#pragma once
#ifndef OPER_HPP
# define OPER_HPP
# include "includes.hpp"


class Oper {
	public:
		void						selectOptions(std::string buff);
		std::vector<std::string>	parseOptions(std::string buff);


		void		join(std::vector<std::string>);
		void		privmsg(std::vector<std::string>);
		void		quit(std::vector<std::string>);
		void		nick(std::vector<std::string>);
		void		user(std::vector<std::string>);
		void		oper(std::vector<std::string>);
		void		mode(std::vector<std::string>);
		void		topic(std::vector<std::string>);
		void		invite(std::vector<std::string>);
		void		kick(std::vector<std::string>);
};

#endif