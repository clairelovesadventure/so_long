#include "so_long.h"

// 函数声明
static void cleanup(t_game *game);
static int load_images(t_game *game);
static int can_move(t_game *game, int new_x, int new_y);
static void update_position(t_game *game, int new_x, int new_y);
static void key_hook(mlx_key_data_t keydata, void *param);

static int load_images(t_game *game)
{
    // 加载玩家动画帧
    for (int i = 0; i < 4; i++)
    {
        char filename[32];
        snprintf(filename, sizeof(filename), "textures/player_%d.png", i);
        game->player_textures[i] = mlx_load_png(filename);
        if (!game->player_textures[i])
            return (0);
    }

    // 加载收集品动画帧
    for (int i = 0; i < 8; i++)
    {
        char filename[32];
        snprintf(filename, sizeof(filename), "textures/collectible_%d.png", i);
        game->collectible_textures[i] = mlx_load_png(filename);
        if (!game->collectible_textures[i])
            return (0);
    }

    game->wall_texture = mlx_load_png("textures/wall.png");
    game->exit_texture = mlx_load_png("textures/exit.png");
    game->floor_texture = mlx_load_png("textures/floor.png");
    game->enemy_texture = mlx_load_png("textures/enemy.png");

    if (!game->wall_texture || !game->exit_texture || 
        !game->floor_texture || !game->enemy_texture)
        return (0);

    // 转换为图像
    game->wall_img = mlx_texture_to_image(game->mlx, game->wall_texture);
    game->exit_img = mlx_texture_to_image(game->mlx, game->exit_texture);
    game->floor_img = mlx_texture_to_image(game->mlx, game->floor_texture);
    game->enemy_img = mlx_texture_to_image(game->mlx, game->enemy_texture);

    return (game->wall_img && game->exit_img && 
            game->floor_img && game->enemy_img);
}

static int can_move(t_game *game, int new_x, int new_y)
{
    char target;

    target = game->map.grid[new_y][new_x];
    if (target == '1')
        return (0);
    if (target == 'E' && game->map.collectibles > 0)
        return (0);
    return (1);
}

static void update_position(t_game *game, int new_x, int new_y)
{
    char *target;

    target = &game->map.grid[new_y][new_x];
    if (*target == 'C')
    {
        game->map.collectibles--;
        *target = '0';
    }
    else if (*target == 'E' && game->map.collectibles == 0)
    {
        ft_printf("You won in %d moves!\n", game->moves + 1);
        cleanup(game);
        mlx_close_window(game->mlx);
        return;
    }
    game->map.grid[game->player_y][game->player_x] = '0';
    game->player_x = new_x;
    game->player_y = new_y;
    game->map.grid[new_y][new_x] = 'P';
    game->moves++;
    ft_printf("Moves: %d\n", game->moves);
}

static void key_hook(mlx_key_data_t keydata, void *param)
{
    t_game *game;
    int new_x;
    int new_y;

    if (keydata.action != MLX_PRESS)
        return;

    game = (t_game *)param;
    new_x = game->player_x;
    new_y = game->player_y;

    if (keydata.key == MLX_KEY_ESCAPE)
    {
        cleanup(game);
        mlx_close_window(game->mlx);
    }
    else if (keydata.key == MLX_KEY_W || keydata.key == MLX_KEY_UP)
        new_y--;
    else if (keydata.key == MLX_KEY_A || keydata.key == MLX_KEY_LEFT)
        new_x--;
    else if (keydata.key == MLX_KEY_S || keydata.key == MLX_KEY_DOWN)
        new_y++;
    else if (keydata.key == MLX_KEY_D || keydata.key == MLX_KEY_RIGHT)
        new_x++;

    if (can_move(game, new_x, new_y))
    {
        update_position(game, new_x, new_y);
        render_game(game);
    }
}

static void cleanup(t_game *game)
{
    // 清理静态图像
    if (game->floor_img)
        mlx_delete_image(game->mlx, game->floor_img);
    if (game->wall_img)
        mlx_delete_image(game->mlx, game->wall_img);
    if (game->exit_img)
        mlx_delete_image(game->mlx, game->exit_img);

    // 清理动态图像
    if (game->player_img)
        mlx_delete_image(game->mlx, game->player_img);
    if (game->collect_img)
        mlx_delete_image(game->mlx, game->collect_img);
    if (game->enemy_img)
        mlx_delete_image(game->mlx, game->enemy_img);

    // 清理玩家动画帧
    for (int i = 0; i < 4; i++)
    {
        if (game->player_textures[i])
            mlx_delete_texture(game->player_textures[i]);
    }

    // 清理收集品动画帧
    for (int i = 0; i < 8; i++)
    {
        if (game->collectible_textures[i])
            mlx_delete_texture(game->collectible_textures[i]);
    }

    // 清理其他纹理
    if (game->wall_texture)
        mlx_delete_texture(game->wall_texture);
    if (game->exit_texture)
        mlx_delete_texture(game->exit_texture);
    if (game->floor_texture)
        mlx_delete_texture(game->floor_texture);
    if (game->enemy_texture)
        mlx_delete_texture(game->enemy_texture);
}

int init_game(t_game *game)
{
    ft_printf("Initializing game...\n");
    // 使用实际的地图尺寸
    game->window_width = game->map.width * TILE_SIZE;
    game->window_height = game->map.height * TILE_SIZE;

    game->mlx = mlx_init(game->window_width, game->window_height,
                         "so_long", true);
    if (!game->mlx)
    {
        ft_printf("Failed to initialize MLX\n");
        return (0);
    }

    // 初始化游戏状态
    game->moves = 0;
    game->frame_counter = 0;
    game->current_player_frame = 0;
    game->current_collectible_frame = 0;
    
    // 找到玩家初始位置
    for (int i = 0; i < game->map.height; i++)
    {
        for (size_t j = 0; j < game->map.width; j++)
        {
            if (game->map.grid[i][j] == 'P')
            {
                game->player_x = j;
                game->player_y = i;
                ft_printf("Found player at %d,%d\n", j, i);
            }
        }
    }

    // 初始化所有指针为 NULL
    game->player_img = NULL;
    game->wall_img = NULL;
    game->collect_img = NULL;
    game->exit_img = NULL;
    game->floor_img = NULL;
    game->enemy_img = NULL;

    for (int i = 0; i < 4; i++)
        game->player_textures[i] = NULL;
    for (int i = 0; i < 8; i++)
        game->collectible_textures[i] = NULL;

    game->wall_texture = NULL;
    game->exit_texture = NULL;
    game->floor_texture = NULL;
    game->enemy_texture = NULL;

    if (!load_images(game))
    {
        ft_printf("Failed to load images\n");
        return (0);
    }

    // 初始化敌人
    if (!init_enemies(game))
    {
        ft_printf("Failed to initialize enemies\n");
        return (0);
    }

    // 设置按键钩子
    mlx_key_hook(game->mlx, key_hook, game);

    // 设置动画循环函数
    mlx_loop_hook(game->mlx, (void *)update_animations, game);

    // 初始渲染
    render_game(game);

    return (1);
}
