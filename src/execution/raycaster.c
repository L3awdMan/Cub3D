/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycaster.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 13:31:07 by baelgadi          #+#    #+#             */
/*   Updated: 2026/04/12 19:06:28 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * @brief DDA loop (steps through grid cells until a wall is hit)
 * @details At each step it advances to the nearest grid line (X or Y). The side
 * variable records the axis that was crossed last (0 = X and 1 = Y) thus
 * determining the wall face for later texture selection
 */
static void	run_dda(t_ray *ray, t_map *map)
{
	while (CASTING)
	{
		if (ray->side_dist_x < ray->side_dist_y)
		{
			ray->side_dist_x += ray->delta_x;
			ray->map_x += ray->step_x;
			ray->side = 0;
		}
		else
		{
			ray->side_dist_y += ray->delta_y;
			ray->map_y += ray->step_y;
			ray->side = 1;
		}
		if (ray->map_y < 0 || ray->map_y >= map->height || ray->map_x < 0
			|| ray->map_x >= (int)ft_strlen(map->grid[ray->map_y]))
			break ;
		if (map->grid[ray->map_y][ray->map_x] == '1')
			break ;
	}
}

/**
 * @brief Casts a single ray for screen col x and draws the result
 */
static void	cast_single_ray(t_cub *cub, int x)
{
	t_ray	ray;

	init_ray(&ray, &cub->player, x);
	init_step_side(&ray, &cub->player);
	run_dda(&ray, &cub->map);
	draw_wall_stripe(cub, x, &ray);
}

/**
 * @brief Casts rays for every screen col (left -> right)
 */
void	cast_all_rays(t_cub *cub)
{
	int	x;

	x = 0;
	while (x < WIN_W)
	{
		cast_single_ray(cub, x);
		x++;
	}
}
