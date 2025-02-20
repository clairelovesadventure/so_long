/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 21:32:59 by shutan            #+#    #+#             */
/*   Updated: 2025/02/19 00:16:46 by shutan        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

int	render_static_tile(t_game *game, size_t i, size_t j)
{
	int	x;
	int	y;

	x = j * TILE_SIZE;
	y = i * TILE_SIZE;
	if (!game->floor_img || !game->wall_img || !game->exit_img)
		return (0);
	if (mlx_image_to_window(game->mlx, game->floor_img, x, y) < 0)
		return (0);
	if (game->map.grid[i][j] == '1' && mlx_image_to_window(game->mlx,
			game->wall_img, x, y) < 0)
		return (0);
	if (game->map.grid[i][j] == 'E' && mlx_image_to_window(game->mlx,
			game->exit_img, x, y) < 0)
		return (0);
	return (1);
}

int	render_dynamic_tile(t_game *game, size_t i, size_t j)
{
	int	x;
	int	y;

	x = j * TILE_SIZE;
	y = i * TILE_SIZE;
	if (!game->player_img || !game->collect_img || !game->enemy_img)
		return (0);
	if (game->map.grid[i][j] == 'P' && mlx_image_to_window(game->mlx,
			game->player_img, x, y) < 0)
		return (0);
	if (game->map.grid[i][j] == 'C' && mlx_image_to_window(game->mlx,
			game->collect_img, x, y) < 0)
		return (0);
	if (game->map.grid[i][j] == 'N' && mlx_image_to_window(game->mlx,
			game->enemy_img, x, y) < 0)
		return (0);
	return (1);
}
