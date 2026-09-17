/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycaster.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zotaj-di <zotaj-di@learner.42.tech>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 00:09:43 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/06/10 00:09:43 by zotaj-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

/**
 * @brief Advances the DDA one grid cell along the nearer axis and records
 * which axis was crossed in ray->side.
 */
static void	advance_ray(t_ray *ray)
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
}

/**
 * @brief Steps the DDA one cell and returns 1 if the ray should stop.
 *
 * WALL_CHARS are solid walls
 * (door flag cleared); a 'D' tile is queried with the per-ray hit fraction. The
 * door slides retracts from its center, so a ray that hits the cell within
 * the retracted central band [0.5 - p/2, 0.5 + p/2] passes through and the
 * DDA keeps stepping — that is how the player can already see what's
 * behind the door while it is still opening.
 */
static int	dda_step(t_cub *cub, t_ray *ray)
{
	t_map	*map;
	char	c;
	double	hit;

	map = &cub->map;
	advance_ray(ray);
	if (ray->map_y < 0 || ray->map_y >= map->height || ray->map_x < 0
		|| ray->map_x >= (int)ft_strlen(map->grid[ray->map_y]))
		return (1);
	c = map->grid[ray->map_y][ray->map_x];
	if (ft_strchr(WALL_CHARS, c))
		return (1);
	if (c == 'D')
	{
		hit = ray_hit_fraction(ray, &cub->player);
		if (door_allows_passage(cub, ray->map_y, ray->map_x, hit))
			return (0);
		ray->door = 1;
		ray->door_hit = hit;
		return (1);
	}
	return (0);
}

/**
 * @brief DDA loop — steps through grid cells until dda_step says stop.
 */
static void	run_dda(t_cub *cub, t_ray *ray)
{
	while (CASTING)
	{
		if (dda_step(cub, ray))
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
	ray.door = 0;
	init_step_side(&ray, &cub->player);
	run_dda(cub, &ray);
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
