#ifndef RESPONSES_HPP
# define RESPONSES_HPP

//reply and error numeric codes
# define RPL_WELCOMENBR std::string(" 001 ")
# define RPL_UMODEISNBR std::string(" 221 ")
# define RPL_AWAYNBR  std::string(" 301 ")
# define RPL_ENDOFWHONBR std::string(" 315 ")
# define RPL_CHANNELMODEISNBR std::string(" 324 ")
# define RPL_NOTOPICNBR std::string(" 331 ")
# define RPL_TOPICNBR std::string(" 332 ")
# define RPL_INVITINGNBR std::string(" 341 ")
# define RPL_WHOREPLYNBR std::string(" 352 ")
# define RPL_BANLISTNBR std::string(" 367 ")
# define RPL_ENDOFBANLISTNBR std::string(" 368 ")
# define ERR_NOSUCHNICKNBR std::string(" 401 ")
# define ERR_NOSUCHSERVERNBR std::string(" 402 ")
# define ERR_NOSUCHCHANNELNBR std::string(" 403 ")
# define ERR_CANNOTSENDTOCHANNBR std::string(" 404 ")
# define ERR_TOOMANYCHANNELSNBR std::string(" 405 ")
# define ERR_TOOMANYTARGETSNBR std::string(" 407 ")
# define ERR_NORECIPIENTNBR std::string(" 411 ")
# define ERR_NOTEXTTOSENDNBR std::string(" 412 ")
# define ERR_NOTOPLEVELNBR std::string(" 413 ")
# define ERR_WILDTOPLEVELNBR std::string(" 414 ")
# define ERR_NONICKNAMEGIVENNBR std::string(" 431 ")
# define ERR_ERRONEUSNICKNAMENBR std::string(" 432 ")
# define ERR_NICKNAMEINUSENBR std::string(" 433 ")
# define ERR_NICKCOLLISIONNBR std::string(" 436 ")
# define ERR_NOTONCHANNELNBR std::string(" 442 ")
# define ERR_USERONCHANNELNBR std::string(" 443 ")
# define ERR_NEEDMOREPARAMSNBR std::string(" 461 ")
# define ERR_ALREADYREGISTEREDNBR std::string(" 462 ")
# define ERR_PASSWDMISMATCHNBR std::string(" 464 ")
# define ERR_KEYSETNBR std::string(" 467 ")
# define ERR_CHANNELISFULLNBR std::string(" 471 ")
# define ERR_UNKNOWNMODENBR std::string(" 472 ")
# define ERR_INVITEONLYCHANNBR std::string(" 473 ")
# define ERR_BANNEDFROMCHANNBR std::string(" 474 ")
# define ERR_BADCHANMASKNBR std::string(" 476 ")
# define ERR_CHANOPRIVSNEEDEDNBR std::string(" 482 ")
# define ERR_UMODEUNKNOWNFLAGNBR std::string(" 501 ")
# define ERR_USERSDONTMATCHNBR std::string(" 502 ")

//reply messages
# define RPL_ENDOFWHO std::string(" :End of /WHO list ")
# define RPL_NOTOPIC std::string(" :No topic is set ")
# define RPL_ENDOFBANLIST std::string(" :End of channel ban list ")
# define RPL_WELCOME std::string(" :Welcome to the Internet Relay Chat ")

//error messages
# define ERR_ACCEPT std::string(" Error on accept ")
# define ERR_ALREADYREGISTRED std::string(" :You may not reregister")
# define ERR_BADCHANMASK std::string(" :Invalid channel name ")
# define ERR_BANNEDFROMCHAN std::string(" :Cannot join channel (+b) ")
# define ERR_BIND std::string(" Error on bind ")
# define ERR_CANNOTSENDTOCHAN std::string(" :Cannot send to channel ")
# define ERR_CHANNELISFULL std::string(" :Cannot join channel (+l) ")
# define ERR_CHANOPRIVSNEEDED std::string(" :You're not channel operator ")
# define ERR_ERRONEUSNICKNAME std::string(" :Erroneus nickname ")
# define ERR_FCNTL std::string(" Error on fcntl ")
# define ERR_INVITEONLYCHAN std::string(" :Cannot join channel (+i) ")
# define ERR_KEYSET std::string(" :Channel key already set ")
# define ERR_LISTEN std::string(" Error on listen ")
# define ERR_NEEDMOREPARAMS std::string(" :Not enough parameters ")
# define ERR_NICKCOLLISION std::string(" :Nickname collision KILL ")
# define ERR_NICKNAMEINUSE std::string(" :Nickname is already in use ")
# define ERR_NONICKNAMEGIVEN std::string(" :No nickname given ")
# define ERR_NORECIPIENT std::string(" :No recipient given ")
# define ERR_NOSUCHCHANNEL std::string(" :No such channel ")
# define ERR_NOSUCHNICK std::string(" :No such nick/channel ")
# define ERR_NOSUCHSERVER std::string(" :No such server ")
# define ERR_NOTEXTTOSEND std::string(" :No text to send ")
# define ERR_NOTONCHANNEL std::string(" :You're not on that channel ")
# define ERR_NOTOPLEVEL std::string(" :No toplevel domain specified ")
# define ERR_PASSWDMISMATCH std::string(" :Password incorrect ")
# define ERR_POLL std::string(" Error on poll ")
# define ERR_RECV std::string(" Error on recv ")
# define ERR_SETSOCKOPT std::string(" Error on setsockopt ")
# define ERR_SOCKET std::string(" Error creating socket ")
# define ERR_TOOMANYCHANNELS std::string(" :You have joined too many \                         channels ")
# define ERR_TOOMANYTARGETS std::string(" :Duplicate recipients. No message delivered ")
# define ERR_UMODEUNKNOWNFLAG std::string(" :Unknown MODE flag ")
# define ERR_UNKNOWNMODE std::string(" :is unknown mode char to me ")
# define ERR_USER std::string(" Error on User ")
# define ERR_USERONCHANNEL std::string(" :is already on channel ")
# define ERR_USERSDONTMATCH std::string(" :Cant change mode for other users ")
# define ERR_WILDTOPLEVEL std::string(" :Wildcard in toplevel domain ")



#endif