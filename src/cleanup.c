/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/21 20:07:59 by baelgadi          #+#    #+#             */
/*   Updated: 2026/06/21 23:02:18 by zotaj-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "mlx.h"

/**
 * @brief Prints error msg to STDERR, cleanup and exit
 */
void	exit_error(t_cub *cub, char *msg)
{
	ft_putstr_fd("Error\n", 2);
	ft_putendl_fd(msg, 2);
	cub_destroy(cub);
	exit (1);
}

/**
 * @brief Frees the map grid rows and the grid array
 */
static void	free_grid(t_map *map)
{
	int	i;

	i = 0;
	while (i < map->height)
	{
		free(map->grid[i]);
		i++;
	}
	free(map->grid);
	map->grid = NULL;
}

/**
 * @brief Frees texture path strings (were stored during parsing)
 */
static void	free_tex_paths(t_map *map)
{
	int	i;

	i = 0;
	while (i < TEX_COUNT)
	{
		free(map->tex_path[i]);
		map->tex_path[i] = NULL;
		i++;
	}
}

/**
 * @brief Destroys all MLX resources and frees map data
 */
void	cub_destroy(t_cub *cub)
{
	if (!cub)
		return ;
	if (cub->parse_fd >= 0)
	{
		close(cub->parse_fd);
		cub->parse_fd = -1;
	}
	free_textures(cub);
	if (cub->img.id && cub->mlx)
		mlx_destroy_image(cub->mlx, cub->img.id);
	if (cub->win && cub->mlx)
		mlx_destroy_window(cub->mlx, cub->win);
	if (cub->mlx)
	{
		mlx_destroy_display(cub->mlx);
		free(cub->mlx);
	}
	cub->mlx = NULL;
	if (cub->map.grid)
		free_grid(&cub->map);
	free_tex_paths(&cub->map);
	get_next_line(-42);
}
