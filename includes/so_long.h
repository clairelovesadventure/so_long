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

typedef struct s_map {
    char    **grid;
    size_t  width;
    int     height;
    int     collectibles;
    int     exit;
    int     player;
} t_map;

typedef struct s_enemy {
    size_t x;
    int y;
    int direction;  // 0: 上, 1: 右, 2: 下, 3: 左
} t_enemy;

typedef struct s_game {
    void        *mlx;
    mlx_image_t *win;
    t_map       map;
    int         moves;
    size_t      player_x;
    int         player_y;
    int         window_width;
    int         window_height;
    
    mlx_image_t *player_img;
    mlx_image_t *wall_img;
    mlx_image_t *collect_img;
    mlx_image_t *exit_img;
    mlx_image_t *floor_img;
    
    // 动画相关
    mlx_texture_t *player_textures[4];
    mlx_texture_t *collectible_textures[8];
    int current_player_frame;
    int current_collectible_frame;
    int frame_counter;
    
    // 敌人相关
    mlx_texture_t *enemy_texture;
    mlx_image_t *enemy_img;
    t_enemy *enemies;
    int enemy_count;

    // 其他纹理
    mlx_texture_t *wall_texture;
    mlx_texture_t *exit_texture;
    mlx_texture_t *floor_texture;
} t_game;

// map_parser.c
int     parse_map(t_game *game, char *filename);

// game.c
int     init_game(t_game *game);
int     handle_key(int keycode, t_game *game);

// graphics.c
int     init_graphics(t_game *game);
void    render_game(t_game *game);

// utils.c
void    free_map(t_map *map);
void    error_exit(char *message);

// 新增函数声明
void update_animations(t_game *game);
void move_enemies(t_game *game);
int init_enemies(t_game *game);
void check_enemy_collision(t_game *game);

#endif 