SRC = mode.cpp main.cpp server.cpp channel.cpp client.cpp nick.cpp \
	  privmsg.cpp Topic.cpp WhoCmd.cpp Parsing.cpp

NAME = ft_irc

OBJ = $(SRC:.cpp=.o)

INC = ./Include

FLAG = -Werror -Wall -Wextra -std=c++98 -g3

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
