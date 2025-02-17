/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graphics.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 21:32:59 by shutan            #+#    #+#             */
/*   Updated: 2025/02/17 21:32:59 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

static int	render_static_tile(t_game *game, size_t i, size_t j)
{
	int	x;
	int	y;

	x = j * TILE_SIZE;
	y = i * TILE_SIZE;
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

static int	render_dynamic_tile(t_game *game, size_t i, size_t j)
{
	int	x;
	int	y;

	x = j * TILE_SIZE;
	y = i * TILE_SIZE;
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

static int	init_and_render_static(t_game *game)
{
	size_t	i;
	size_t	j;

	game->floor_img = mlx_texture_to_image(game->mlx, game->floor_texture);
	game->wall_img = mlx_texture_to_image(game->mlx, game->wall_texture);
	game->exit_img = mlx_texture_to_image(game->mlx, game->exit_texture);
	if (!game->floor_img || !game->wall_img || !game->exit_img)
		return (0);
	i = -1;
	while (++i < game->map.height)
	{
		j = -1;
		while (++j < game->map.width)
			if (!render_static_tile(game, i, j))
				return (0);
	}
	return (1);
}

static int	update_and_render_dynamic(t_game *game)
{
	size_t	i;
	size_t	j;

	if (game->player_img)
		mlx_delete_image(game->mlx, game->player_img);
	if (game->collect_img)
		mlx_delete_image(game->mlx, game->collect_img);
	if (game->enemy_img)
		mlx_delete_image(game->mlx, game->enemy_img);
	game->player_img = mlx_texture_to_image(game->mlx,
			game->player_textures[game->current_player_frame]);
	game->collect_img = mlx_texture_to_image(game->mlx,
			game->collectible_textures[game->current_collectible_frame]);
	game->enemy_img = mlx_texture_to_image(game->mlx, game->enemy_texture);
	if (!game->player_img || !game->collect_img || !game->enemy_img)
		return (0);
	i = -1;
	while (++i < game->map.height)
	{
		j = -1;
		while (++j < game->map.width)
			if (!render_dynamic_tile(game, i, j))
				return (0);
	}
	return (1);
}

void	render_game(t_game *game)
{
	static int	first_render = 1;

	if (!game || !game->mlx)
		return ;
	if (first_render)
	{
		first_render = 0;
		if (!init_and_render_static(game))
			return ;
	}
	if (!update_and_render_dynamic(game))
		return ;
}
