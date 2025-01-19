/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opoure <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 22:49:57 by opoure            #+#    #+#             */
/*   Updated: 2025/01/16 01:36:02 by opoure           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*fill_line(int fd, char *stock, char *buf)
{
	ssize_t	bread;
	char	*temp;

	bread = 1;
	while (bread > 0)
	{
		bread = read(fd, buf, BUFFER_SIZE);
		if (bread == -1)
		{
			if (stock)
			{
				free(stock);
				stock = 0;
			}
			return (NULL);
		}
		else if (bread == 0)
			break ;
		buf[bread] = '\0';
		if (!stock)
			stock = ft_strdup("");
		temp = stock;
		stock = ft_strjoin(temp, buf);
		free(temp);
		temp = NULL;
		if (ft_strchr(buf, '\n') || !stock)
			break ;
	}
	return (stock);
}

static char	*extract_stock(char *line)
{
	char	*stock;
	size_t	len;

	len = 0;
	if (!line || !(line)[0])
		return (NULL);
	while (line[len] && line[len] != '\n')
		len++;
	if (line[len] == '\n')
		len++;
	if (line[len] == '\0' || line[1] == '\0')
		return (NULL);
	stock = ft_substr(line, len, ft_strlen(line) - len);
	if (!stock)
		return (NULL);
	if (stock[0] == '\0')
	{
		free(stock);
		stock = NULL;
	}
	line[len] = '\0';
	return (stock);
}

char	*get_next_line(int fd)
{
	static char	*stock;
	char		*line;
	char		*buf;

	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, 0, 0) < 0)
	{
		if (stock)
		{
			free(stock);
			stock = NULL;
		}
		return (NULL);
	}
	buf = malloc(sizeof(char) * BUFFER_SIZE + 1);
	if (!buf)
	{
		if (stock)
		{
			free(stock);
			stock = NULL;
		}
		return (NULL);
	}
	line = fill_line(fd, stock, buf);
	free(buf);
	buf = NULL;
	if (!line)
		return (NULL);
	stock = extract_stock(line);
	return (line);
}
/*
#include <stdio.h>
int main(int argc, char *argv[])
{
    int fd;
		char *line;

    if (argc == 2)
    {
        fd = open(argv[1], O_RDONLY);
        if (fd < 0)
        {
            printf("erreur fichier");
            return (1);
        }
        while ((line = get_next_line(fd)) != NULL)
        {
            printf("%s", line);
            free(line);
        }
        close(fd);
    }
    else
    {
        printf("manque le fichier");
    }
    return (0);
}
*/