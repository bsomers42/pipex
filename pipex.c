/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipex.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: bsomers <bsomers@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/31 10:30:34 by bsomers       #+#    #+#                 */
/*   Updated: 2022/02/19 19:14:33 by bsomers       ########   odam.nl         */
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
		if_error("dup2");
	if (close (fds[0]) < 0)
		if_error("close");
	if (close (fds[1]) < 0)
		if_error("close");
	if (execve(cmd_path->path1, cmd_path->cmd1, NULL) < 0)
		if_error("execve");
	if (close(file_i) < 0)
		if_error("close");
}

void	child_process2(t_pip *cmd_path, char *argv[], int *fds, t_pip *doc)
{
	int	file_o;

	if (dup2(fds[0], STDIN_FILENO) < 0)
		if_error("dup2");
	if (close (fds[0]) < 0)
		if_error("close");
	if (close (fds[1]) < 0)
		if_error("close");
	if (doc->here_doc == 0)
		file_o = open(argv[5], O_WRONLY);
	else
		file_o = open(argv[4], O_CREAT | O_WRONLY | O_TRUNC, 0707);
	if (file_o < 0)
		if_error("open");
	file_o = dup2(file_o, STDOUT_FILENO);
	if (file_o < 0)
		if_error("dup2");
	if (execve(cmd_path->path2, cmd_path->cmd2, NULL) < 0)
		if_error("execve");
	if (close(file_o) < 0)
		if_error("close");
}

int	pipe_and_fork(t_pip *cmd_path, t_pip *doc, char *argv[])
{
	int	fds[2];
	int	pid1;
	int	pid2;

	if (pipe(fds) < 0)
		if_error("pipe");
	pid1 = fork();
	if (pid1 == 0)
		child_process1(cmd_path, argv, fds, doc);
	pid2 = fork();
	if (pid2 < 0 || pid1 < 0)
		if_error("fork");
	if (pid2 == 0)
		child_process2(cmd_path, argv, fds, doc);
	if (close(fds[0]) < 0)
		if_error("close");
	if (close(fds[1]) < 0)
		if_error("close");
	if (waitpid(pid1, NULL, 0) < 0)
		if_error("waitpid");
	if (waitpid(pid2, NULL, 0) < 0)
		if_error("waitpid");
	return (0);
}

void	handle_here_doc(char *argv[])
{
	int		fd;
	char	*str;
	int		cmp;

	cmp = 1;
	str = NULL;
	fd = open("here.txt", O_CREAT | O_RDWR | O_APPEND, 0707);
	if (fd < 0)
		if_error("open");
	while (cmp != 0)
	{
		str = get_next_line(STDIN_FILENO);
		if (str == NULL)
			if_error("gnl");
		cmp = ft_strncmp(str, argv[2], ft_strlen(argv[2]));
		if (cmp == 0)
			break ;
		ft_putstr_fd(str, fd);
		free (str);
	}
}

void	pipex(char *argv[], char *envp[], t_pip *doc)
{
	t_pip	cmd_path;
	char	*origpath;
	char	**paths;
	int		i;

	i = 0;
	if (doc->here_doc == 0)
		handle_here_doc(argv);
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
	//free paths!
	pipe_and_fork(&cmd_path, doc, argv);
	//free doc & cmd_path structs? (alles waarvoor gemallocd is)
	if (doc->here_doc == 0)
	{
		if (unlink("here.txt") > 0)
			if_error("unlink");
	}
}

void	atexit_func(void)
{
	system("leaks pipex");
}

int	main(int argc, char *argv[], char *envp[])
{
	t_pip	doc;

	atexit(atexit_func);
	doc.here_doc = 1;
	if (argc < 5)
	{
		ft_putstr_fd("\033[0;33m\e[1mInvalid number of arguments!\e[0m\n", 2);
		return (1);
	}
	if (ft_strncmp(argv[1], "here_doc", 8) == 0)
		doc.here_doc = 0;
	pipex(argv, envp, &doc);
	return (0);
}

//Bold: \e[1m
//End bold: \e[0m
//Color: \033[0;33m
//Emoji: \U0001F4A5 
//Emoji code: https://emojipedia.org/
//gcc -Wall -Wextra -Werror pipex.c pipex_paths.c pipex.h libft/ft_putstr_fd.c libft/ft_split.c libft/ft_strjoin.c libft/ft_strncmp.c libft/ft_strtrim.c libft/ft_calloc.c libft/ft_strdup.c libft/ft_strlen.c libft/ft_memmove.c libft/get_next_line.c libft/ft_substr_gnl.c libft/joinstr_gnl.c 
