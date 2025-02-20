/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graphics.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 21:32:59 by shutan            #+#    #+#             */
/*   Updated: 2025/02/19 14:48:11 by shutan        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

int	render_static_tile(t_game *game, size_t i, size_t j);
int	render_dynamic_tile(t_game *game, size_t i, size_t j);

static int	init_static_images(t_game *game)
{
	game->floor_img = mlx_texture_to_image(game->mlx, game->floor_texture);
	game->wall_img = mlx_texture_to_image(game->mlx, game->wall_texture);
	game->exit_img = mlx_texture_to_image(game->mlx, game->exit_texture);
	return (game->floor_img && game->wall_img && game->exit_img);
}

static int	init_dynamic_images(t_game *game)
{
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
	return (game->player_img && game->collect_img && game->enemy_img);
}

static int	render_map(t_game *game, int is_static)
{
	size_t	i;
	size_t	j;

	i = -1;
	while (++i < game->map.height)
	{
		j = -1;
		while (++j < game->map.width)
		{
			if (is_static && !render_static_tile(game, i, j))
				return (0);
			if (!is_static && !render_dynamic_tile(game, i, j))
				return (0);
		}
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
		if (!init_static_images(game) || !render_map(game, 1))
			return ;
	}
	if (!init_dynamic_images(game) || !render_map(game, 0))
		return ;
}

void	update_moves_display(t_game *game)
{
	char	moves_str[32];

	if (game->moves_text)
		mlx_delete_image(game->mlx, game->moves_text);
	snprintf(moves_str, sizeof(moves_str), "Moves: %d", game->moves);
	game->moves_text = mlx_put_string(game->mlx, moves_str, 10, 10);
}
