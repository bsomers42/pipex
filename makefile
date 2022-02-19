# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: bsomers <bsomers@student.codam.nl>           +#+                      #
#                                                    +#+                       #
#    Created: 2022/02/04 12:14:27 by bsomers       #+#    #+#                  #
#    Updated: 2022/02/19 18:02:53 by bsomers       ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

NAME = pipex

LIBFT = libft

SRC = pipex.c \
	  pipex_paths.c

HEADER = pipex.h

CFLAGS = -Wall -Wextra -Werror

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ) $(LIBFT).a
		$(CC) $(CFLAGS) -L. -lft $(OBJ) -o $(NAME)

%.o: %.c $(HEADER)
		$(CC) -c $(CFLAGS) -o $@ $<

$(LIBFT).a:
	@make -C $(LIBFT)
	@cp $(LIBFT)/$(LIBFT).a .

bonus: $(NAME)

clean:
		rm -f $(O_FILES)
		@make clean -C $(LIBFT)

fclean: clean
		rm -f $(NAME)
		@make fclean -C $(LIBFT)

re:
		$(MAKE) fclean
		$(MAKE) all

.PHONY: all clean fclean re
