/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_validation_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 21:34:20 by shutan            #+#    #+#             */
/*   Updated: 2025/02/17 21:34:21 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

char	**init_visited(t_map *map)
{
	char	**visited;
	size_t	i;

	visited = malloc(sizeof(char *) * map->height);
	if (!visited)
		return (NULL);
	i = 0;
	while (i < map->height)
	{
		visited[i] = malloc(sizeof(char) * map->width);
		if (!visited[i])
		{
			while (i--)
				free(visited[i]);
			free(visited);
			return (NULL);
		}
		ft_memset(visited[i], 0, map->width);
		i++;
	}
	return (visited);
}

void	cleanup_visited(char **visited, size_t height)
{
	size_t	i;

	i = 0;
	while (i < height)
		free(visited[i++]);
	free(visited);
}

int	check_path_validity(t_map *map, char **visited)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (i < map->height)
	{
		j = 0;
		while (j < map->width)
		{
			if ((map->grid[i][j] == 'C' || map->grid[i][j] == 'E')
				&& !visited[i][j])
				return (0);
			j++;
		}
		i++;
	}
	return (1);
}
