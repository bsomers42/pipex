/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipex.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: bsomers <bsomers@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/31 10:30:34 by bsomers       #+#    #+#                 */
/*   Updated: 2022/02/23 16:46:47 by bsomers       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include <stdio.h> //remove!!!!

void	child_process1(t_pip *cmd_path, char *argv[], int *fds, t_pip *doc)
{
	int	file_i;

	if (doc->here_doc == 0)
		file_i = open("here.txt", O_RDONLY);
	else
		file_i = open(argv[1], O_RDONLY);
	if (file_i < 0)
		if_error("open");
	file_i = dup2(file_i, STDIN_FILENO);
	dup2(fds[1], STDOUT_FILENO);
	if (fds[1] < 0 || file_i < 0)
		if_error2("dup2");
	if (close (fds[0]) < 0)
		if_error2("close");
	if (close (fds[1]) < 0)
		if_error2("close");
	if (execve(cmd_path->path1, cmd_path->cmd1, NULL) < 0)
		if_error2("execve");
	if (close(file_i) < 0)
		if_error2("close");
}

void	child_process2(t_pip *cmd_path, char *argv[], int *fds, t_pip *doc)
{
	int	file_o;

	if (dup2(fds[0], STDIN_FILENO) < 0)
		if_error2("dup2");
	if (close (fds[0]) < 0)
		if_error2("close");
	if (close (fds[1]) < 0)
		if_error2("close");
	if (doc->here_doc == 0)
		file_o = open(argv[5], O_CREAT | O_WRONLY | O_TRUNC, 0707);
	else
		file_o = open(argv[4], O_CREAT | O_WRONLY | O_TRUNC, 0707);
	if (file_o < 0)
		if_error("open");
	file_o = dup2(file_o, STDOUT_FILENO);
	if (file_o < 0)
		if_error2("dup2");
	if (execve(cmd_path->path2, cmd_path->cmd2, NULL) < 0)
		if_error2("execve");
	if (close(file_o) < 0)
		if_error2("close");
}

int	pipe_and_fork(t_pip *cmd_path, t_pip *doc, char *argv[])
{
	int	fds[2];
	int	pid1;
	int	pid2;

	if (pipe(fds) < 0)
		if_error2("pipe");
	pid1 = fork();
	if (pid1 == 0)
		child_process1(cmd_path, argv, fds, doc);
	pid2 = fork();
	if (pid2 < 0 || pid1 < 0)
		if_error2("fork");
	if (pid2 == 0)
		child_process2(cmd_path, argv, fds, doc);
	if (close(fds[0]) < 0)
		if_error2("close");
	if (close(fds[1]) < 0)
		if_error2("close");
	if (waitpid(pid1, NULL, 0) < 0)
		if_error2("waitpid");
	if (waitpid(pid2, NULL, 0) < 0)
		if_error2("waitpid");
	return (0);
}

void	pipex(char *argv[], char *envp[], t_pip *doc)
{
	t_pip	cmd_path;
	char	*origpath;
	char	**paths;
	int		i;

	i = find_path(envp);
	origpath = ft_strtrim(envp[i], "PATH=");
	if (origpath == NULL)
		if_error(NULL);
	paths = ft_split(origpath, ':');
	free (origpath);
	if (paths == NULL)
		if_error(NULL);
	split_cmd_args(&cmd_path, argv, doc);
	cmd_path.path1 = make_and_check_paths(paths, cmd_path.cmd1);
	cmd_path.path2 = make_and_check_paths(paths, cmd_path.cmd2);
	free_array(paths);
	pipe_and_fork(&cmd_path, doc, argv);
	free_struct(&cmd_path);
	if (doc->here_doc == 0)
	{
		if (unlink("here.txt") > 0)
			if_error("unlink");
	}
}

int	main(int argc, char *argv[], char *envp[])
{
	t_pip	doc;

	doc.here_doc = 1;
	if (argc < 5)
	{
		ft_putstr_fd("\033[0;33m\e[1mInvalid number of arguments!\e[0m\n", 2);
		return (1);
	}
	if (ft_strncmp(argv[1], "here_doc", 8) == 0)
	{
		doc.here_doc = 0;
		handle_here_doc(argv);
	}
	pipex(argv, envp, &doc);
	return (0);
}
