/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipex.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: bsomers <bsomers@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/04 10:09:19 by bsomers       #+#    #+#                 */
/*   Updated: 2022/03/09 11:32:24 by bsomers       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H

# define PIPEX_H

# include <fcntl.h>
# include <stdlib.h>
# include <unistd.h>
# include <errno.h>
# include <string.h>
# include <limits.h>

typedef struct s_pip
{
	char	*path1;
	char	*path2;
	char	**cmd1;
	char	**cmd2;
	int		here_doc;
}				t_pip;

typedef struct s_gnl
{
	int		ret;
	int		n;
	char	*nextline;
	int		a;
	int		flag;
}		t_gnl;

void	split_cmd_args(t_pip *cmd_path, char *argv[], t_pip *doc);
void	paths_and_cmds(char *envp[], t_pip *doc, t_pip *cmd_path, char *argv[]);
char	*make_check_paths(char **paths, char **cmd, t_pip *doc, char *argv[]);
int		find_path(char *envp[]);
void	free_array(char **str);
void	free_struct(t_pip *cmd_path);
void	if_error(void);
void	wait_children(int pid1, int pid2);
int		command_not_found_error(char *cmd, t_pip *doc, char *argv[]);
void	handle_here_doc(char *argv[]);
void	ft_putstr_fd(char *s, int fd);
char	**ft_split(char const *s, char c);
char	*ft_strdup(const char *s);
char	*ft_strjoin(char const *s1, char const *s2);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_substr_gnl(char const *s, unsigned int st, size_t len, t_gnl *stat);
char	*joinstr_gnl(char *s1, char c, char *sdef);
char	*get_next_line(int fd);
char	*ft_strtrim(char const *s1, char const *set);
char	*ft_strchr(const char *s, int c);
size_t	ft_strlen(const char *s);
void	*ft_calloc(size_t nmemb, size_t size);
void	*ft_memmove(void *dst, const void *src, size_t len);

#endif
