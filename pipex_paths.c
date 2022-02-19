/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipex_paths.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: bsomers <bsomers@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/04 12:19:58 by bsomers       #+#    #+#                 */
/*   Updated: 2022/02/19 18:47:15 by bsomers       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h> //!!!remove

#include "pipex.h"

void	if_error(char *str)
{
	ft_putstr_fd("\033[0;31m\e[1mERROR \U0001F4A5\e[0m ", 2);
	if (str == NULL)
		ft_putstr_fd("Malloc error!\n", 2);
	else if (ft_strncmp(str, "open", 4) == 0)
		ft_putstr_fd("Open error!\n", 2);
	else if (ft_strncmp(str, "dup2", 4) == 0)
		ft_putstr_fd("Dup2 error in child process!\n", 2);
	else if (ft_strncmp(str, "close", 5) == 0)
		ft_putstr_fd("Close error in child process!\n", 2);
	else if (ft_strncmp(str, "execve", 6) == 0)
		ft_putstr_fd("Execve error in child process!\n", 2);
	else if (ft_strncmp(str, "pipe", 4) == 0)
		ft_putstr_fd("Pipe error!\n", 2);
	else if (ft_strncmp(str, "fork", 4) == 0)
		ft_putstr_fd("Fork error!\n", 2);
	else if (ft_strncmp(str, "waitpid", 7) == 0)
		ft_putstr_fd("Waitpid error!\n", 2);
	else if (ft_strncmp(str, "gnl", 3) == 0)
		ft_putstr_fd("Error in running get_next_line!\n", 2);
	else if (ft_strncmp(str, "unlink", 6) == 0)
		ft_putstr_fd("Unlink error!\n", 2);
	else if (ft_strncmp(str, "no_path", 7) == 0)
		ft_putstr_fd("No path available\n", 2);
	else if (ft_strncmp(str, "cmd", 3) == 0)
		ft_putstr_fd("Command not found", 2);
	exit(1);
}

void	split_cmd_args(t_pip *cmd_path, char *argv[], t_pip *doc)
{
	if (doc->here_doc == 0)
		cmd_path->cmd1 = ft_split(argv[3], ' ');
	else
		cmd_path->cmd1 = ft_split(argv[2], ' ');
	if (doc->here_doc == 0)
		cmd_path->cmd2 = ft_split(argv[4], ' ');
	else
		cmd_path->cmd2 = ft_split(argv[3], ' ');
	if (cmd_path->cmd1 == NULL || cmd_path->cmd2 == NULL)
		if_error(NULL);
}

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
