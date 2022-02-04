/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipex_paths.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: bsomers <bsomers@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/04 12:19:58 by bsomers       #+#    #+#                 */
/*   Updated: 2022/02/04 12:22:35 by bsomers       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*make_and_check_paths(char **paths, char **cmd)
{
	int		p;
	char	*path;

	p = 0;
	path = NULL;
	while (p < 8)
	{
		path = ft_strjoin(paths[p], "/");
		path = ft_strjoin(path, cmd[0]);
		if (access(path, F_OK) == 0)
			break ;
		free(path);
		p++;
	}
	// if (p == 8)
	// 	return (NULL); //put error message here! No correct path given.
	// ^^left this out, because if no path is found, path will still be NULL
	return (path);
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
			break ;
		i++;
	}
	return (i);
}
