/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipex.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: bsomers <bsomers@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/04 10:09:19 by bsomers       #+#    #+#                 */
/*   Updated: 2022/02/23 16:49:39 by bsomers       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H

# define PIPEX_H

# include <fcntl.h>
# include <stdlib.h>
# include <unistd.h>
# include <errno.h>
# include "./libft/libft.h"

typedef struct s_pip
{
	char	*path1;
	char	*path2;
	char	**cmd1;
	char	**cmd2;
	int		here_doc;
}				t_pip;

void	split_cmd_args(t_pip *cmd_path, char *argv[], t_pip *doc);
char	*make_and_check_paths(char **paths, char **cmd);
int		find_path(char *envp[]);
void	free_array(char **str);
void	free_struct(t_pip *cmd_path);
void	if_error(char *str);
void	if_error2(char *str);
void	handle_here_doc(char *argv[]);

#endif
