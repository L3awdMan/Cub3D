/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/21 16:20:12 by baelgadi          #+#    #+#             */
/*   Updated: 2026/06/20 18:21:43 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "mlx.h"

/**
 * @brief Zero initializes the cub state
 */
void	cub_init(t_cub *cub)
{
	ft_bzero(cub, sizeof(t_cub));
	cub->parse_fd = -1;
}

/**
 * @brief Initialize MLX context, wiondow and screen image
 */
static void	init_mlx(t_cub *cub)
{
	cub->mlx = mlx_init();
	if (!cub->mlx)
		exit_error(cub, "MLX initialization failed");
	cub->win = mlx_new_window(cub->mlx, WIN_W, WIN_H, "cub3d");
	cub->img.id = mlx_new_image(cub->mlx, WIN_W, WIN_H);
	cub->img.data = mlx_get_data_addr(cub->img.id, &cub->img.bpp,
			&cub->img.line_len, &cub->img.endian);
	cub->img.width = WIN_W;
	cub->img.height = WIN_H;
}

/**
 * @brief Entry point
 * @return 0 on clean exit and 1 on argument error
 */
int	main(int ac, char **av)
{
	t_cub	cub;

	if (ac != 2)
	{
		ft_putstr_fd("Error\nUsage: ./cub3D <map.cub>\n", 2);
		return (1);
	}
	cub_init(&cub);
	parse_file(&cub, av[1]);
	init_mlx(&cub);
	load_textures(&cub);
	register_hooks(&cub);
	mlx_loop(cub.mlx);
	cub_destroy(&cub);
	return (0);
}
