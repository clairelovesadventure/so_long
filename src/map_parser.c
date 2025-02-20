/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_parser.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shuxintan <shuxintan@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 21:33:18 by shutan            #+#    #+#             */
/*   Updated: 2025/02/20 18:16:37 by shuxintan        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

static int	read_map_content(t_game *game, int fd)
{
	char	*line;
	int		count;

	count = 0;
	line = read_line(fd);
	while (line)
	{
		if (count == 0)
			game->map.width = ft_strlen(line);
		count++;
		free(line);
		line = read_line(fd);
	}
	game->map.height = count;
	return (1);
}

static int	load_map_data(t_game *game, int fd)
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

static int	find_player_pos(t_game *game)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (i < game->map.height)
	{
		j = 0;
		while (j < game->map.width)
		{
			if (game->map.grid[i][j] == 'P')
			{
				game->player_x = j;
				game->player_y = i;
				return (1);
			}
			j++;
		}
		i++;
	}
	return (0);
}

static int	validate_and_load(t_game *game, char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (0);
	if (!read_map_content(game, fd))
	{
		close(fd);
		return (0);
	}
	close(fd);
	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (0);
	if (!load_map_data(game, fd))
	{
		close(fd);
		return (0);
	}
	close(fd);
	return (1);
}

int	parse_map(t_game *game, char *filename)
{
	if (!check_filename(filename))
		return (0);
	init_map(&game->map);
	if (!validate_and_load(game, filename))
		return (0);
	if (!validate_map_chars(&game->map)
		|| !validate_map_shape(&game->map)
		|| !validate_map_walls(&game->map)
		|| !validate_map_path(&game->map))
	{
		free_map(&game->map);
		return (0);
	}
	if (!find_player_pos(game))
	{
		free_map(&game->map);
		return (0);
	}
	return (1);
}
