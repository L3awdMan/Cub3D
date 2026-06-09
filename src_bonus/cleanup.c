/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zotaj-di <zotaj-di@learner.42.tech>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 00:10:29 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/06/10 00:10:29 by zotaj-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"
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
void	free_grid(t_map *map)
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
	free(map->row_len);
	map->row_len = NULL;
}

/**
 * @brief Frees texture path strings (were stored during parsing)
 */
void	free_tex_paths(t_map *map)
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
 * @brief SECTION 4 (src_bonus/cleanup.c): frees bonus-only heap data — the
 * door-state grid, the sprite array, the in-flight get_next_line buffer
 * (mid-parse exit_error path) and the GNL static fd_list (drained via the
 * magic fd -42 sentinel exposed by libft's get_next_line).
 */
static void	free_bonus_data(t_cub *cub)
{
	if (cub->doors)
	{
		free(cub->doors);
		cub->doors = NULL;
	}
	if (cub->sprites)
	{
		free(cub->sprites);
		cub->sprites = NULL;
	}
	if (cub->spawn_sprites)
	{
		free(cub->spawn_sprites);
		cub->spawn_sprites = NULL;
	}
	if (cub->pending_line)
	{
		free(cub->pending_line);
		cub->pending_line = NULL;
	}
	get_next_line(-42);
}

/**
 * @brief Destroys all MLX resources and frees map data
 */
void	cub_destroy(t_cub *cub)
{
	if (!cub)
		return ;
	free_textures(cub);
	free_anim_sprites(cub);
	free_weapons(cub);
	free_menu_ui(cub);
	free_mission_ui(cub);
	if (cub->img.id && cub->mlx)
		mlx_destroy_image(cub->mlx, cub->img.id);
	if (cub->win && cub->mlx)
	{
		mlx_mouse_show(cub->mlx, cub->win);
		mlx_destroy_window(cub->mlx, cub->win);
	}
	if (cub->mlx)
	{
		mlx_destroy_display(cub->mlx);
		free(cub->mlx);
	}
	cub->mlx = NULL;
	if (cub->map.grid)
		free_grid(&cub->map);
	free_tex_paths(&cub->map);
	free_bonus_data(cub);
}
