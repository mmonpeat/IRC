# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kate <kate@student.42.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/02 13:22:55 by kkoval            #+#    #+#              #
#    Updated: 2025/08/04 18:44:47 by amagnell         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME = ircserv
INC = inc/
SRC = src/main.cpp src/Server.cpp src/Client.cpp src/Channel.cpp src/join.cpp src/utilsJoin.cpp src/command_utils.cpp src/modes.cpp src/topic.cpp

BUILD = .build
OBJ = $(SRC:%.cpp=$(BUILD)/%.o)
DEP = $(OBJ:%.o=%.d)

CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98 -fsanitize=address

all: $(NAME)

$(BUILD)/%.o: %.cpp Makefile
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -I $(INC) -MMD -MP -c $< -o $@
	@echo "Created $@"

$(NAME) : $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
	@echo "Created $(NAME)"

-include $(DEP)

clean:
	rm -rf $(BUILD)
	@echo "deleted build"

fclean: clean
	rm -rf $(NAME)
	@echo "$(NAME) deleted"

re: fclean all

.PHONY : all, clean, fclean, re
.SILENT:
