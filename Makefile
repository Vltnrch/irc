# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vroche <vroche@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2015/03/16 10:49:38 by vroche            #+#    #+#              #
#    Updated: 2016/12/08 17:04:01 by vroche           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ft_irc

NAME_CLIENT = client

NAME_SERVER = serveur

LIB = -L./libft -lft ~/.brew/opt/readline/lib/libreadline.a -ltermcap

HDR = -I./libft/includes -I./includes -I/Users/vroche/.brew/opt/readline/include

FLAGS = -Wall -Wextra -Werror -g

C_CLIENT =	srcs/client/irc_client.c \
			srcs/client/ircc_select.c \
			srcs/client/ircc_cmd.c 

O_CLIENT = $(C_CLIENT:.c=.o)

C_SERVER =	srcs/server/irc_server.c \
			srcs/server/ircs_select.c \
			srcs/server/ircs_cmd.c 

O_SERVER = $(C_SERVER:.c=.o)

C_COMMON =	srcs/common/circular_buffer.c \
			srcs/common/manage.c \
			srcs/common/ft_spacesplit.c

O_COMMON = $(C_COMMON:.c=.o)

C = $(C_CLIENT) $(C_SERVER) $(C_COMMON)

O = $(O_CLIENT) $(O_SERVER) $(O_COMMON)

all: $(NAME)

$(NAME): $(NAME_CLIENT) $(NAME_SERVER)

$(NAME_CLIENT): $(O_COMMON) $(O_CLIENT) 
	make -C ./libft
	clang $(FLAGS) $(HDR) $(LIB) $^ -o $@

$(NAME_SERVER): $(O_COMMON) $(O_SERVER)
	make -C ./libft
	clang $(FLAGS) $(HDR) $(LIB) $^ -o $@

%.o:%.c
	clang $(FLAGS) $(HDR) -c $< -o $@

clean:
	make -C ./libft clean
	rm -f $(O)

fclean: clean
	make -C ./libft fclean
	rm -f $(NAME_CLIENT) $(NAME_SERVER)

re: fclean all

.PHONY: all clean fclean re libft
