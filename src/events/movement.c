/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/12 12:58:03 by baelgadi          #+#    #+#             */
/*   Updated: 2026/04/12 19:06:53 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * @brief FIX 6 helper (src/events/movement.c): true iff the cell containing
 * point (x, y) is OOB or a wall. The previous single-point can_move() tested
 * only the new cell center, so the player rubbed flush against walls and
 * could clip diagonally through corners. COLLISION (0.2) was defined in
 * cub3d.h but never referenced.
 */
static int	corner_blocked(t_map *map, double x, double y)
{
	int	mx;
	int	my;

	mx = (int)x;
	my = (int)y;
	if (my < 0 || my >= map->height)
		return (1);
	if (mx < 0 || mx >= (int)ft_strlen(map->grid[my]))
		return (1);
	return (map->grid[my][mx] == '1');
}

/**
 * @brief FIX 6 (src/events/movement.c): bounding-box collision test using the
 * COLLISION padding from cub3d.h. Checks all 4 corners of a square centered on
 * (x, y) — if any corner sits in a wall or OOB cell, the move is rejected.
 * Keeps a 0.2-cell air gap between the player and every wall.
 */
static int	can_move(t_map *map, double x, double y)
{
	if (corner_blocked(map, x - COLLISION, y - COLLISION))
		return (0);
	if (corner_blocked(map, x + COLLISION, y - COLLISION))
		return (0);
	if (corner_blocked(map, x - COLLISION, y + COLLISION))
		return (0);
	if (corner_blocked(map, x + COLLISION, y + COLLISION))
		return (0);
	return (1);
}

/**
 * @brief Moves the player along a vector with a given sign (1 or -1)
 */
static void	move_along(t_cub *cub, double vx, double vy, int sign)
{
	double	nx;
	double	ny;

	nx = cub->player.pos_x + sign * vx * MOVE_SPD;
	ny = cub->player.pos_y + sign * vy * MOVE_SPD;
	if (can_move(&cub->map, nx, cub->player.pos_y))
		cub->player.pos_x = nx;
	if (can_move(&cub->map, cub->player.pos_x, ny))
		cub->player.pos_y = ny;
}

/**
 * @brief Applies movement based on the held keys (WASD)
 * @note Calls move along with dir (going forward / backward moves the player 
 * in the dir direction) or plane vector (going left / right moves the player
 * perpendicular to the dir direction)
 * @param cub 
 */
void	apply_movement(t_cub *cub)
{
	if (cub->keys[KEY_W])
		move_along(cub, cub->player.dir_x, cub->player.dir_y, 1);
	if (cub->keys[KEY_S])
		move_along(cub, cub->player.dir_x, cub->player.dir_y, -1);
	if (cub->keys[KEY_A])
		move_along(cub, cub->player.plane_x, cub->player.plane_y, -1);
	if (cub->keys[KEY_D])
		move_along(cub, cub->player.plane_x, cub->player.plane_y, 1);
}
