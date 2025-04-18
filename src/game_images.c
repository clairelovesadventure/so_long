/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_images.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 22:06:36 by shutan            #+#    #+#             */
/*   Updated: 2025/02/20 22:06:37 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

static int	load_player_textures(t_game *game)
{
	int		i;
	char	filename[32];

	i = 0;
	while (i < 4)
	{
		snprintf(filename, sizeof(filename), "textures/player_%d.png", i);
		game->player_textures[i] = mlx_load_png(filename);
		if (!game->player_textures[i])
		{
			while (--i >= 0)
				mlx_delete_texture(game->player_textures[i]);
			return (0);
		}
		i++;
	}
	return (1);
}

static int	load_collectible_textures(t_game *game)
{
	int		i;
	char	filename[32];

	i = 0;
	while (i < 8)
	{
		snprintf(filename, sizeof(filename), "textures/collectible_%d.png", i);
		game->collectible_textures[i] = mlx_load_png(filename);
		if (!game->collectible_textures[i])
		{
			while (--i >= 0)
				mlx_delete_texture(game->collectible_textures[i]);
			return (0);
		}
		i++;
	}
	return (1);
}

static int	load_basic_textures(t_game *game)
{
	game->wall_texture = mlx_load_png("textures/wall.png");
	game->exit_texture = mlx_load_png("textures/exit.png");
	game->floor_texture = mlx_load_png("textures/floor.png");
	game->enemy_texture = mlx_load_png("textures/enemy.png");
	if (!game->wall_texture || !game->exit_texture
		|| !game->floor_texture || !game->enemy_texture)
	{
		cleanup_textures(game);
		return (0);
	}
	return (1);
}

static int	convert_textures_to_images(t_game *game)
{
	game->wall_img = mlx_texture_to_image(game->mlx, game->wall_texture);
	game->exit_img = mlx_texture_to_image(game->mlx, game->exit_texture);
	game->floor_img = mlx_texture_to_image(game->mlx, game->floor_texture);
	game->enemy_img = mlx_texture_to_image(game->mlx, game->enemy_texture);
	if (!game->wall_img || !game->exit_img
		|| !game->floor_img || !game->enemy_img)
	{
		cleanup_images(game);
		cleanup_textures(game);
		return (0);
	}
	return (1);
}

int	load_images(t_game *game)
{
	if (!load_player_textures(game))
		return (0);
	if (!load_collectible_textures(game))
	{
		cleanup_textures(game);
		return (0);
	}
	if (!load_basic_textures(game))
		return (0);
	return (convert_textures_to_images(game));
}
