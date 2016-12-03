# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vroche <vroche@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2015/03/16 10:49:38 by vroche            #+#    #+#              #
#    Updated: 2016/12/03 12:20:43 by vroche           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ft_irc

NAME_CLIENT = ft_nm

NAME_SERVEUR = ft_otool

LIB = -L./libft -lft

HDR = -I./libft/includes -I./includes

FLAGS = -Wall -Wextra -Werror -g

C_CLIENT =	srcs/irc_client.c

O_CLIENT = $(C_CLIENT:.c=.o)

C_SERVEUR =	srcs/irc_serveur.c

O_SERVEUR = $(C_SERVEUR:.c=.o)

C = $(C_CLIENT) $(C_SERVEUR) $(C_COMMON)

O = $(O_CLIENT) $(O_SERVEUR) $(O_COMMON)

all: $(NAME)

$(NAME): $(NAME_CLIENT) $(NAME_SERVEUR)

$(NAME_CLIENT): $(O_CLIENT) 
	make -C ./libft
	clang $(FLAGS) $(HDR) $(LIB) $^ -o $@

$(NAME_SERVEUR): $(O_SERVEUR)
	make -C ./libft
	clang $(FLAGS) $(HDR) $(LIB) $^ -o $@

%.o:%.c
	clang $(FLAGS) $(HDR) -c $< -o $@

clean:
	make -C ./libft clean
	rm -f $(O)

fclean: clean
	make -C ./libft fclean
	rm -f $(NAME_CLIENT) $(NAME_SERVEUR)

re: fclean all

.PHONY: all clean fclean re libft
