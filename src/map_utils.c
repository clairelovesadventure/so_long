/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shuxintan <shuxintan@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 21:33:08 by shutan            #+#    #+#             */
/*   Updated: 2025/02/20 18:33:51 by shuxintan        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

int	check_filename(char *filename)
{
	size_t	len;

	len = ft_strlen(filename);
	return (len > 4 && !ft_strncmp(filename + len - 4, ".ber", 4));
}

static char	*create_temp(char *line, char c, int len)
{
	char	*temp;

	temp = malloc(len + 2);
	if (!temp)
		return (NULL);
	if (line)
	{
		ft_strlcpy(temp, line, len + 1);
		free(line);
	}
	temp[len] = c;
	temp[len + 1] = '\0';
	return (temp);
}

char	*read_line(int fd)
{
	char	buffer[2];
	char	*line;
	int		ret;
	int		len;

	line = NULL;
	len = 0;
	ret = read(fd, buffer, 1);
	while (ret > 0)
	{
		buffer[ret] = '\0';
		if (buffer[0] == '\n')
			break ;
		line = create_temp(line, buffer[0], len);
		if (!line)
			return (NULL);
		len++;
		ret = read(fd, buffer, 1);
	}
	return (line);
}

void	init_map(t_map *map)
{
	map->grid = NULL;
	map->width = 0;
	map->height = 0;
	map->collectibles = 0;
	map->exit = 0;
	map->player = 0;
}

int	load_map_data(t_game *game, int fd)
{
	char	*line;
	size_t	i;

	game->map.grid = malloc(sizeof(char *) * game->map.height);
	if (!game->map.grid)
		return (0);
	i = 0;
	line = read_line(fd);
	while (line && i < game->map.height)
	{
		game->map.grid[i] = line;
		i++;
		line = read_line(fd);
	}
	return (1);
}
