# /**======================
# *            42sp
# * @file      : Makefile
# * @author    : luizedua
# * @email     : luizedua@student.42sp.org.br
# * @createdOn : 06/02/2024
# *========================**/


NAME=ircserv
ECHO=echo
YELLOW=\033[1;33m
GREEN=\033[1;32m
RED=\033[0;31m
RESET=\033[0m
C++ = c++
CFLAGS = -Wall -Werror -Wextra -g3 -std=c++98

# ./src/Server.cpp ./src/Client.cpp 
HEADER_FILES = ./include/includes.hpp ./include/Server.hpp ./include/Client.hpp
SOURCES      = ./src/main.cpp ./src/Server.cpp ./src/Client.cpp
OBJ_DIR      = ./mandatory/obj
OBJ_FILES    = $(SRC_FILES:%.c=$(OBJ_DIR)/%.o)
INCLUDE =	-I ./include

all : ${NAME}

${NAME}: ${SOURCES}
	@${ECHO} "${YELLOW}Compiling $@...${RESET}"
	@c++ -g3 -Wall -Wextra -Werror -std=c++98 -I include ${SOURCES} -o $@
	@${ECHO} "${GREEN}Compiled!${RESET}"


$(OBJ_DIR)/%.o: %.c $(HEADER_FILES)
	@mkdir -p $(dir $@)
	@$(C++) $(CFLAGS) -c $< -o $@ $(INCLUDE)


clean:
	@rm -fr ${OBJ_DIR}
	@${ECHO} "${RED}Objects removed!${RESET}"

fclean: clean
	@rm -fr ${NAME}
	@${ECHO} "${RED}Files removed!${RESET}"

re: fclean ${NAME}

.PHONY: all clean fclean re