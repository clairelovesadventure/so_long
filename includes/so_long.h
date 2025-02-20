/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 21:35:11 by shutan            #+#    #+#             */
/*   Updated: 2025/02/20 18:56:51 by shutan        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SO_LONG_H
# define SO_LONG_H

# include <MLX42/MLX42.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include "libft.h"

# define TILE_SIZE 32
# define ENEMY_MOVE_INTERVAL 30  // 降低敌人移动速度
# define ANIMATION_SPEED 12      // 降低动画速度
# define MAP_WIDTH 25
# define MAP_HEIGHT 9
# define RENDER_DELAY 2  // 控制渲染频率

typedef struct s_map
{
	char		**grid;
	size_t		width;
	size_t		height;
	int			collectibles;
	int			exit;
	int			player;
}	t_map;

typedef struct s_enemy
{
	size_t		x;
	size_t		y;
	int			direction;
}	t_enemy;

typedef struct s_game
{
	void			*mlx;
	mlx_image_t		*win;
	t_map			map;
	int				moves;
	size_t			player_x;
	size_t			player_y;
	int				window_width;
	int				window_height;

	mlx_image_t		*player_img;
	mlx_image_t		*wall_img;
	mlx_image_t		*collect_img;
	mlx_image_t		*exit_img;
	mlx_image_t		*floor_img;
	mlx_image_t		*moves_text;

	mlx_texture_t	*player_textures[4];
	mlx_texture_t	*collectible_textures[8];
	int				current_player_frame;
	int				current_collectible_frame;
	int				frame_counter;

	mlx_texture_t	*enemy_texture;
	mlx_image_t		*enemy_img;
	t_enemy			*enemies;
	int				enemy_count;

	mlx_texture_t	*wall_texture;
	mlx_texture_t	*exit_texture;
	mlx_texture_t	*floor_texture;
}	t_game;

// map_parser.c
int		parse_map(t_game *game, char *filename);

// game.c
int		init_enemies(t_game *game);

// game_cleanup.c
void	cleanup(t_game *game);
void	cleanup_hook(void *param);
void	cleanup_images(t_game *game);
void	cleanup_textures(t_game *game);

// game_movement.c
int		can_move(t_game *game, size_t new_x, size_t new_y);
void	update_position(t_game *game, size_t new_x, size_t new_y);
void	key_hook(mlx_key_data_t keydata, void *param);

// game_images.c
int		load_images(t_game *game);

// graphics.c
int		render_static_tile(t_game *game, size_t i, size_t j);
int		render_dynamic_tile(t_game *game, size_t i, size_t j);
void	render_game(t_game *game);
void	update_moves_display(t_game *game);

// utils.c
void	free_map(t_map *map);
void	error_exit(char *message);

// 新增函数声明
void	update_animations(t_game *game);
void	move_enemies(t_game *game);
void	check_enemy_collision(t_game *game);
void	copy_grid_dir(t_game *game, char grid[MAP_HEIGHT][MAP_WIDTH],
			int to_temp);
void	update_enemy(t_game *game, char grid[MAP_HEIGHT][MAP_WIDTH], int i);

// map_validation.c
int		validate_map_chars(t_map *map);
int		validate_map_shape(t_map *map);
int		validate_map_walls(t_map *map);
int		validate_map_path(t_map *map);

// map_utils.c
int		check_filename(char *filename);
char	*read_line(int fd);
void	init_map(t_map *map);

// map_path.c
char	**init_visited(t_map *map);
void	cleanup_visited(char **visited, size_t height);
int		check_path_validity(t_map *map, char **visited);

// game_init.c
int		init_game(t_game *game);

#endif
