# Colour codes
GREEN	=	\033[0;32m
CYAN	=	\033[0;36m
CYAN_B	=	\033[1;96m

NAME	=	ircserv
CC		=	c++
FLAGS	=	-Wall -Wextra -Werror -std=c++98
INCLUDE	=	-I includes
RM		=	rm -rf

SRC_D 	=	srcs/
SRC		=	main
SRCS	=	$(addprefix  $(SRC_D), $(addsuffix .cpp, $(SRC)))

OBJ_D 	=	objs/
OBJ		=	$(SRC:%.c=%.o)
OBJS	=	$(addprefix $(OBJ_D), $(addsuffix .o, $(OBJ)))

$(OBJ_D)%.o : $(SRC_D)%.cpp
			@mkdir -p $(OBJ_D)
			@($(CC) $(FLAGS) $(INCLUDE) -c $< -o $@)

all:		$(NAME)

$(NAME):	$(OBJS)
			@($(CC) $(FLAGS) $(INCLUDE) $(OBJS) -o $(NAME))
			@echo "$(CYAN) [ OK ] | $(CYAN_B)$(NAME)$(END) $(CYAN)ready!$(END)"

clean:
			@($(RM) $(OBJ_D))
			@echo "$(GREEN) [ OK ] | Objects removed!$(END)"

fclean: 	clean
			@($(RM) $(NAME))
			@echo "$(GREEN) [ OK ] | Everything is clean!$(END)"

re: 		fclean all

.PHONY:		all clean fclean re
