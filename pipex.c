/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipex.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: bsomers <bsomers@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/31 10:30:34 by bsomers       #+#    #+#                 */
/*   Updated: 2022/03/09 11:33:18 by bsomers       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	child_process1(t_pip *cmd_path, char *argv[], int *fds, t_pip *doc)
{
	int	file_i;

	if (doc->here_doc == 0)
		file_i = open("here.txt", O_RDONLY);
	else
		file_i = open(argv[1], O_RDONLY);
	if (file_i < 0)
		if_error();
	file_i = dup2(file_i, STDIN_FILENO);
	dup2(fds[1], STDOUT_FILENO);
	if (fds[1] < 0 || file_i < 0)
		if_error();
	if (close(fds[0]) < 0)
		if_error();
	if (close(fds[1]) < 0)
		if_error();
	if (execve(cmd_path->path1, cmd_path->cmd1, NULL) < 0)
		if_error();
	if (close(file_i) < 0)
		if_error();
}

void	child_process2(t_pip *cmd_path, char *argv[], int *fds, t_pip *doc)
{
	int	file_o;

	if (dup2(fds[0], STDIN_FILENO) < 0)
		if_error();
	if (close (fds[0]) < 0)
		if_error();
	if (close (fds[1]) < 0)
		if_error();
	if (doc->here_doc == 0)
		file_o = open(argv[5], O_CREAT | O_WRONLY | O_TRUNC, 0644);
	else
		file_o = open(argv[4], O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (file_o < 0)
		if_error();
	file_o = dup2(file_o, STDOUT_FILENO);
	if (file_o < 0)
		if_error();
	if (execve(cmd_path->path2, cmd_path->cmd2, NULL) < 0)
		if_error();
	if (close(file_o) < 0)
		if_error();
}

int	pipe_and_fork(t_pip *cmd_path, t_pip *doc, char *argv[])
{
	int	fds[2];
	int	pid1;
	int	pid2;

	if (pipe(fds) < 0)
		if_error();
	pid1 = fork();
	if (pid1 == 0)
		child_process1(cmd_path, argv, fds, doc);
	pid2 = fork();
	if (pid2 < 0 || pid1 < 0)
		if_error();
	if (pid2 == 0)
		child_process2(cmd_path, argv, fds, doc);
	if (close(fds[0]) < 0)
		if_error();
	if (close(fds[1]) < 0)
		if_error();
	wait_children(pid1, pid2);
	return (0);
}

void	pipex(char *argv[], char *envp[], t_pip *doc)
{
	t_pip	cmd_path;

	paths_and_cmds(envp, doc, &cmd_path, argv);
	pipe_and_fork(&cmd_path, doc, argv);
	free_struct(&cmd_path);
	if (doc->here_doc == 0)
	{
		if (unlink("here.txt") > 0)
			if_error();
	}
}

int	main(int argc, char *argv[], char *envp[])
{
	t_pip	doc;

	doc.here_doc = 1;
	if (argc < 5 || argc > 6)
	{
		ft_putstr_fd("Invalid number of arguments!\n", 2);
		return (1);
	}
	if (ft_strncmp(argv[1], "here_doc", 8) == 0)
	{
		if (argc != 6)
		{
			ft_putstr_fd("Invalid number of arguments!\n", 2);
			return (1);
		}
		doc.here_doc = 0;
		handle_here_doc(argv);
	}
	if (doc.here_doc == 1 && argc != 5)
	{
		ft_putstr_fd("Invalid number of arguments!\n", 2);
		return (1);
	}
	pipex(argv, envp, &doc);
	return (0);
}
