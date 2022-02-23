/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipex_paths.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: bsomers <bsomers@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/04 12:19:58 by bsomers       #+#    #+#                 */
/*   Updated: 2022/02/23 16:50:19 by bsomers       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*make_and_check_paths(char **paths, char **cmd)
{
	int		p;
	char	*path[2];

	p = 0;
	path[0] = NULL;
	path[1] = NULL;
	while (p < 8)
	{
		path[0] = ft_strjoin(paths[p], "/");
		if (path[0] == NULL)
			if_error(NULL);
		path[1] = ft_strjoin(path[0], cmd[0]);
		free(path[0]);
		if (path[1] == NULL)
			if_error(NULL);
		if (access(path[1], F_OK) == 0)
			break ;
		free(path[1]);
		p++;
	}
	if (p == 8)
		if_error("cmd");
	return (path[1]);
}

int	find_path(char *envp[])
{
	int	p;
	int	i;

	p = 0;
	i = 0;
	while (envp)
	{
		p = ft_strncmp(envp[i], "PATH=", 5);
		if (p == 0)
			return (i);
		i++;
	}
	if_error("no_path");
	return (1);
}
