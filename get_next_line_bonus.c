/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiyamamo <hiyamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/23 16:37:35 by hiyamamo          #+#    #+#             */
/*   Updated: 2022/04/28 18:36:28 by hiyamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

int	fill_container(char **container, char **buff)
{
	char	*tmp_container;

	if (*container == NULL)
	{
		*container = ft_strdup(*buff);
		if (*container == NULL)
		{
			free(*buff);
			return (0);
		}
	}
	else
	{
		tmp_container = *container;
		*container = ft_strjoin(*container, *buff);
		if (*container == NULL)
		{
			free(tmp_container);
			free(*buff);
			return (0);
		}
		free(tmp_container);
	}
	free(*buff);
	return (1);
}

char	*free_container(char **container)
{
	free(*container);
	*container = NULL;
	return (NULL);
}

int	read_with_container(int fd, char **container)
{
	char	*buff;
	int		bytes_read;

	buff = (char *) malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (buff == NULL)
	{
		if (*container != NULL)
		{
			free_container(container);
			return (0);
		}
	}
	bytes_read = read(fd, buff, BUFFER_SIZE);
	if (bytes_read == -1 || bytes_read == 0)
	{
		if (bytes_read == -1 && *container != NULL)
			free_container(container);
		free(buff);
		return (0);
	}
	buff[bytes_read] = '\0';
	if (fill_container(container, &buff) == 0 || bytes_read < BUFFER_SIZE)
		return (0);
	return (1);
}

char	*update_str_and_container(char *split_ptr, char **container, char *str)
{
	char	*tmp_container;
	char	*tmp_buff;

	tmp_buff = ft_strdup(*container);
	if (tmp_buff == NULL)
		return (free_container(container));
	tmp_buff[(split_ptr - *container) + 1] = '\0';
	str = ft_strdup(tmp_buff);
	free(tmp_buff);
	tmp_container = *container;
	*container = ft_strdup(split_ptr + 1);
	if (*container == NULL)
	{
		free(tmp_container);
		free(str);
		return (NULL);
	}
	if (**container == '\0')
		free_container(container);
	free(tmp_container);
	return (str);
}

char	*get_next_line(int fd)
{
	static char	*container[OPEN_MAX];
	char		*split_ptr;
	char		*str;
	
	if (fd < 0 || fd > OPEN_MAX)
		return (NULL);
	str = NULL;
	while (ft_strchr(container[fd], '\n') == NULL)
		if (read_with_container(fd, &container[fd]) == 0)
			break ;
	split_ptr = ft_strchr(container[fd], '\n');
	if (split_ptr != NULL)
		str = update_str_and_container(split_ptr, &container[fd], str);
	else
	{
		if (container[fd] != NULL)
		{
			str = ft_strdup(container[fd]);
			free(container[fd]);
			container[fd] = NULL;
		}
	}
	return (str);
}
