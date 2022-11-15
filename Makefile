NAME			=	ircserv
CXX				=	c++
CXXFLAGS		=	-Wall -Wextra -std=c++98 #-Werror
DEBUG			=	-g3


SRCS_DIR		=	srcs/
SRCS_FILES		=	main.cpp \
					server.cpp
SRCS			=	$(addprefix $(SRCS_DIR), $(SRCS_FILES))


OBJS_DIR		=	objs/
OBJS_FILES		=	$(SRCS_FILES:.cpp=.o)
OBJS			=	$(addprefix $(OBJS_DIR), $(OBJS_FILES))


INC_DIR			=	incs/

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJS)
		$(CXX) -o $@ $(OBJS)

$(OBJS): $(SRCS)
		mkdir -p $(OBJS_DIR)
		$(CXX) $(CXXFLAGS) -I$(INC_DIR) $(DEBUG) -c $< -o $@

clean:
	rm -rf $(OBJS_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all
