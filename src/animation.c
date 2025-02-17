#include "so_long.h"

void update_animations(t_game *game)
{
    game->frame_counter++;
    
    // 只在需要时重新渲染
    bool need_render = false;
    
    // 更新玩家动画
    if (game->frame_counter % (ANIMATION_SPEED * 2) == 0)  // 降低动画速度
    {
        game->current_player_frame = (game->current_player_frame + 1) % 4;
        game->current_collectible_frame = (game->current_collectible_frame + 1) % 8;
        need_render = true;
    }
    
    // 更新敌人位置
    if (game->frame_counter % ENEMY_MOVE_INTERVAL == 0)
    {
        move_enemies(game);
        need_render = true;
    }
    
    // 检查与敌人的碰撞
    check_enemy_collision(game);
    
    // 如果需要，进行渲染
    if (need_render)
        render_game(game);
}

void move_enemies(t_game *game)
{
    static const int dx[] = {0, 1, 0, -1};  // 方向数组：上、右、下、左
    static const int dy[] = {-1, 0, 1, 0};
    char temp_grid[MAP_HEIGHT][MAP_WIDTH];  // 临时网格用于存储移动结果
    
    // 复制当前地图状态
    for (size_t i = 0; i < game->map.height; i++)
        for (size_t j = 0; j < game->map.width; j++)
            temp_grid[i][j] = game->map.grid[i][j];
    
    for (int i = 0; i < game->enemy_count; i++)
    {
        t_enemy *enemy = &game->enemies[i];
        int new_x = enemy->x + dx[enemy->direction];
        int new_y = enemy->y + dy[enemy->direction];
        
        // 如果遇到障碍物或其他敌人，改变方向
        if (game->map.grid[new_y][new_x] == '1' || 
            game->map.grid[new_y][new_x] == 'C' || 
            game->map.grid[new_y][new_x] == 'E' ||
            game->map.grid[new_y][new_x] == 'N')
        {
            enemy->direction = (enemy->direction + 1) % 4;
            temp_grid[enemy->y][enemy->x] = 'N';  // 保持原位置
            continue;
        }
        
        // 在临时网格中更新位置
        temp_grid[enemy->y][enemy->x] = '0';
        enemy->x = new_x;
        enemy->y = new_y;
        temp_grid[new_y][new_x] = 'N';
    }
    
    // 将临时网格复制回游戏地图
    for (size_t i = 0; i < game->map.height; i++)
        for (size_t j = 0; j < game->map.width; j++)
            game->map.grid[i][j] = temp_grid[i][j];
}

void check_enemy_collision(t_game *game)
{
    for (int i = 0; i < game->enemy_count; i++)
    {
        if (game->player_x == game->enemies[i].x && 
            game->player_y == game->enemies[i].y)
        {
            ft_printf("Game Over! You were caught by an enemy!\n");
            mlx_close_window(game->mlx);
            return;
        }
    }
} 