/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_validation.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 22:07:02 by shutan            #+#    #+#             */
/*   Updated: 2025/02/20 22:07:04 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

// 验证地图字符
int	validate_map_chars(t_map *map)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (i < map->height)
	{
		j = 0;
		while (j < map->width)
		{
			if (map->grid[i][j] != '0' && map->grid[i][j] != '1'
				&& map->grid[i][j] != 'C' && map->grid[i][j] != 'E'
				&& map->grid[i][j] != 'P' && map->grid[i][j] != 'N')
				return (0);
			if (map->grid[i][j] == 'C')
				map->collectibles++;
			if (map->grid[i][j] == 'E')
				map->exit++;
			if (map->grid[i][j] == 'P')
				map->player++;
			j++;
		}
		i++;
	}
	return (map->collectibles > 0 && map->exit == 1 && map->player == 1);
}

// 验证地图形状
int	validate_map_shape(t_map *map)
{
	size_t	i;

	i = 1;
	while (i < map->height)
	{
		if (ft_strlen(map->grid[i]) != map->width)
			return (0);
		i++;
	}
	return (1);
}

// 验证地图墙壁
int	validate_map_walls(t_map *map)
{
	size_t	i;

	i = 0;
	while (i < map->width)
	{
		if (map->grid[0][i] != '1' || map->grid[map->height - 1][i] != '1')
			return (0);
		i++;
	}
	i = 0;
	while (i < map->height)
	{
		if (map->grid[i][0] != '1' || map->grid[i][map->width - 1] != '1')
			return (0);
		i++;
	}
	return (1);
}

static void	flood_fill_from_pos(t_map *map, char **visited, size_t i, size_t j)
{
	static const int	dx[4] = {0, 1, 0, -1};
	static const int	dy[4] = {-1, 0, 1, 0};
	int					k;

	visited[i][j] = 1;
	k = 0;
	while (k < 4)
	{
		if (i + dy[k] < map->height && j + dx[k] < map->width
			&& map->grid[i + dy[k]][j + dx[k]] != '1'
			&& !visited[i + dy[k]][j + dx[k]])
			flood_fill_from_pos(map, visited, i + dy[k], j + dx[k]);
		k++;
	}
}

// 验证地图路径
int	validate_map_path(t_map *map)
{
	char	**visited;
	size_t	i;
	size_t	j;
	int		valid;

	i = 0;
	visited = init_visited(map);
	if (!visited)
		return (0);
	while (i < map->height)
	{
		j = 0;
		while (j < map->width)
		{
			if (map->grid[i][j] == 'P')
				flood_fill_from_pos(map, visited, i, j);
			j++;
		}
		i++;
	}
	valid = check_path_validity(map, visited);
	cleanup_visited(visited, map->height);
	return (valid);
}
