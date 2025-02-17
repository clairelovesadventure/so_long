#include "so_long.h"

static void cleanup(t_game *game)
{
    // 删除图像
    if (game->player_img)
        mlx_delete_image(game->mlx, game->player_img);
    if (game->wall_img)
        mlx_delete_image(game->mlx, game->wall_img);
    if (game->collect_img)
        mlx_delete_image(game->mlx, game->collect_img);
    if (game->exit_img)
        mlx_delete_image(game->mlx, game->exit_img);
    if (game->floor_img)
        mlx_delete_image(game->mlx, game->floor_img);

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

    // 释放地图内存
    free_map(&game->map);

    // 终止 MLX
    if (game->mlx)
        mlx_terminate(game->mlx);
}

int main(int argc, char **argv)
{
    t_game  game;

    if (argc != 2)
        error_exit("Usage: ./so_long map_file.ber");

    // 解析地图
    if (!parse_map(&game, argv[1]))
        error_exit("Invalid map: check file format and content");

    // 初始化游戏
    if (!init_game(&game))
    {
        cleanup(&game);
        error_exit("Game initialization failed");
    }

    // 开始游戏循环
    mlx_loop(game.mlx);

    // 清理资源
    cleanup(&game);

    return (0);
} 