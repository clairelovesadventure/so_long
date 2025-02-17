/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_parser.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 21:33:18 by shutan            #+#    #+#             */
/*   Updated: 2025/02/17 21:33:19 by shutan           ###   ########.fr       */
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
	if (game->map.width > 50 || game->map.height > 50
		|| game->map.width < 3 || game->map.height < 3)
		return (0);
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

static int	validate_and_load(t_game *game, int fd)
{
	if (!read_map_content(game, fd))
		return (0);
	lseek(fd, 0, SEEK_SET);
	if (!load_map_data(game, fd))
		return (0);
	if (!validate_map_shape(&game->map)
		|| !validate_map_walls(&game->map)
		|| !validate_map_chars(&game->map)
		|| !validate_map_path(&game->map)
		|| !find_player_pos(game))
	{
		free_map(&game->map);
		return (0);
	}
	return (1);
}

int	parse_map(t_game *game, char *filename)
{
	int	fd;
	int	result;

	if (!check_filename(filename))
		return (0);
	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (0);
	init_map(&game->map);
	result = validate_and_load(game, fd);
	close(fd);
	return (result);
}
