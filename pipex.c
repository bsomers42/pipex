/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipex.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: bsomers <bsomers@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/28 15:31:04 by bsomers       #+#    #+#                 */
/*   Updated: 2022/01/28 20:46:14 by bsomers       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    int fds[2];
	int stat;
    int	pid1;
	int pid2;
    
    pipe(fds);
	if (fds < 0)
		return (1);
    pid1 = fork();
	if (pid1 < 0)
		return (1);
    if (pid1 == 0) //child process 1, handles first cmd
    {
		dup2(fds[1], STDOUT_FILENO);
		close (fds[0]);
		close (fds[1]);
		execlp("ping", "ping", "-c", "5", "google.com", NULL);
       	// fds[0] = open(argv[1], O_RDONLY);
		// dup2(fds[0], STDIN_FILENO);
		// close(fds[0]);
        // close(fds[1]);
        // if ((execvp(argv[0], &argv[1])) < 0)
        //     exit(0);
    }

	pid2 = fork();
	if (pid2 < 0)
		return(1);
	if (pid2 == 0) //child process 2, handles 2nd cmd
	{
		dup2(fds[0], STDIN_FILENO);
		close (fds[0]);
		close (fds[1]);
		execlp("grep", "grep", "rtt", NULL);
	}

	close(fds[0]);
	close(fds[1]);

	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
	// close(fds[0]);

	// fds[1] = open(argv[3], O_WRONLY);
	// dup2(fds[1], STDOUT_FILENO);
	// close(fds[1]);

	// printf("Argc: %d", argc);

	// wp = waitpid(cp, &stat, 0);
	// return wp == cp && WIFEXITED(stat) ? WEXITSTATUS(stat) : -1;

}

//Example to run: input.txt "grep -e 'c'" "wc -l" output.txt
