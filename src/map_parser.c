#include "so_long.h"
#include <stdlib.h>
#include "libft.h"

static int validate_map_chars(t_map *map)
{
    int i;
    int j;

    i = 0;
    while (i < map->height)
    {
        j = 0;
        while ((size_t)j < map->width)
        {
            if (map->grid[i][j] != '0' && map->grid[i][j] != '1' &&
                map->grid[i][j] != 'C' && map->grid[i][j] != 'E' &&
                map->grid[i][j] != 'P' && map->grid[i][j] != 'N')
                return (0);
            if (map->grid[i][j] == 'C')
                map->collectibles++;
            if (map->grid[i][j] == 'E')
                map->exit++;
            if (map->grid[i][j] == 'P')
                map->player++;
            j++;
        }
        i++;
    }
    return (map->collectibles > 0 && map->exit == 1 && map->player == 1);
}

static char *read_line(int fd)
{
    char buffer[2];
    char *line;
    char *temp;
    int ret;
    int len;

    ft_printf("Reading line...\n");

    line = NULL;
    len = 0;
    while ((ret = read(fd, buffer, 1)) > 0)
    {
        buffer[ret] = '\0';
        if (buffer[0] == '\n')
        {
            ft_printf("Found newline, line length: %d\n", len);
            break;
        }
        temp = malloc(len + 2);
        if (!temp)
            return (NULL);
        if (line)
        {
            ft_strlcpy(temp, line, len + 1);
            free(line);
        }
        temp[len] = buffer[0];
        temp[len + 1] = '\0';
        line = temp;
        len++;
    }

    if (line)
        ft_printf("Line content: '%s'\n", line);
    else
        ft_printf("Empty line\n");

    return (line);
}

static int validate_map_shape(t_map *map)
{
    int i;

    ft_printf("First line width: %zu\n", map->width);

    i = 1;
    while (i < map->height)
    {
        ft_printf("Line %d width: %zu\n", i, ft_strlen(map->grid[i]));
        if (ft_strlen(map->grid[i]) != map->width)
            return (0);
        i++;
    }
    return (1);
}

static int validate_map_walls(t_map *map)
{
    int i;
    
    // 检查第一行和最后一行
    i = 0;
    while ((size_t)i < map->width)
    {
        if (map->grid[0][i] != '1' || map->grid[map->height - 1][i] != '1')
            return (0);
        i++;
    }

    // 检查第一列和最后一列
    i = 0;
    while (i < map->height)
    {
        if (map->grid[i][0] != '1' || map->grid[i][map->width - 1] != '1')
            return (0);
        i++;
    }
    return (1);
}

static int check_filename(char *filename)
{
    size_t len;

    len = ft_strlen(filename);
    if (len < 4)  // 文件名至少要有 .ber (4个字符)
        return (0);
    return (ft_strncmp(filename + len - 4, ".ber", 4) == 0);
}

static int flood_fill(t_map *map, int x, int y, char **visited)
{
    // 检查边界和墙
    if (x < 0 || (size_t)x >= map->width || y < 0 || y >= map->height || 
        map->grid[y][x] == '1' || visited[y][x])
        return (0);
    
    // 标记为已访问
    visited[y][x] = 1;
    
    // 递归检查四个方向
    static const int dx[] = {0, 1, 0, -1};
    static const int dy[] = {-1, 0, 1, 0};
    
    for (int i = 0; i < 4; i++)
        flood_fill(map, x + dx[i], y + dy[i], visited);
    
    return (1);
}

static int validate_map_path(t_map *map)
{
    char **visited;
    int i;
    int j;
    int player_x;
    int player_y;
    int valid;

    // 分配访问数组
    visited = malloc(sizeof(char *) * map->height);
    if (!visited)
        return (0);
    i = 0;
    while (i < map->height)
    {
        visited[i] = malloc(sizeof(char) * map->width);
        if (!visited[i])
        {
            while (--i >= 0)
                free(visited[i]);
            free(visited);
            return (0);
        }
        ft_memset(visited[i], 0, map->width);
        i++;
    }

    // 找到玩家位置
    player_x = -1;
    player_y = -1;
    i = 0;
    while (i < map->height)
    {
        j = 0;
        while ((size_t)j < map->width)
        {
            if (map->grid[i][j] == 'P')
            {
                player_x = j;
                player_y = i;
                break;
            }
            j++;
        }
        if (player_x != -1)
            break;
        i++;
    }

    // 从玩家位置开始填充
    flood_fill(map, player_x, player_y, visited);

    // 检查所有收集品和出口是否可达
    valid = 1;
    i = 0;
    while (i < map->height)
    {
        j = 0;
        while ((size_t)j < map->width)
        {
            if ((map->grid[i][j] == 'C' || map->grid[i][j] == 'E') && !visited[i][j])
            {
                valid = 0;
                break;
            }
            j++;
        }
        i++;
    }

    // 清理内存
    i = 0;
    while (i < map->height)
        free(visited[i++]);
    free(visited);

    return (valid);
}

