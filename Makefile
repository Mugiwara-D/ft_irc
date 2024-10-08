SRC = source/channel.cpp source/client.cpp source/main.cpp source/mode.cpp \
	  source/nick.cpp source/parssing.cpp source/privmsg.cpp \
	  source/Rpl_senders.cpp source/server.cpp source/utils.cpp 

NAME = ft_irc

OBJ = $(SRC:.cpp=.o)

INC = ./include

FLAG = -Werror -Wall -Wextra -std=c++98

CC = c++ 

all : $(NAME) 

$(NAME) : $(OBJ) 
	$(CC) $(OBJ) -o $(NAME)
	
%.o : %.cpp
	$(CC) -I$(INC) $(FLAG) -c $< -o $@


clean :
	rm -f $(OBJ)

fclean:
	rm -f $(OBJ) $(NAME)

re : fclean all

.PHONY: all clean fclean re
