NAME	=	ircserv_bot
SRCS	=	Bot.cpp BotMain.cpp BotUtils.cpp
HEADER	=	Bot.hpp BotUtils.hpp
OBJS	=	$(SRCS:.cpp=.o)
COMPIL	=	c++
FLAGS	=	-g3 -std=c++98 -Wall -Wextra -Werror
DEBUGFLAGS	= -g3 -std=c++98 -Wall -Wextra -Werror


# $< = dependances | $@ = regles -> .o

%.o:	%.cpp
	$(COMPIL) $(FLAGS) -c $< -o $@

all:	$(NAME)

$(NAME): $(OBJS) $(HEADER)
	$(COMPIL) $(FLAGS) $(OBJS) -o $@

debug:
	$(COMPIL) $(DEBUGFLAGS) $(OBJS) -o $@

clean:
	rm -rf $(OBJS)

fclean:	clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
