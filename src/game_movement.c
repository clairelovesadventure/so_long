/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_movement.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 22:04:19 by shutan            #+#    #+#             */
/*   Updated: 2025/02/20 22:06:00 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

int	can_move(t_game *game, size_t new_x, size_t new_y)
{
	char	target;

	target = game->map.grid[new_y][new_x];
	if (target == '1')
		return (0);
	if (target == 'E' && game->map.collectibles > 0)
		return (0);
	return (1);
}

void	update_position(t_game *game, size_t new_x, size_t new_y)
{
	char	*target;

	target = &game->map.grid[new_y][new_x];
	if (*target == 'C')
	{
		game->map.collectibles--;
		*target = '0';
	}
	else if (*target == 'E' && game->map.collectibles == 0)
	{
		game->moves++;
		ft_printf("You won in %d moves!\n", game->moves);
		mlx_close_window(game->mlx);
		return ;
	}
	if (new_x != game->player_x || new_y != game->player_y)
	{
		game->map.grid[game->player_y][game->player_x] = '0';
		game->player_x = new_x;
		game->player_y = new_y;
		game->map.grid[new_y][new_x] = 'P';
		game->moves++;
		ft_printf("Moves: %d\n", game->moves);
		update_moves_display(game);
	}
}

static void	handle_movement_key(mlx_key_data_t keydata,
			size_t *new_x, size_t *new_y)
{
	if (keydata.key == MLX_KEY_W || keydata.key == MLX_KEY_UP)
		(*new_y)--;
	else if (keydata.key == MLX_KEY_A || keydata.key == MLX_KEY_LEFT)
		(*new_x)--;
	else if (keydata.key == MLX_KEY_S || keydata.key == MLX_KEY_DOWN)
		(*new_y)++;
	else if (keydata.key == MLX_KEY_D || keydata.key == MLX_KEY_RIGHT)
		(*new_x)++;
}

void	key_hook(mlx_key_data_t keydata, void *param)
{
	t_game	*game;
	size_t	new_x;
	size_t	new_y;

	if (keydata.action != MLX_PRESS)
		return ;
	game = (t_game *)param;
	new_x = game->player_x;
	new_y = game->player_y;
	if (keydata.key == MLX_KEY_ESCAPE)
	{
		mlx_close_window(game->mlx);
		return ;
	}
	handle_movement_key(keydata, &new_x, &new_y);
	if (new_x == game->player_x && new_y == game->player_y)
		return ;
	if (can_move(game, new_x, new_y))
	{
		update_position(game, new_x, new_y);
		render_game(game);
	}
}
