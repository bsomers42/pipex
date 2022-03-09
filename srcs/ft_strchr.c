/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strchr.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: bsomers <bsomers@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/11/25 14:02:45 by bsomers       #+#    #+#                 */
/*   Updated: 2022/02/28 13:10:31 by bsomers       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

char	*ft_strchr(const char *s, int c)
{
	int	i;

	i = 0;
	while (s[i] != '\0')
	{
		if (s[i] == c)
			return (&((char *)s)[i]);
		i++;
	}
	if (c == '\0')
		return (&((char *)s)[i]);
	return (NULL);
}
