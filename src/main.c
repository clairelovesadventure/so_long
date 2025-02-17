#include "so_long.h"

int	main(int argc, char **argv)
{
	t_game	game;

	if (argc != 2)
		error_exit("Usage: ./so_long map_file.ber");
	if (!parse_map(&game, argv[1]))
		error_exit("Invalid map: check file format and content");
	if (!init_game(&game))
		error_exit("Game initialization failed");
	mlx_loop(game.mlx);
	return (0);
}
