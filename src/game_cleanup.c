/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_cleanup.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 22:06:14 by shutan            #+#    #+#             */
/*   Updated: 2025/02/20 22:06:17 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

void	cleanup_images(t_game *game)
{
	if (!game || !game->mlx)
		return ;
	if (game->moves_text)
		mlx_delete_image(game->mlx, game->moves_text);
	if (game->floor_img)
		mlx_delete_image(game->mlx, game->floor_img);
	if (game->wall_img)
		mlx_delete_image(game->mlx, game->wall_img);
	if (game->exit_img)
		mlx_delete_image(game->mlx, game->exit_img);
	if (game->player_img)
		mlx_delete_image(game->mlx, game->player_img);
	if (game->collect_img)
		mlx_delete_image(game->mlx, game->collect_img);
	if (game->enemy_img)
		mlx_delete_image(game->mlx, game->enemy_img);
}

static void	cleanup_player_textures(t_game *game)
{
	int	i;

	if (!game)
		return ;
	i = 0;
	while (i < 4)
	{
		if (game->player_textures[i])
			mlx_delete_texture(game->player_textures[i]);
		game->player_textures[i] = NULL;
		i++;
	}
	i = 0;
	while (i < 8)
	{
		if (game->collectible_textures[i])
			mlx_delete_texture(game->collectible_textures[i]);
		i++;
	}
}

void	cleanup_textures(t_game *game)
{
	if (!game)
		return ;
	cleanup_player_textures(game);
	if (game->wall_texture)
		mlx_delete_texture(game->wall_texture);
	if (game->exit_texture)
		mlx_delete_texture(game->exit_texture);
	if (game->floor_texture)
		mlx_delete_texture(game->floor_texture);
	if (game->enemy_texture)
		mlx_delete_texture(game->enemy_texture);
}

void	cleanup(t_game *game)
{
	if (!game)
		return ;
	if (game->mlx)
	{
		cleanup_images(game);
		cleanup_textures(game);
		mlx_terminate(game->mlx);
		game->mlx = NULL;
	}
	if (game->enemies)
	{
		free(game->enemies);
		game->enemies = NULL;
	}
	free_map(&game->map);
}

void	cleanup_hook(void *param)
{
	t_game	*game;

	game = (t_game *)param;
	cleanup(game);
}
