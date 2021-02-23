/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jimkwon <jimkwon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/21 14:51:40 by jimkwon           #+#    #+#             */
/*   Updated: 2021/02/23 11:35:59 by jimkwon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub_3d.h"

size_t				ft_strlen(const char *str)
{
	size_t			i;

	i = 0;
	while (*str != '\0')
	{
		i++;
		str++;
	}
	return (i);
}

char				*is_in_newline(char *str)
{
	char			*p;

	p = str;
	while (*p != '\0')
	{
		if (*p == '\n')
			return (p);
		p++;
	}
	return (NULL);
}

char				*ft_strjoin(char *s1, char *s2)
{
	size_t			len;
	char			*str;
	size_t			i;

	if (!s1 || !s2)
		return (0);
	len = ft_strlen(s1) + ft_strlen(s2);
	if (len == 0)
		return (ft_strdup(""));
	else if (*s1 == '\0')
		return (ft_strdup(s2));
	else if (*s2 == '\0')
		return (ft_strdup(s1));
	else
	{
		if (!(str = (char *)malloc(sizeof(char) * (len + 1))))
			return (NULL);
	}
	i = ft_strlcpy(str, s1, ft_strlen(s1) + 1);
	ft_strlcpy(str + i, s2, len + 1);
	return (str);
}

char				*ft_substr(char *s, unsigned int start, size_t len)
{
	unsigned int	s_len;
	unsigned int	i;
	char			*str;

	if (!s)
		return (0);
	s_len = (int)ft_strlen(s);
	if (start > s_len)
		return (ft_strdup(""));
	if (!(str = (char *)malloc(sizeof(char) * (len + 1))))
		return (NULL);
	i = 0;
	while (i != start)
	{
		s++;
		i++;
	}
	i = 0;
	while (i < len && *s != '\0')
	{
		str[i++] = *s;
		s++;
	}
	str[i] = '\0';
	return (str);
}
