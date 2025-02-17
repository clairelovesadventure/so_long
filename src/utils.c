#include "so_long.h"
#include <stdlib.h>
#include "libft.h"

void free_map(t_map *map)
{
    int i;

    if (map->grid)
    {
        i = 0;
        while (i < map->height)
            free(map->grid[i++]);
        free(map->grid);
        map->grid = NULL;
    }
}

void error_exit(char *message)
{
    ft_putstr_fd("Error\n", 2);
    ft_putstr_fd(message, 2);
    ft_putstr_fd("\n", 2);
    exit(1);
}