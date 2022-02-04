/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipex.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: bsomers <bsomers@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/31 10:30:34 by bsomers       #+#    #+#                 */
/*   Updated: 2022/02/04 11:28:05 by bsomers       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdio.h> //REMOVE THIS ONE
#include <stdlib.h>
#include <unistd.h>
#include "./libft/libft.h"

typedef struct s_info
{
	char	*path1;
	char	*path2;
	char	**cmd1;
	char	**cmd2;
}				t_info; //REMOVE THIS TO HEADER!

void	child_process1(t_info *cmd_path, char *argv[], int *fds)
{
	int	file_i;

	file_i = open(argv[1], O_RDONLY);
	file_i = dup2(file_i, STDIN_FILENO);
	dup2(fds[1], STDOUT_FILENO);
	close (fds[0]);
	close (fds[1]);
	execve(cmd_path->path1, cmd_path->cmd1, NULL);
	close(file_i);
}

void	child_process2(t_info *cmd_path, char *argv[], int *fds)
{
	int	file_o;

	dup2(fds[0], STDIN_FILENO);
	close (fds[0]);
	close (fds[1]);
	file_o = open(argv[4], O_WRONLY);
	file_o = dup2(file_o, STDOUT_FILENO);
	execve(cmd_path->path2, cmd_path->cmd2, NULL);
	close (file_o);
}

int	pipe_and_fork(t_info *cmd_path, char *argv[])
{
	int	fds[2];
	int	pid1;
	int	pid2;

	pipe(fds);
	pid1 = fork();
	if (pid1 < 0)
		return (1);
	if (pid1 == 0)
		child_process1(cmd_path, argv, fds);
	pid2 = fork();
	if (pid2 < 0)
		return (1);
	if (pid2 == 0)
		child_process2(cmd_path, argv, fds);
	close(fds[0]);
	close(fds[1]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
	return (0);
}

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

int	pipex(int argc, char *argv[], char *envp[])
{
	t_info	cmd_path;
	char	*origpath;
	char	**paths;
	int		i;

	i = 0;
	if (argc != 5)
	{
		write(1, "X", 1);
		return (-1);
	}
	i = find_path(envp);
	origpath = ft_strtrim(envp[i], "PATH=");
	paths = ft_split(origpath, ':'); //aantal paden: 8
	free (origpath);
	cmd_path.cmd1 = ft_split(argv[2], ' ');
	cmd_path.cmd2 = ft_split(argv[3], ' ');
	cmd_path.path1 = make_and_check_paths(paths, cmd_path.cmd1);
	cmd_path.path2 = make_and_check_paths(paths, cmd_path.cmd2);
	if (cmd_path.path1 == NULL || cmd_path.path2 == NULL)
		return (0); //put error message here! No correct path given.
	pipe_and_fork(&cmd_path, argv);
	return (0);
}

int	main(int argc, char *argv[], char *envp[])
{
	pipex(argc, argv, envp);
	return (0);
}
