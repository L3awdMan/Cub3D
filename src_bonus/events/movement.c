/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zotaj-di <zotaj-di@learner.42.tech>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 00:09:31 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/06/10 00:09:31 by zotaj-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

/**
 * @brief FIX 6 helper (src/events/movement.c): true iff the cell containing
 * point (x, y) is OOB, a wall, or a not-yet-open door. The old single-point
 * can_move() tested only the new cell center, so the player rubbed flush
 * against walls and could clip diagonally through corners.
 *
 * a 'D' cell also blocks until door_is_open().
 */
static int	corner_blocked(t_cub *cub, double x, double y)
{
	t_map	*map;
	int		mx;
	int		my;
	char	c;

	map = &cub->map;
	mx = (int)x;
	my = (int)y;
	if (my < 0 || my >= map->height)
		return (1);
	if (mx < 0 || mx >= map->row_len[my])
		return (1);
	c = map->grid[my][mx];
	if (ft_strchr(WALL_CHARS, c))
		return (1);
	if (c == 'D' && !door_is_open(cub, my, mx))
		return (1);
	return (0);
}

/**
 * @brief FIX 6 (src/events/movement.c): bounding-box collision test using the
 * COLLISION padding from cub3d.h. Checks all 4 corners of a square centered on
 * (x, y) — if any corner sits in a wall, a closed door, or an OOB cell, the
 * move is rejected. Keeps a 0.2-cell air gap between the player and walls.
 */
static int	can_move(t_cub *cub, double x, double y)
{
	if (corner_blocked(cub, x - COLLISION, y - COLLISION))
		return (0);
	if (corner_blocked(cub, x + COLLISION, y - COLLISION))
		return (0);
	if (corner_blocked(cub, x - COLLISION, y + COLLISION))
		return (0);
	if (corner_blocked(cub, x + COLLISION, y + COLLISION))
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
	if (can_move(cub, nx, cub->player.pos_y))
		cub->player.pos_x = nx;
	if (can_move(cub, cub->player.pos_x, ny))
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
