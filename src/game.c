/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 21:31:56 by shutan            #+#    #+#             */
/*   Updated: 2025/02/20 22:05:58 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

static int	count_enemies(t_game *game)
{
	size_t	i;
	size_t	j;
	int		count;

	count = 0;
	i = 0;
	while (i < game->map.height)
	{
		j = 0;
		while (j < game->map.width)
		{
			if (game->map.grid[i][j] == 'N')
				count++;
			j++;
		}
		i++;
	}
	return (count);
}

static void	set_enemy_position(t_enemy *enemy, size_t x, size_t y)
{
	enemy->x = x;
	enemy->y = y;
	enemy->direction = 0;
}

int	init_enemies(t_game *game)
{
	size_t	i;
	size_t	j;
	int		count;

	count = count_enemies(game);
	game->enemies = malloc(sizeof(t_enemy) * count);
	if (!game->enemies)
		return (0);
	count = 0;
	i = -1;
	while (++i < game->map.height)
	{
		j = -1;
		while (++j < game->map.width)
		{
			if (game->map.grid[i][j] == 'N')
				set_enemy_position(&game->enemies[count++], j, i);
		}
	}
	game->enemy_count = count;
	return (1);
}
