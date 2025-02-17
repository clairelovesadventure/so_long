/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_cleanup.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 21:32:08 by shutan            #+#    #+#             */
/*   Updated: 2025/02/17 21:32:09 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

static void	cleanup_images(t_game *game)
{
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

	i = -1;
	while (++i < 4)
		if (game->player_textures[i])
			mlx_delete_texture(game->player_textures[i]);
	i = -1;
	while (++i < 8)
		if (game->collectible_textures[i])
			mlx_delete_texture(game->collectible_textures[i]);
}

static void	cleanup_other_textures(t_game *game)
{
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
	if (game->enemies)
		free(game->enemies);
	if (game->mlx)
	{
		cleanup_images(game);
		cleanup_player_textures(game);
		cleanup_other_textures(game);
		free_map(&game->map);
		mlx_terminate(game->mlx);
	}
}

void	cleanup_hook(void *param)
{
	t_game	*game;

	game = (t_game *)param;
	cleanup(game);
}
