#include "so_long.h"

void render_game(t_game *game)
{
    // 添加参数检查
    if (!game || !game->mlx)
        return;

    static int first_render = 1;

    // 如果是第一次渲染，创建所有静态图像
    if (first_render)
    {
        first_render = 0;

        game->floor_img = mlx_texture_to_image(game->mlx, game->floor_texture);
        game->wall_img = mlx_texture_to_image(game->mlx, game->wall_texture);
        game->exit_img = mlx_texture_to_image(game->mlx, game->exit_texture);
        
        // 检查静态图像创建是否成功
        if (!game->floor_img || !game->wall_img || !game->exit_img)
        {
            ft_printf("Failed to create static images\n");
            return;
        }
        
        // 只渲染背景和静态元素
        for (int i = 0; i < game->map.height; i++)
        {
            for (size_t j = 0; j < game->map.width; j++)
            {
                int x = j * TILE_SIZE;
                int y = i * TILE_SIZE;

                // 先渲染地板
                if (mlx_image_to_window(game->mlx, game->floor_img, x, y) < 0)
                {
                    ft_printf("Failed to render floor at %zu,%d\n", j, i);
                    return;
                }

                // 再渲染墙和出口
                if (game->map.grid[i][j] == '1')
                {
                    if (mlx_image_to_window(game->mlx, game->wall_img, x, y) < 0)
                    {
                        ft_printf("Failed to render wall at %zu,%d\n", j, i);
                        return;
                    }
                }
                else if (game->map.grid[i][j] == 'E')
                {
                    if (mlx_image_to_window(game->mlx, game->exit_img, x, y) < 0)
                    {
                        ft_printf("Failed to render exit at %zu,%d\n", j, i);
                        return;
                    }
                }
            }
        }
    }

    // 更新动态元素
    if (game->player_img)
        mlx_delete_image(game->mlx, game->player_img);
    if (game->collect_img)
        mlx_delete_image(game->mlx, game->collect_img);
    if (game->enemy_img)
        mlx_delete_image(game->mlx, game->enemy_img);

    // 创建新的动态元素
    game->player_img = mlx_texture_to_image(game->mlx, 
        game->player_textures[game->current_player_frame]);
    game->collect_img = mlx_texture_to_image(game->mlx, 
        game->collectible_textures[game->current_collectible_frame]);
    game->enemy_img = mlx_texture_to_image(game->mlx, game->enemy_texture);

    // 检查创建是否成功
    if (!game->player_img || !game->collect_img || !game->enemy_img)
    {
        ft_printf("Failed to create images\n");
        return;
    }

    // 更新动态元素位置
    for (int i = 0; i < game->map.height; i++)
    {
        for (size_t j = 0; j < game->map.width; j++)
        {
            int x = j * TILE_SIZE;
            int y = i * TILE_SIZE;

            if (game->map.grid[i][j] == 'P')
            {
                if (mlx_image_to_window(game->mlx, game->player_img, x, y) < 0)
                {
                    ft_printf("Failed to render player at %zu,%d\n", j, i);
                    return;
                }
            }
            else if (game->map.grid[i][j] == 'C')
            {
                if (mlx_image_to_window(game->mlx, game->collect_img, x, y) < 0)
                {
                    ft_printf("Failed to render collectible at %zu,%d\n", j, i);
                    return;
                }
            }
            else if (game->map.grid[i][j] == 'N')
            {
                if (mlx_image_to_window(game->mlx, game->enemy_img, x, y) < 0)
                {
                    ft_printf("Failed to render enemy at %zu,%d\n", j, i);
                    return;
                }
            }
        }
    }
}

int init_graphics(t_game *game)
{
    render_game(game);
    return (1);
}
