#include "so_long.h"

static int	init_window(t_game *game)
{
	game->window_width = game->map.width * TILE_SIZE;
	game->window_height = game->map.height * TILE_SIZE;
	game->mlx = mlx_init(game->window_width, game->window_height,
			"so_long", true);
	if (!game->mlx)
		return (0);
	game->moves = 0;
	game->frame_counter = 0;
	game->current_player_frame = 0;
	game->current_collectible_frame = 0;
	return (1);
}

static void	init_null_pointers(t_game *game)
{
	int	i;

	game->player_img = NULL;
	game->wall_img = NULL;
	game->collect_img = NULL;
	game->exit_img = NULL;
	game->floor_img = NULL;
	game->enemy_img = NULL;
	i = 0;
	while (i < 4)
		game->player_textures[i++] = NULL;
	i = 0;
	while (i < 8)
		game->collectible_textures[i++] = NULL;
	game->wall_texture = NULL;
	game->exit_texture = NULL;
	game->floor_texture = NULL;
	game->enemy_texture = NULL;
}

static int	find_player_position(t_game *game)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (i < game->map.height)
	{
		j = 0;
		while (j < game->map.width)
		{
			if (game->map.grid[i][j] == 'P')
			{
				game->player_x = j;
				game->player_y = i;
				return (1);
			}
			j++;
		}
		i++;
	}
	return (0);
}

static int	init_hooks(t_game *game)
{
	mlx_key_hook(game->mlx, key_hook, game);
	mlx_close_hook(game->mlx, cleanup_hook, game);
	mlx_loop_hook(game->mlx, (void *)update_animations, game);
	render_game(game);
	return (1);
}

int	init_game(t_game *game)
{
	if (!init_window(game))
		return (0);
	if (!find_player_position(game))
		return (0);
	init_null_pointers(game);
	if (!load_images(game))
		return (0);
	if (!init_enemies(game))
		return (0);
	return (init_hooks(game));
}
