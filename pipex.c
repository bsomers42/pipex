/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipex.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: bsomers <bsomers@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/31 10:30:34 by bsomers       #+#    #+#                 */
/*   Updated: 2022/02/02 18:51:03 by bsomers       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i] != '\0')
		i++;
	return (i);
}

void	*ft_calloc(size_t nmemb, size_t size)
{
	char	*ptr;
	int		i;
	int		all;

	all = nmemb * size;
	ptr = malloc(all);
	i = 0;
	if (ptr == NULL)
		return (NULL);
	while (all > i)
	{
		ptr[i] = 0;
		i++;
	}
	return (ptr);
}

static int	wcount(char const *s, char c)
{
	int	i;
	int	d;
	int	w;

	d = 0;
	w = 0;
	i = ft_strlen(s);
	while (d < i)
	{
		if (s[d] != c && (s[d - 1] == c || (d - 1) == -1))
			w++;
		d++;
	}
	return (w + 1);
}

static char	*wsplit(const char **s, char c)
{
	int		w;
	int		x;
	char	*s1;
	char	*word;

	s1 = (char *)*s;
	w = 0;
	x = 0;
	while (s1[w] != '\0' && s1[w] != c)
		w++;
	word = ft_calloc(w + 1, sizeof(char));
	if (word == NULL)
		return (NULL);
	while (x < w)
	{
		word[x] = *s1;
		s1++;
		x++;
	}
	word[x] = '\0';
	while (*s1 != '\0' && *s1 == c)
		s1++;
	*s = s1;
	return (word);
}

static char	**clearspace(char **array, int x)
{
	while (x > 0)
	{
		x--;
		free(array[x]);
	}
	free(array);
	return (NULL);
}

char	**ft_split(char const *s, char c)
{
	char	**array;
	int		x;
	int		w;

	if (s == NULL)
		return (NULL);
	while (*s == c && *s != '\0')
		s++;
	x = 0;
	w = wcount(s, c);
	array = ft_calloc(w + 1, sizeof(char *));
	if (array == NULL)
		return (NULL);
	while (w > x && *s != '\0')
	{
		array[x] = wsplit(&s, c);
		if (array[x] == NULL)
		{
			clearspace(array, x);
			return (NULL);
		}
		x++;
	}
	array[x] = NULL;
	return (array);
}

char	*ft_strdup(const char *s)
{
	int		len;
	char	*dup;
	int		i;

	len = 0;
	i = 0;
	while (s[len] != '\0')
		len++;
	dup = malloc((sizeof(char)) *(len + 1));
	if (dup == NULL)
		return (NULL);
	while (i < len)
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}


static int	check_set(char c, char const *set)
{
	size_t	i;

	i = 0;
	while (set[i])
	{
		if (set[i] == c)
			return (1);
		i++;
	}
	return (0);
}

static char	*copy_set(char *sdef, char const *s1, int start, int end)
{
	int	i;

	i = 0;
	while (start < end)
	{
		sdef[i] = s1[start];
		i++;
		start++;
	}
	sdef[i] = '\0';
	return (sdef);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*sdef;
	size_t	start;
	size_t	end;

	if (set == NULL || s1 == NULL)
		return ((char *) s1);
	start = 0;
	while (s1[start] && check_set(s1[start], set))
		start++;
	end = ft_strlen(s1);
	while (end > start && check_set(s1[end - 1], set))
		end--;
	if (start >= end)
		return (ft_strdup(""));
	sdef = malloc((sizeof(char)) * (end - start + 1));
	if (sdef == NULL)
		return (NULL);
	copy_set(sdef, s1, start, end);
	return (sdef);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		if (s1[i] == '\0' && s2[i] == '\0')
			return (0);
		if (s1[i] != s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return (0);
}

static char	*joinstr(char *s1, char *s2, char *sdef)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (s1[i] != '\0')
	{
		sdef[i] = s1[i];
		i++;
	}
	while (s2[j] != '\0')
	{
		sdef[i] = s2[j];
		i++;
		j++;
	}
	sdef[i] = '\0';
	return (sdef);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*sdef;
	int		i;
	int		j;

	if (s1 == NULL || s2 == NULL)
		return (0);
	i = ft_strlen(s1);
	j = ft_strlen(s2);
	sdef = malloc((i + j + 1) * (sizeof(unsigned char)));
	if (sdef == NULL)
		return (NULL);
	sdef = joinstr((char *)s1, (char *)s2, sdef);
	return (sdef);
}


int	main(int argc, char *argv[], char *envp[])
{
	int	fds[2];
	int	pid1;
	int	pid2;
	int	file_i;
	int	file_o;
	char **cmd1;
	char **cmd2;
	char *path1;
	char *path2;
	char *origpath;
	char **path;
	int p = 0;
		// envp[6] is PATH;

	if (argc != 5)
	{
		write(1, "X", 1);
		return (-1);
	}
	origpath = ft_strtrim(envp[6], "PATH=");
	path = ft_split(origpath, ':'); //aantal paden: 8
	free (origpath);
	while (p < 8)
	{
		printf("Pathname %d: %s\n", p, path[p]);
		p++;
	}

	cmd1 = ft_split(argv[2], ' ');
	cmd2 = ft_split(argv[3], ' ');
	p = 0;
	while (p < 8)
	{
		origpath = ft_strjoin(path[p], "/");
		origpath = ft_strjoin(origpath, cmd1[0]);
		printf("Origpath %d: %s\n", p, origpath);
		if (access(origpath, F_OK) == 0)
			break;
		free(origpath);
		p++;
	}
	printf("P after break: %d\n", p);
	if (p == 8)
		return (0); //put error message here! No correct path given.
	p = 0;
	path1 = ft_strdup(origpath);
	free (origpath);
	printf("Path1: %s\n", path1);

	while (p < 8)
	{
		origpath = ft_strjoin(path[p], "/");
		origpath = ft_strjoin(origpath, cmd2[0]);
		printf("Origpath %d: %s\n", p, origpath);
		if (access(origpath, F_OK) == 0)
			break;
		free(origpath);
		p++;
	}
	printf("P after break: %d\n", p);
	if (p == 8)
		return (0); //put error message here! No correct path given.

	p = 0;
	path2 = ft_strdup(origpath);
	free (origpath);
	printf("Path2: %s\n", path2);

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
	//	execlp("ls", "ls", file_i, NULL);
		execve(path1, cmd1, NULL);
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
		file_o = open(argv[4], O_WRONLY);
		file_o = dup2(file_o, STDOUT_FILENO);
		// execlp("cat", "cat", "-e", NULL);
		execve(path2, cmd2, NULL);
		close (file_o);
	}
	close(fds[0]);
	close(fds[1]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
	return (0);
}
