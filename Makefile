# E X E C U T A B L E  N A M E #

NAME			=	ircserv


# C X X  F L A G S #

CXX				=	c++
CXXFLAGS		=	-Wall -Wextra -std=c++98  #-Werror
DEBUG			=	-g3


# S O U R C E  F I L E S #

SRCS_DIR		=	srcs/
SRCS_FILES		=	main.cpp \
					Server.cpp \
					ServerHandle.cpp \
					User.cpp \
					Channel.cpp \
					Utils.cpp \
					Commands.cpp
SRCS			=	$(addprefix $(SRCS_DIR), $(SRCS_FILES))


# O B J .  F I L E S #

OBJS_DIR		=	objs/
OBJS_FILES		=	$(SRCS_FILES:.cpp=.o)
OBJS			=	$(addprefix $(OBJS_DIR), $(OBJS_FILES))


# I N C .  F I L E S #

INC				=	-I incs/
INC_DIR			=	incs/
INC_FILES		=	headers.hpp \
					server.hpp \
					User.hpp \
					Channel.hpp \
					Utils.hpp \
					Commands.hpp
INCS			=	$(addprefix $(INC_DIR), $(INC_FILES))


#  B U I L D  R U L E S #

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJS)
		$(CXX) -o $@ $(OBJS)

$(OBJS_DIR)%.o: $(SRCS_DIR)%.cpp $(INCS)
		mkdir -p $(OBJS_DIR)
		$(CXX) $(INC) $(CXXFLAGS) $(DEBUG) -c $< -o $@


# C L E A N  &  O T H E R  R U L E S #

clean:
	rm -rf $(OBJS_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all
