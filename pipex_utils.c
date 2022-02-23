/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipex_utils.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: bsomers <bsomers@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/23 15:17:19 by bsomers       #+#    #+#                 */
/*   Updated: 2022/02/23 16:46:25 by bsomers       ########   odam.nl         */
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

void	if_error(char *str)
{
	ft_putstr_fd("\033[0;31m\e[1mERROR \U0001F4A5\e[0m ", 2);
	if (str == NULL)
		ft_putstr_fd("Malloc error!\n", 2);
	else if (ft_strncmp(str, "open", 4) == 0)
		ft_putstr_fd("Open error!\n", 2);
	else if (ft_strncmp(str, "gnl", 3) == 0)
		ft_putstr_fd("Error in running get_next_line!\n", 2);
	else if (ft_strncmp(str, "unlink", 6) == 0)
		ft_putstr_fd("Unlink error!\n", 2);
	else if (ft_strncmp(str, "no_path", 7) == 0)
		ft_putstr_fd("No path available\n", 2);
	else if (ft_strncmp(str, "cmd", 3) == 0)
		ft_putstr_fd("Command not found\n", 2);
	exit(1);
}

void	if_error2(char *str)
{
	ft_putstr_fd("\033[0;31m\e[1mERROR \U0001F4A5\e[0m ", 2);
	if (ft_strncmp(str, "dup2", 4) == 0)
		ft_putstr_fd("Dup2 error in child process!\n", 2);
	else if (ft_strncmp(str, "close", 5) == 0)
		ft_putstr_fd("Close error in child process!\n", 2);
	else if (ft_strncmp(str, "execve", 6) == 0)
		ft_putstr_fd("Execve error in child process!\n", 2);
	else if (ft_strncmp(str, "pipe", 4) == 0)
		ft_putstr_fd("Pipe error!\n", 2);
	else if (ft_strncmp(str, "fork", 4) == 0)
		ft_putstr_fd("Fork error!\n", 2);
	else if (ft_strncmp(str, "waitpid", 7) == 0)
		ft_putstr_fd("Waitpid error!\n", 2);
	exit(1);
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
	free (str);
}
