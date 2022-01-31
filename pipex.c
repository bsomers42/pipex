/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipex.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: bsomers <bsomers@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/31 10:30:34 by bsomers       #+#    #+#                 */
/*   Updated: 2022/01/31 14:17:39 by bsomers       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int	main(int argc, char *argv[])
{
	int	fds[2];
	int	pid1;
	int	pid2;
	int	file_i;
	int	file_o;

	if (argc != 5)
		return (-1);
	pipe(fds);
	if (fds < 0)
		return (1);
	pid1 = fork();
	if (pid1 < 0)
		return (1);
	if (pid1 == 0)
	{
		file_i = open(argv[1], O_RDONLY);
		file_i = dup2(file_i, STDIN_FILENO);
		dup2(fds[1], STDOUT_FILENO);
		close (fds[0]);
		close (fds[1]);
		execlp("grep", "grep", "c", file_i, NULL);
		close(file_i);
	}
	pid2 = fork();
	if (pid2 < 0)
		return (1);
	if (pid2 == 0)
	{
		dup2(fds[0], STDIN_FILENO);
		close (fds[0]);
		close (fds[1]);
		file_o = open(argv[2], O_WRONLY);
		file_o = dup2(file_o, STDOUT_FILENO);
		execlp("wc", "wc", "-l", NULL);
		close (file_o);
	}
	close(fds[0]);
	close(fds[1]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
	return (0);
}