int parse_map(t_game *game, char *filename)
{
    int     fd;
    char    *line;
    int     i;
    int     line_count;

    ft_printf("Parsing map file: %s\n", filename);

    if (!check_filename(filename))
    {
        ft_printf("Invalid file extension (must be .ber)\n");
        return (0);
    }

    fd = open(filename, O_RDONLY);
    if (fd < 0)
    {
        ft_printf("Failed to open map file\n");
        return (0);
    }
    
    // 初始化地图结构
    game->map.grid = NULL;
    game->map.width = 0;
    game->map.height = 0;
    game->map.collectibles = 0;
    game->map.exit = 0;
    game->map.player = 0;

    // 计算地图尺寸
    game->map.height = 0;
    line_count = 0;
    while ((line = read_line(fd)))
    {
        if (line_count == 0)
        {
            game->map.width = ft_strlen(line);
            if (game->map.width == 0)
            {
                ft_printf("Invalid map: empty line\n");
                free(line);
                close(fd);
                return (0);
            }
        }
        else if (ft_strlen(line) != game->map.width)
        {
            ft_printf("Invalid map: line %d has different length\n", line_count + 1);
            free(line);
            close(fd);
            return (0);
        }
        // 检查行是否包含无效字符
        for (int i = 0; line[i]; i++)
        {
            if (line[i] != '0' && line[i] != '1' && line[i] != 'C' &&
                line[i] != 'E' && line[i] != 'P' && line[i] != 'N')
            {
                ft_printf("Invalid map: invalid character '%c' at line %d\n", line[i], line_count + 1);
                free(line);
                close(fd);
                return (0);
            }
        }
        line_count++;
        free(line);
    }
    game->map.height = line_count;

    // 检查地图大小是否合理
    if (game->map.width > (size_t)50 || game->map.height > 50)
    {
        ft_printf("Invalid map: too large (max 50x50)\n");
        close(fd);
        return (0);
    }
    if (game->map.width < (size_t)3 || game->map.height < 3)
    {
        ft_printf("Invalid map: too small (min 3x3)\n");
        close(fd);
        return (0);
    }

    // 分配地图内存
    game->map.grid = malloc(sizeof(char *) * game->map.height);
    if (!game->map.grid)
    {
        close(fd);
        return (0);
    }

    // 读取地图内容
    lseek(fd, 0, SEEK_SET);
    i = 0;
    while ((line = read_line(fd)) && i < game->map.height)
    {
        game->map.grid[i] = line;
        i++;
    }
    
    // 在返回前添加额外的验证
    if (!validate_map_shape(&game->map))
    {
        ft_printf("Invalid map shape: all rows must have the same length\n");
        free_map(&game->map);
        close(fd);
        return (0);
    }
    if (!validate_map_walls(&game->map))
    {
        ft_printf("Invalid map walls: map must be surrounded by walls\n");
        free_map(&game->map);
        close(fd);
        return (0);
    }
    if (!validate_map_chars(&game->map))
    {
        ft_printf("Invalid map characters: must contain exactly one P, one E, and at least one C\n");
        ft_printf("Found: %d P, %d E, %d C\n", game->map.player, game->map.exit, game->map.collectibles);
        free_map(&game->map);
        close(fd);
        return (0);
    }
    if (!validate_map_path(&game->map))
    {
        ft_printf("Invalid map path: all collectibles and exit must be reachable\n");
        free_map(&game->map);
        close(fd);
        return (0);
    }

    ft_printf("Map validation successful:\n");
    ft_printf("- Size: %dx%d\n", game->map.width, game->map.height);
    ft_printf("- Collectibles: %d\n", game->map.collectibles);
    ft_printf("- Exit: %d\n", game->map.exit);
    ft_printf("- Player: %d\n", game->map.player);

    close(fd);
    return (1);
}

int init_enemies(t_game *game)
{
    int i, j, count = 0;
    
    // 计算敌人数量
    for (i = 0; i < game->map.height; i++)
        for (j = 0; j < (int)game->map.width; j++)
            if (game->map.grid[i][j] == 'N')
                count++;
    
    game->enemy_count = count;
    if (count == 0)
        return (1);
    
    // 分配敌人数组内存
    game->enemies = malloc(sizeof(t_enemy) * count);
    if (!game->enemies)
        return (0);
    
    // 初始化敌人位置
    count = 0;
    for (i = 0; i < game->map.height; i++)
    {
        for (j = 0; j < (int)game->map.width; j++)
        {
            if (game->map.grid[i][j] == 'N')
            {
                game->enemies[count].x = j;
                game->enemies[count].y = i;
                game->enemies[count].direction = count % 4;  // 随机初始方向
                count++;
            }
        }
    }
    return (1);
}