#include "so_long.h"

int	check_filename(char *filename)
{
	size_t	len;

	len = ft_strlen(filename);
	if (len < 4)
		return (0);
	return (ft_strncmp(filename + len - 4, ".ber", 4) == 0);
}

static char	*create_temp(char *line, char c, int len)
{
	char	*temp;

	temp = malloc(len + 2);
	if (!temp)
		return (NULL);
	if (line)
	{
		ft_strlcpy(temp, line, len + 1);
		free(line);
	}
	temp[len] = c;
	temp[len + 1] = '\0';
	return (temp);
}

char	*read_line(int fd)
{
	char	buffer[2];
	char	*line;
	int		ret;
	int		len;

	line = NULL;
	len = 0;
	ret = read(fd, buffer, 1);
	while (ret > 0)
	{
		buffer[ret] = '\0';
		if (buffer[0] == '\n')
			break ;
		line = create_temp(line, buffer[0], len);
		if (!line)
			return (NULL);
		len++;
		ret = read(fd, buffer, 1);
	}
	return (line);
}

void	init_map(t_map *map)
{
	map->grid = NULL;
	map->width = 0;
	map->height = 0;
	map->collectibles = 0;
	map->exit = 0;
	map->player = 0;
}
