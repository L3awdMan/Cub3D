/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 00:10:27 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/06/20 18:24:17 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"
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
 * @brief Caches each grid row's length once after parsing so the per-frame
 * floor/ceiling cast (is_boss_room) and line-of-sight checks bound-test with
 * an int lookup instead of an ft_strlen() call per pixel. Rows are ragged, so
 * the length is stored individually.
 */
void	init_row_len(t_cub *cub)
{
	int	y;

	cub->map.row_len = ft_calloc(cub->map.height, sizeof(int));
	if (!cub->map.row_len)
		exit_error(cub, "Memory allocation failed");
	y = -1;
	while (++y < cub->map.height)
		cub->map.row_len[y] = (int)ft_strlen(cub->map.grid[y]);
}

static void	init_game(t_cub *cub, char *map_path)
{
	cub_init(cub);
	cub->show_minimap = 1;
	cub->floor = 1;
	cub->player.hp = PLAYER_HP_MAX;
	parse_file(cub, map_path);
	init_row_len(cub);
	init_doors(cub);
	init_sprites(cub);
	init_mlx(cub);
	load_textures(cub);
	load_anim_sprites(cub);
	load_weapons(cub);
	capture_spawn(cub);
	cub->game_state = GS_INTRO;
}

/**
 * @brief Entry point
 * @return 0 on clean exit and 1 on argument error
 *
 * SECTION 4 (src_bonus/main.c): the minimap starts enabled (KEY_M toggles);
 * init_doors allocates the door grid and init_sprites scans '2' tiles into
 * sprite entities — both run after parse_file, once map dimensions are set.
 */
int	main(int ac, char **av)
{
	t_cub	cub;

	if (ac != 2)
	{
		ft_putstr_fd("Error\nUsage: ./cub3D <map.cub>\n", 2);
		return (1);
	}
	init_game(&cub, av[1]);
	register_hooks(&cub);
	mlx_loop(cub.mlx);
	cub_destroy(&cub);
	return (0);
}
