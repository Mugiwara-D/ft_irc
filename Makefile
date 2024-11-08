SRC_DIR = source
OBJ_DIR = obj
INC_DIR = ./Include

SRC = $(wildcard $(SRC_DIR)/*.cpp)
NAME = ft_irc

CC = c++
CFLAGS = -Werror -Wall -Wextra -std=c++98 -g3 -I$(INC_DIR)

OBJ = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC))
DEPS = $(OBJ:.o=.d)

GREEN = \033[32m
RED = \033[31m
YELLOW = \033[33m
RESET = \033[0m

all: $(NAME)

$(NAME): $(OBJ)
	@echo "$(YELLOW)[Linking]$(RESET) $@"
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
	@echo "$(GREEN)[Build Complete]$(RESET)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	@echo "$(YELLOW)[Compiling]$(RESET) $<"
	$(CC) $(CFLAGS) -MMD -c $< -o $@

-include $(DEPS)

clean:
	@echo "$(RED)[Cleaning]$(RESET)"
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

VALGRIND_ARGS = 
valgrind: $(NAME)
	@echo "$(YELLOW)[Running Valgrind]$(RESET)"
	valgrind --leak-check=full --track-origins=yes ./$(NAME) $(VALGRIND_ARGS)

.PHONY: all clean fclean re valgrind
