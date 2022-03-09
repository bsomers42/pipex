/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipex_paths.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: bsomers <bsomers@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/04 12:19:58 by bsomers       #+#    #+#                 */
/*   Updated: 2022/03/09 11:14:00 by bsomers       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	command_not_found_error(char *cmd, t_pip *doc, char *argv[])
{
	int	file_o;

	ft_putstr_fd("Command not found: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd("\n", 2);
	if (unlink("here.txt") > 0)
		if_error();
	if (doc->here_doc == 0)
		file_o = open(argv[5], O_CREAT | O_WRONLY | O_TRUNC, 0644);
	else
		file_o = open(argv[4], O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (file_o < 0)
		if_error();
	if (close(file_o) < 0)
		if_error();
	return (2);
}

void	paths_and_cmds(char *envp[], t_pip *doc, t_pip *cmd_path, char *argv[])
{
	char	*origpath;
	char	**paths;
	int		i;

	i = find_path(envp);
	origpath = ft_strtrim(envp[i], "PATH=");
	if (origpath == NULL)
		if_error();
	paths = ft_split(origpath, ':');
	free (origpath);
	if (paths == NULL)
		if_error();
	split_cmd_args(cmd_path, argv, doc);
	cmd_path->path1 = make_check_paths(paths, cmd_path->cmd1, doc, argv);
	cmd_path->path2 = make_check_paths(paths, cmd_path->cmd2, doc, argv);
	if (cmd_path->path2 == NULL)
		exit(127);
	free_array(paths);
}

char	*make_check_paths(char **paths, char **cmd, t_pip *doc, char *argv[])
{
	int		p;
	char	*path[2];

	p = 0;
	path[0] = NULL;
	path[1] = NULL;
	while (paths[p])
	{
		path[0] = ft_strjoin(paths[p], "/");
		if (path[0] == NULL)
			if_error();
		path[1] = ft_strjoin(path[0], cmd[0]);
		free(path[0]);
		if (path[1] == NULL)
			if_error();
		if (access(path[1], F_OK) == 0)
			return (path[1]);
		free(path[1]);
		p++;
	}
	command_not_found_error(cmd[0], doc, argv);
	return (NULL);
}

int	find_path(char *envp[])
{
	int	p;
	int	i;

	p = 0;
	i = 0;
	while (envp[i] != NULL)
	{
		p = ft_strncmp(envp[i], "PATH=", 5);
		if (p == 0)
			return (i);
		i++;
	}
	ft_putstr_fd("Path not found\n", 2);
	exit(1);
	return (0);
}
