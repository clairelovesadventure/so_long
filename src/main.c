/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 21:33:40 by shutan            #+#    #+#             */
/*   Updated: 2025/02/20 18:54:11 by shutan        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

int	main(int argc, char **argv)
{
	t_game	game;

	if (argc != 2)
		error_exit("Usage: ./so_long map_file.ber");
	ft_memset(&game, 0, sizeof(t_game));
	if (!parse_map(&game, argv[1]))
	{
		cleanup(&game);
		error_exit("Invalid map");
	}
	if (!init_game(&game))
	{
		cleanup(&game);
		error_exit("Game init failed");
	}
	mlx_loop(game.mlx);
	cleanup(&game);
	return (0);
}
