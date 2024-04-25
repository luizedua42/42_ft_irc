#include "../include/Channel.hpp"

void Channel::promoteToOperator(User user) {

} //add to operator, remove from user

void Channel::demoteFromOperator(User user){
	
} //add to user, remove from operator

std::map<User> Channel::getOperators(){
	
}

std::map<User> Channel::getNonOperators(){
	
}

Channel::Channel(void) : _number(), _topic("default"), _mode("standard"), _users(), _operators() {
	
}

Channel::Channel(std::string name) {
	
}

Channel::~Channel(void) {}
