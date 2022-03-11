/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipex_cmd.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: bsomers <bsomers@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/23 16:48:07 by bsomers       #+#    #+#                 */
/*   Updated: 2022/03/11 10:19:49 by bsomers       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*split_long_cmd(char *str, t_pip *doc)
{
	int		i;
	int		j;
	char	**arr;
	char	*final;

	i = 0;
	j = 0;
	if (doc->here_doc == 0)
		arr = ft_split(str, '/');
	else
		arr = ft_split(str, '/');
	if (arr[i] == NULL)
		if_error();
	while (arr[i])
		i++;
	final = ft_strdup(arr[i - 1]);
	free_array(arr);
	return (final);
}

char	**check_if_cmd_is_path(char **arr, t_pip *doc)
{
	int		i;
	int		x;
	char	*temp;

	i = 0;
	x = ft_strncmp(arr[0], "sed", 3);
	while (arr[i])
	{
		if ((ft_strchr(arr[i], '/') != NULL) && (x != 0))
		{
			temp = ft_strdup(arr[i]);
			free(arr[i]);
			if (temp == NULL)
				if_error();
			arr[i] = split_long_cmd(temp, doc);
			free(temp);
		}
		if (arr[i] == NULL)
			if_error();
		i++;
	}
	return (arr);
}

char	**check_and_trim_apos(char **arr)
{
	int		i;
	char	*temp;

	i = 0;
	temp = NULL;
	while (arr[i])
	{
		if (arr[i][0] == 39)
		{
			temp = ft_strdup(arr[i]);
			free(arr[i]);
			if (temp == NULL)
				if_error();
			arr[i] = ft_strtrim(temp, "'");
			free(temp);
		}
		if (arr[i] == NULL)
			if_error();
		i++;
	}
	return (arr);
}

void	split_cmd_args(t_pip *cmd_path, char *argv[], t_pip *doc)
{
	int		i;
	char	*temp;

	i = 0;
	temp = NULL;
	if (doc->here_doc == 0)
		cmd_path->cmd1 = ft_split(argv[3], ' ');
	else
		cmd_path->cmd1 = ft_split(argv[2], ' ');
	if (doc->here_doc == 0)
		cmd_path->cmd2 = ft_split(argv[4], ' ');
	else
		cmd_path->cmd2 = ft_split(argv[3], ' ');
	if (cmd_path->cmd1 == NULL || cmd_path->cmd2 == NULL)
		if_error();
	cmd_path->cmd1 = check_if_cmd_is_path(cmd_path->cmd1, doc);
	cmd_path->cmd2 = check_if_cmd_is_path(cmd_path->cmd2, doc);
	cmd_path->cmd1 = check_and_trim_apos(cmd_path->cmd1);
	cmd_path->cmd2 = check_and_trim_apos(cmd_path->cmd2);
}
