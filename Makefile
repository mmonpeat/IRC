NAME = ircserv
INC = inc/
SRC = main.cpp

BUILD = .build
OBJ = $(SRC:%.cpp=$(BUILD)/%.o)
DEP = $(OBJ:%.o=%.d)

CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98 -fsanitize=address

all:

$(BUILD)/%.o: %cpp Makefile
	mkdir -p $(BUILD)
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