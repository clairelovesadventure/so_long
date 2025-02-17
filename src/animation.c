/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   animation.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 19:53:32 by shutan           #+#    #+#             */
/*   Updated: 2024/02/17 19:53:32 by shutan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

void	update_animations(t_game *game)
{
	bool	need_render;

	game->frame_counter++;
	need_render = false;
	if (game->frame_counter % (ANIMATION_SPEED * 2) == 0)
	{
		game->current_player_frame = (game->current_player_frame + 1) % 4;
		game->current_collectible_frame
			= (game->current_collectible_frame + 1) % 8;
		need_render = true;
	}
	if (game->frame_counter % ENEMY_MOVE_INTERVAL == 0)
	{
		move_enemies(game);
		need_render = true;
	}
	check_enemy_collision(game);
	if (need_render)
		render_game(game);
}

void	copy_grid_dir(t_game *game, char grid[MAP_HEIGHT][MAP_WIDTH],
			int to_temp)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (i < game->map.height)
	{
		j = 0;
		while (j < game->map.width)
		{
			if (to_temp)
				grid[i][j] = game->map.grid[i][j];
			else
				game->map.grid[i][j] = grid[i][j];
			j++;
		}
		i++;
	}
}

void	update_enemy(t_game *game, char grid[MAP_HEIGHT][MAP_WIDTH], int i)
{
	static const int	dx[] = {0, 1, 0, -1};
	static const int	dy[] = {-1, 0, 1, 0};
	t_enemy				*enemy;
	int					new_x;
	int					new_y;

	enemy = &game->enemies[i];
	new_x = enemy->x + dx[enemy->direction];
	new_y = enemy->y + dy[enemy->direction];
	if (game->map.grid[new_y][new_x] == '1' ||
		game->map.grid[new_y][new_x] == 'C' ||
		game->map.grid[new_y][new_x] == 'E' ||
		game->map.grid[new_y][new_x] == 'N')
	{
		enemy->direction = (enemy->direction + 1) % 4;
		grid[enemy->y][enemy->x] = 'N';
		return ;
	}
	grid[enemy->y][enemy->x] = '0';
	enemy->x = new_x;
	enemy->y = new_y;
	grid[new_y][new_x] = 'N';
}

void	move_enemies(t_game *game)
{
	char	temp_grid[MAP_HEIGHT][MAP_WIDTH];
	int		i;

	copy_grid_dir(game, temp_grid, 1);
	i = 0;
	while (i < game->enemy_count)
	{
		update_enemy(game, temp_grid, i);
		i++;
	}
	copy_grid_dir(game, temp_grid, 0);
}

void	check_enemy_collision(t_game *game)
{
	int	i;

	i = 0;
	while (i < game->enemy_count)
	{
		if (game->player_x == game->enemies[i].x
			&& game->player_y == game->enemies[i].y)
		{
			ft_printf("Game Over! You were caught by an enemy!\n");
			mlx_close_window(game->mlx);
			return ;
		}
		i++;
	}
}
