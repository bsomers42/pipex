# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: bsomers <bsomers@student.codam.nl>           +#+                      #
#                                                    +#+                       #
#    Created: 2022/02/04 12:14:27 by bsomers       #+#    #+#                  #
#    Updated: 2022/03/09 12:36:17 by bsomers       ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

NAME = pipex

SRC = pipex.c \
	  pipex_paths.c \
	  pipex_cmd.c \
	  pipex_utils.c \
	  srcs/ft_putstr_fd.c \
	  srcs/ft_split.c \
	  srcs/ft_strdup.c \
	  srcs/ft_strjoin.c \
	  srcs/ft_strncmp.c \
	  srcs/ft_substr_gnl.c \
	  srcs/get_next_line.c \
	  srcs/joinstr_gnl.c \
	  srcs/ft_calloc.c \
	  srcs/ft_memmove.c \
	  srcs/ft_strchr.c \
	  srcs/ft_strlen.c \
	  srcs/ft_strtrim.c

HEADER = pipex.h

CFLAGS = -Wall -Wextra -Werror

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
		$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.c $(HEADER)
		$(CC) -c $(CFLAGS) -o $@ $<

bonus: $(NAME)

clean:
		rm -f $(OBJ)

fclean: clean
		rm -f $(NAME)

re:
		$(MAKE) fclean
		$(MAKE) all

.PHONY: all clean fclean re
