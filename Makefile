# Directories
SRC_DIR = source
OBJ_DIR = obj
INC_DIR = ./Include

# Source files and target binary
SRC = $(wildcard $(SRC_DIR)/*.cpp)
NAME = ft_irc

# Compiler and flags
CC = c++
CFLAGS = -Werror -Wall -Wextra -std=c++98 -g3 -I$(INC_DIR)

# Object files and dependency files
OBJ = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC))
DEPS = $(OBJ:.o=.d)

# Colors for output
GREEN = \033[32m
RED = \033[31m
YELLOW = \033[33m
RESET = \033[0m

# Default rule
all: $(NAME)

# Build the executable
$(NAME): $(OBJ)
	@echo "$(YELLOW)[Linking]$(RESET) $@"
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
	@echo "$(GREEN)[Build Complete]$(RESET)"

# Compile source files to object files with dependencies
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	@echo "$(YELLOW)[Compiling]$(RESET) $<"
	$(CC) $(CFLAGS) -MMD -c $< -o $@

# Include generated dependency files
-include $(DEPS)

# Clean object files
clean:
	@echo "$(RED)[Cleaning]$(RESET)"
	rm -rf $(OBJ_DIR)

# Clean all, including the executable
fclean: clean
	rm -f $(NAME)

# Rebuild everything
re: fclean all

# Valgrind target for memory leak checks
VALGRIND_ARGS = 
valgrind: $(NAME)
	@echo "$(YELLOW)[Running Valgrind]$(RESET)"
	valgrind --leak-check=full --track-origins=yes ./$(NAME) $(VALGRIND_ARGS)

# Use parallel build with -j flag (optional)
.PHONY: all clean fclean re valgrind

