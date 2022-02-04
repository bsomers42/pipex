# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: bsomers <bsomers@student.codam.nl>           +#+                      #
#                                                    +#+                       #
#    Created: 2022/02/04 12:14:27 by bsomers       #+#    #+#                  #
#    Updated: 2022/02/04 12:20:50 by bsomers       ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

NAME = pipex.a

LIBFT = libft

O_FILES = pipex.o \
			pipex_paths.c

SRC = $(O_FILES:.o=.c)

HEADER = pipex.h

CFLAGS = -Wall -Wextra -Werror

OBJ = $(O_FILES)

all: $(NAME)

$(NAME): $(OBJ)
		@make -C $(LIBFT)
		@cp libft/libft.a ./$(NAME)
		ar rcs $@ $^

%.o: %.c $(HEADER)
		$(CC) -c $(CFLAGS) -o $@ $<

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