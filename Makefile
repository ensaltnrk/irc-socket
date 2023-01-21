SRCS		=	src/main.cpp \
				src/Server.cpp src/ServerSendMessage.cpp src/ServerUtils.cpp \
				src/User.cpp src/Channel.cpp \
				src/Command.cpp src/CommandChannel.cpp src/CommandAuth.cpp \
				src/utils.cpp src/Reply.cpp

C++	=	c++ -Wall -Wextra -Werror -std=c++98

NAME		=	ircserv

all : $(NAME)

$(NAME) : $(SRCS)
	@$(C++) $(SRCS) -o $(NAME)

clean :
	@rm -rf $(NAME)

fclean : clean

re : fclean
	make

.PHONY: all clean fclean re
