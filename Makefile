# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/12/22 14:06:43 by lumenthi          #+#    #+#              #
#    Updated: 2018/05/31 17:58:23 by lumenthi         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = fdf

CC = gcc
FLAGS = -Wall -Werror -Wextra -g
MLX_FLAGS = -L ./minilibx/ -l mlx -framework OpenGL -framework Appkit

LIBDIR = libft
SRCDIR = srcs
INCDIR = includes libft minilibx
OBJDIR = objs
MLXDIR = minilibx

MLX = $(MLXDIR)/libmlx.a

LIBFT = $(LIBDIR)/libft.a
LIBFT_OBJ = $(LIBDIR)/objs

SRCS = fdf.c

SOURCES = $(addprefix $(SRCDIR)/, $(SRCS))
INCLUDES = $(addprefix -I, $(INCDIR))
OBJS = $(addprefix $(OBJDIR)/, $(SRCS:.c=.o))

vpath %.c srcs

GREEN = '\033[4;32m'
RED = '\033[4;31m'
BLANK = '\033[0m'
YELLOW = '\033[4;33m'

TICK = '\033[1;32m~\033[0m'
CROSS = '\033[1;31mx\033[0m'

.PHONY : all clean fclean re

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT) $(MLX)
	@ $(CC) $(LIBFT) $(MLX_FLAGS) -o $(NAME) $(OBJS)
	@ printf " %b | Compiled %b%b%b\n" $(TICK) $(GREEN) $(NAME) $(BLANK)

$(OBJDIR)/%.o: %.c
	@ mkdir -p $(OBJDIR)
	@ $(CC) -c $(FLAGS) $(INCLUDES) -o $@ $<

$(LIBFT): $(LIBFT_OBJ)
	@ make -sC $(LIBDIR)

$(LIBFT_OBJ):
	@ make -sC $(LIBDIR)

$(MLX):
	@ make -sC $(MLXDIR)

clean:
	@ make -sC $(MLXDIR) clean
	@ make -C $(LIBDIR) clean
	@ test -d $(OBJDIR) && \
	rm -rf $(OBJDIR) && \
	printf " %b | " $(TICK) && \
	printf "Removed %b%b%b object folder\n" $(YELLOW) $(NAME) $(BLANK) \
	|| (printf " %b | " $(CROSS) && \
	printf "No %b%b%b object folder\n" $(YELLOW) $(NAME) $(BLANK))

fclean: clean
	@ test -f $(LIBFT) && \
	rm -rf $(LIBFT) && \
	printf " %b | " $(TICK) && \
	printf "Removed %blibft%b library\n" $(RED) $(BLANK) \
	|| (printf " %b | " $(CROSS) && \
	printf "No %blibft%b library\n" $(RED) $(BLANK))
	@ test -f $(NAME) && \
	rm -rf $(NAME) && \
	printf " %b | " $(TICK) && \
	printf "Removed %b%b%b executable\n" $(RED) $(NAME) $(BLANK) \
	|| (printf " %b | " $(CROSS) && \
	printf "No %b%b%b executable\n" $(RED) $(NAME) $(BLANK))

re: fclean all
