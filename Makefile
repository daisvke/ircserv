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
INC_FILES		=	headers.hpp \
					server.hpp
HEADERS			=	$(addprefix $(INC_DIR), $(INC_FILES))

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJS)
		$(CXX) -o $@ $(OBJS)

$(OBJS): $(SRCS) $(HEADERS)
		mkdir -p $(OBJS_DIR)
		$(CXX) $(CXXFLAGS) $(DEBUG) -c $< -o $@

clean:
	rm -rf $(OBJS_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all


# NAME			=	ircserv
# CXX				=	c++
# CXXFLAGS		=	-Wall -Wextra -std=c++98 #-Werror
# DEBUG			=	-g3

# #
# #	F I L E S
# #

# #	Source files

# SRCS_DIR		=	srcs/ 
# SRCS_FILES		=	main.cpp \
# 					server.cpp
# SRCS			=	$(addprefix $(SRCS_DIR), $(SRCS_FILES))

# #	OBJ. files
# OBJS_DIR		=	objs/
# OBJS_FILES		=	$(SRCS:.cpp=.o)
# OBJS			=	$(addprefix $(OBJS_DIR), $(OBJS_FILES))


# #	INC. files
# #	INC. files
# INC_DIR				=	inc/
# INC_FILES			=	cub3d.h cub3d_parser.h get_next_line.h
# HEADERS				=	$(addprefix $(INC_DIR), $(INC_FILES))


# # INCS				= -I inc/

# .PHONY: all clean fclean re

# all: $(NAME)

# $(OBJS_DIR)%.o: $(SRCS_DIR)%.c $(HEADERS)
# 	@mkdir -p $(OBJS_DIR)
# 	$(CXX) $(CXXFLAGS) $(HEADERS) -o $@ -c $<

# $(NAME): $(OBJS)
# 		$(CXX) -o $@ $(OBJS)

# clean:
# 	rm -rf $(OBJS_DIR)

# fclean: clean
# 	rm -f $(NAME)

# re: fclean all
