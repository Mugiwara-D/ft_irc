SRC = main.cpp server.cpp channel.cpp client.cpp nick.cpp

NAME = ft_irc

OBJ = $(SRC:.cpp=.o)

INC = ./Include

FLAG = -Werror -Wall -Wextra -std=c++98

CC = c++ 

all : $(NAME) 

$(NAME) : $(OBJ) 
	$(CC) $(FLAG) $(OBJ) -o $(NAME)
	
%.o : %.cpp
	$(CC) $(FLAG) -c $< -o $@


clean :
	rm -f $(OBJ)

fclean:
	rm -f $(OBJ) $(NAME)

re : fclean all

.PHONY: all clean fclean re
