#include "../headers/mainHeader.hpp"

void splitString(std::string& str, char delimiter, std::vector<std::string>& tokens) {
	std::stringstream ss(str);
	std::string token;
	while (ss.eof() && std::getline(ss, token, delimiter)) {
		tokens.push_back(token);
	}
}