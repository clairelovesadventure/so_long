#include "so_long.h"
#include <stdlib.h>
#include "libft.h"

void	free_map(t_map *map)
{
	size_t	i;

	if (!map->grid)
		return ;
	i = 0;
	while (i < map->height)
	{
		if (map->grid[i])
			free(map->grid[i]);
		i++;
	}
	free(map->grid);
	map->grid = NULL;
}

void	error_exit(char *message)
{
	ft_printf("Error\n%s\n", message);
	exit(1);
}
