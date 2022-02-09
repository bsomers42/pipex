/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipex_multiple.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: bsomers <bsomers@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/31 10:30:34 by bsomers       #+#    #+#                 */
/*   Updated: 2022/02/07 19:18:39 by bsomers       ########   odam.nl         */
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

void	child_process1(t_info *cmd_path, char *argv[], int fds[3][2])
{
	int	file_i;

	close(fds[0][1]);
	close(fds[1][0]);
	close(fds[2][0]);
	close(fds[2][1]);

	file_i = open(argv[1], O_RDONLY);
	file_i = dup2(file_i, STDIN_FILENO);
	dup2(fds[1][1], STDOUT_FILENO);
	close(fds[0][0]);
	close(fds[1][1]);
	execve(cmd_path->path1, cmd_path->cmd1, NULL);
	close(file_i);
}

void	child_processmid(t_info *cmd_path, /*char *argv[], */int fds[3][2])
{
	int	file_o;

	close(fds[0][0]);
	close(fds[0][1]);
	close(fds[1][1]);
	close(fds[2][0]);

	fds[1][0] = dup2(fds[1][0], STDIN_FILENO);
	// close (fds[0]);
	// close (fds[1]);
	file_o =  dup2(fds[2][1], STDOUT_FILENO);
	close(fds[2][1]);
	close(fds[1][0]);
	execve(cmd_path->path2, cmd_path->cmd2, NULL);
	close(file_o); //PUINZOOOO00Oi

}

void	child_process2(t_info *cmd_path, char *argv[], int fds[3][2])
{
	int	file_o;

	dup2(fds[2][0], STDIN_FILENO);
	// close (fds[0]);
	// close (fds[1]);
	file_o = open(argv[6], O_WRONLY);
	file_o = dup2(file_o, STDOUT_FILENO);
	execve(cmd_path->path3, cmd_path->cmd3, NULL);
	close (file_o);
}

int	pipe_and_fork(t_info *cmd_path, char *argv[])
{
	int	fds[3][2]; //want drie pipes (en per pipe 2 fd's)
	int	pid1;
	int	pid2;
	int pid4;
	int	i = 0;

	while (i < 3 /*= number of pipes*/)
	{
		pipe(fds[i]);
		//error protecten! Want als laatste pipe failt, moet je de vorige closen
		i++;
	}
	pid1 = fork();
	if (pid1 < 0)
		return (1);
	if (pid1 == 0)
		child_process1(cmd_path, argv, fds);
	pid2 = fork();
	if (pid2 < 0)
		return (1);
	if (pid2 == 0)
		child_processmid(cmd_path, /*argv,*/ fds);
	pid4 = fork();
	if (pid4 < 0)
		return (1);
	if (pid4 == 0)
		child_process2(cmd_path, argv, fds);

	close(fds[0][0]);
	close(fds[0][1]);
	close(fds[1][0]);
	close(fds[1][1]);
	close(fds[2][0]);
	close(fds[2][1]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
	waitpid(pid4, NULL, 0);

	return (0);
}

int	pipex(int argc, char *argv[], char *envp[])
{
	t_info	cmd_path;
	char	*origpath;
	char	**paths;
	int		i;

	i = 0;
	if (argc < 5)
	{
		ft_putstr_fd("Not enough arguments!\n", 1);
		exit(1);
	}
	i = find_path(envp);
	origpath = ft_strtrim(envp[i], "PATH=");
		//malloc protecten
	paths = ft_split(origpath, ':'); //aantal paden: 8
		//malloc protecten
	free (origpath);
	cmd_path.cmd1 = ft_split(argv[2], ' ');
		//malloc protecten!
	cmd_path.cmd2 = ft_split(argv[3], ' ');
		//malloc protecten!
	cmd_path.cmd3 = ft_split(argv[4], ' ');
	cmd_path.cmd4 = ft_split(argv[5], ' ');
	cmd_path.path1 = make_and_check_paths(paths, cmd_path.cmd1);
	cmd_path.path2 = make_and_check_paths(paths, cmd_path.cmd2);
	cmd_path.path3 = make_and_check_paths(paths, cmd_path.cmd3);
	cmd_path.path4 = make_and_check_paths(paths, cmd_path.cmd4);
	if (cmd_path.path1 == NULL || cmd_path.path2 == NULL || cmd_path.path3 == NULL)
		return (0); //put error message here! No correct path given.
	pipe_and_fork(&cmd_path, argv);
	return (0);
}

int	main(int argc, char *argv[], char *envp[])
{
	pipex(argc, argv, envp);
	return (0);
}
 
