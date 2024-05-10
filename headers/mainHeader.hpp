/**======================
*            42sp
* @file      : mainHeader.hpp
* @author    : luizedua
* @email     : luizedua@student.42sp.org.br
* @createdOn : 15/04/2024
*========================**/
#pragma once
#ifndef INCLUDES_HPP
# define INCLUDES_HPP
# define MAX_USERS 666

# include <algorithm>
# include <arpa/inet.h>
# include <csignal>
# include <cstdio>
# include <cstdlib>
# include <cstring>
# include <errno.h>
# include <fcntl.h>
# include <iostream>
# include <limits>
# include <list>
# include <map>
# include <netinet/in.h>
# include <poll.h>
# include <sstream>
# include <string>
# include <sys/types.h>
# include <sys/socket.h>
# include <unistd.h>
# include <vector>
# include "Channel.hpp"
# include "error.hpp"
# include "responses.hpp"
# include "Server.hpp"
# include "User.hpp"

void	splitString(std::string& str, char delimiter, std::vector<std::string>& tokens);

#endif
