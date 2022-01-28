/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipex.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: bsomers <bsomers@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/28 15:31:04 by bsomers       #+#    #+#                 */
/*   Updated: 2022/01/28 16:10:24 by bsomers       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int fds[2];
	int stat;
    pid_t cp;
	pid_t wp;
    
    pipe(fds);
    cp = fork();

    if (cp == 0)
    {
       	fds[0] = open(argv[1], O_RDONLY);
		dup2(fds[0], STDIN_FILENO);
		close(fds[0]);
        close(fds[1]);
        if ((execvp(argv[0], &argv[1])) < 0)
            exit(0);
    }
	close(fds[0]);

	fds[1] = open(argv[3], O_WRONLY);
	dup2(fds[1], STDOUT_FILENO);
	close(fds[1]);

	printf("Argc: %d", argc);

	wp = waitpid(cp, &stat, 0);
	return wp == cp && WIFEXITED(stat) ? WEXITSTATUS(stat) : -1;

}

//Example to run: input.txt "grep -e 'c'" "wc -l" output.txt
