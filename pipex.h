/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipex.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: bsomers <bsomers@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/04 10:09:19 by bsomers       #+#    #+#                 */
/*   Updated: 2022/02/04 12:20:38 by bsomers       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H

# define PIPEX_H

# include <fcntl.h>
# include <stdlib.h>
# include <unistd.h>
# include "./libft/libft.h"

typedef struct s_info
{
	char *path1;
    char *path2;
    char **cmd1;
    char **cmd2;
}				t_info;

char	*make_and_check_paths(char **paths, char **cmd);
int	find_path(char *envp[]);

#endif