/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jimkwon <jimkwon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/21 14:44:59 by jimkwon           #+#    #+#             */
/*   Updated: 2021/02/23 11:36:05 by jimkwon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub_3d.h"

size_t				ft_strlcpy(char *dest, const char *src, size_t size)
{
	size_t			i;
	size_t			len;

	if (!src)
		return (0);
	i = 0;
	len = ft_strlen(src);
	if ((int)size <= 0)
		return (len);
	while (i < size - 1 && *src != '\0')
	{
		*dest = *(char *)src;
		dest++;
		src++;
		i++;
	}
	*dest = '\0';
	return (len);
}

char				*ft_strdup(char *s)
{
	size_t			len;
	char			*str;

	len = ft_strlen(s);
	if (!(str = (char *)malloc(sizeof(char) * (len + 1))))
		return (NULL);
	ft_strlcpy(str, s, len + 1);
	return (str);
}

int					return_status(char **line, char **prev, char *index)
{
	char			*temp;

	if (index)
	{
		*line = ft_substr(*prev, 0, index - *prev);
		temp = ft_substr(*prev, (index + 1) - *prev,
		ft_strlen(*prev) - ft_strlen(*line));
		free(*prev);
		*prev = ft_strdup(temp);
		free(temp);
		return (1);
	}
	else
	{
		*line = ft_strdup(*prev);
		free(*prev);
		*prev = NULL;
	}
	return (0);
}

int					get_next_line(int fd, char **line)
{
	char			*buf;
	static	char	*prev[OPEN_MAX];
	char			*temp;
	char			*index;
	ssize_t			status;

	if (fd < 0 || BUFFER_SIZE <= 0 || fd > OPEN_MAX ||
	!(buf = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1))) || !line)
		return (-1);
	if (!prev[fd])
		prev[fd] = ft_strdup("");
	while (!(index = is_in_newline(prev[fd]))
	&& (status = read(fd, buf, BUFFER_SIZE)) > 0)
	{
		buf[status] = '\0';
		temp = ft_strjoin(prev[fd], buf);
		free(prev[fd]);
		prev[fd] = ft_strdup(temp);
		free(temp);
	}
	free(buf);
	if (status < 0)
		return (-1);
	return (return_status(line, &prev[fd], index));
}
