/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipex_heredoc.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: bsomers <bsomers@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/31 10:30:34 by bsomers       #+#    #+#                 */
/*   Updated: 2022/02/09 17:04:34 by bsomers       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include <stdio.h> //remove!!!!

// void if_error()
// {
// 	char *warning;
// 	if (errno != 0)
// 	{
// 		warning = strerror(errno);
// 		ft_putstr_fd(warning, 1);
// 		exit(errno);
// 	}
// }

void	child_process1(t_info *cmd_path, char *argv[], int *fds, t_info *doc)
{
	int	file_i;

	if (doc->here_doc == 0)
		file_i = open("here.txt", O_RDONLY);
	else
		file_i = open(argv[1], O_RDONLY);
	file_i = dup2(file_i, STDIN_FILENO);
	dup2(fds[1], STDOUT_FILENO);
	close (fds[0]);
	close (fds[1]);
	execve(cmd_path->path1, cmd_path->cmd1, NULL);
	close(file_i);
}

void	child_process2(t_info *cmd_path, char *argv[], int *fds, t_info *doc)
{
	int	file_o;

	dup2(fds[0], STDIN_FILENO);
	close (fds[0]);
	close (fds[1]);
	if (doc->here_doc == 0)
		file_o = open(argv[5], O_WRONLY);
	else
		file_o = open(argv[4], O_WRONLY);
	file_o = dup2(file_o, STDOUT_FILENO);
	execve(cmd_path->path2, cmd_path->cmd2, NULL);
	close (file_o);
}

int	pipe_and_fork(t_info *cmd_path, t_info *doc, char *argv[])
{
	int	fds[2];
	int	pid1;
	int	pid2;

	pipe(fds);
	pid1 = fork();
	if (pid1 < 0)
		return (1);
	if (pid1 == 0)
		child_process1(cmd_path, argv, fds, doc);
	pid2 = fork();
	if (pid2 < 0)
		return (1);
	if (pid2 == 0)
		child_process2(cmd_path, argv, fds, doc);
	close(fds[0]);
	close(fds[1]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
	return (0);
}

void	handle_here_doc(char *argv[])
{
	char buffer[50];
	int i = 0;
	int fd = open("here.txt", O_CREAT | O_RDWR | O_APPEND, 0707);
	while (i < 5)
	{
		read(STDIN_FILENO, buffer, 100);
		if (ft_strncmp(buffer, argv[2], ft_strlen(argv[2])) == 0)
			break ;
		write(fd, buffer, ft_strlen(buffer));
		i++;
	}
}

int	pipex(int argc, char *argv[], char *envp[])
{
	t_info	cmd_path;
	t_info	doc;
	char	*origpath;
	char	**paths;
	int		i;

	i = 0;
	doc.here_doc = 1;
	if (argc < 5)
	{
		ft_putstr_fd("Invalid number of arguments!\n", 1);
		exit(1);
	}
	if (ft_strncmp(argv[1], "here_doc", 8) == 0)
	{
		doc.here_doc = 0;
		handle_here_doc(argv);
	//	return(0);
	}
	i = find_path(envp);
	origpath = ft_strtrim(envp[i], "PATH=");
		//malloc protecten
	paths = ft_split(origpath, ':'); //aantal paden: 8
		//malloc protecten
	free (origpath);
	if (doc.here_doc == 0)
		cmd_path.cmd1 = ft_split(argv[3], ' ');
	else
		cmd_path.cmd1 = ft_split(argv[2], ' ');
	//malloc protecten!
	if (doc.here_doc == 0)
		cmd_path.cmd2 = ft_split(argv[4], ' ');
	else
		cmd_path.cmd2 = ft_split(argv[3], ' ');
		//malloc protecten!
	cmd_path.path1 = make_and_check_paths(paths, cmd_path.cmd1);
	cmd_path.path2 = make_and_check_paths(paths, cmd_path.cmd2); //<<-------
	if (cmd_path.path1 == NULL || cmd_path.path2 == NULL)
		return (0); //put error message here! No correct path given.
	pipe_and_fork(&cmd_path, &doc, argv);
	unlink("here.txt");
		//error protect!
	return (0);
}

int	main(int argc, char *argv[], char *envp[])
{
	pipex(argc, argv, envp);
	return (0);
}


//    gcc -Wall -Wextra -Werror pipex_heredoc.c pipex_paths.c pipex.h libft/ft_putstr_fd.c libft/ft_split.c libft/ft_strjoin.c libft/ft_strncmp.c libft/ft_strtrim.c libft/ft_calloc.c libft/ft_strdup.c libft/ft_strlen.c
