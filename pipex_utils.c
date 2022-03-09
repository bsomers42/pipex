/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipex_utils.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: bsomers <bsomers@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/23 15:17:19 by bsomers       #+#    #+#                 */
/*   Updated: 2022/03/09 11:32:47 by bsomers       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	free_array(char **str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
}

void	free_struct(t_pip *cmd_path)
{
	free(cmd_path->path1);
	free(cmd_path->path2);
	free_array(cmd_path->cmd1);
	free_array(cmd_path->cmd2);
}

void	if_error(void)
{
	ft_putstr_fd("\033[0;\e[1mPipex error: \e[0m ", 2);
	ft_putstr_fd(strerror(errno), 2);
	ft_putstr_fd("\n", 2);
	if (unlink("here.txt") > 0)
		if_error();
	exit(EXIT_FAILURE);
}

void	wait_children(int pid1, int pid2)
{
	int	wexit;

	if (waitpid(pid1, NULL, 0) < 0)
		if_error();
	if (waitpid(pid2, &wexit, 0) < 0)
		if_error();
	if (WIFEXITED(wexit))
		exit(WEXITSTATUS(wexit));
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
		if_error();
	while (cmp != 0)
	{
		str = get_next_line(STDIN_FILENO);
		if (str == NULL)
			if_error();
		cmp = ft_strncmp(str, argv[2], ft_strlen(argv[2]));
		if (cmp == 0)
			break ;
		ft_putstr_fd(str, fd);
		free (str);
	}
	free (str);
}
