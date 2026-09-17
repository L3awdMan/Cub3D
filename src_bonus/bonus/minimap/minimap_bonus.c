/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zotaj-di <zotaj-di@learner.42.tech>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 00:07:52 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/06/10 00:07:52 by zotaj-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* bonus feature #2 — minimap.
 * A player-centered top-down patch blitted over the 3D view each frame via
 * put_px (no extra MLX calls). Shows a MM_RADIUS-cell window of the grid,
 * the player as a dot, the look direction in red, and the FOV edges in blue.
 * Toggled with KEY_M (see hooks.c). All pixels are clipped to the minimap
 * box so it never bleeds into the 3D scene. */

#include "cub3d_bonus.h"

/**
 * @brief Color for one map cell on the minimap. Out-of-bounds and space
 * cells read as void; '1' is wall, 'D' is door, everything else is floor.
 */
static unsigned int	mm_cell_color(t_cub *cub, int gy, int gx)
{
	char	c;

	if (gy < 0 || gy >= cub->map.height)
		return (MM_VOID);
	if (gx < 0 || gx >= (int)ft_strlen(cub->map.grid[gy]))
		return (MM_VOID);
	c = cub->map.grid[gy][gx];
	if (ft_strchr(WALL_CHARS, c))
		return (MM_WALL);
	if (c == 'D')
		return (MM_DOOR);
	if (c == ' ')
		return (MM_VOID);
	return (MM_FLOOR);
}

/**
 * @brief Draws the player dot on top of the already-rendered cell grid.
 * Every pixel is gated by mm_put_circ so the marker stays inside the disc.
 */
static void	mm_overlay(t_cub *cub)
{
	int			i;
	int			j;
	int			dist;

	i = -4;
	while (++i < 5)
	{
		j = -4;
		while (++j < 5)
		{
			dist = i * i + j * j;
			if (dist <= 16)
				mm_put_circ(cub, MM_X + MM_SIZE / 2 + j,
					MM_Y + MM_SIZE / 2 + i, 0xFFFFFF);
			if (dist <= 9)
				mm_put_circ(cub, MM_X + MM_SIZE / 2 + j,
					MM_Y + MM_SIZE / 2 + i, MM_PLAYER);
		}
	}
}

static void	mm_enemy_dot(t_cub *cub, t_sprite *sp, double px, double py)
{
	int	dot[3];

	dot[0] = MM_X + (int)((sp->x - (px - MM_RADIUS)) * MM_CELL);
	dot[1] = MM_Y + (int)((sp->y - (py - MM_RADIUS)) * MM_CELL);
	if (sp->type == SP_FLUID)
	{
		dot[2] = 5;
		mm_dot(cub, dot, MM_BOSS_RING);
		dot[2] = 3;
		mm_dot(cub, dot, MM_BOSS);
	}
	else
	{
		dot[2] = 3;
		mm_dot(cub, dot, MM_ENEMY);
	}
}

static void	mm_enemies(t_cub *cub, double px, double py)
{
	int	i;

	i = -1;
	while (++i < cub->sprite_count)
	{
		if (cub->sprites[i].type < SP_PICKUP
			&& cub->sprites[i].state != SP_DYING)
			mm_enemy_dot(cub, &cub->sprites[i], px, py);
	}
}

/**
 * @brief Renders the whole minimap: a MM_RADIUS-cell window of the grid
 * centered on the player, the player overlay, then the outer ring on top.
 * Skipped entirely when cub->show_minimap is 0.
 */
void	draw_minimap(t_cub *cub)
{
	double	px;
	double	py;
	int		gy;
	int		gx;

	if (!cub->show_minimap)
		return ;
	px = cub->player.pos_x;
	py = cub->player.pos_y;
	mm_backdrop(cub);
	gy = (int)(py - MM_RADIUS) - 1;
	while (++gy <= (int)(py + MM_RADIUS))
	{
		gx = (int)(px - MM_RADIUS) - 1;
		while (++gx <= (int)(px + MM_RADIUS))
			mm_square(cub, MM_X + (int)((gx - (px - MM_RADIUS)) * MM_CELL),
				MM_Y + (int)((gy - (py - MM_RADIUS)) * MM_CELL),
				mm_cell_color(cub, gy, gx));
	}
	mm_enemies(cub, px, py);
	mm_overlay(cub);
	mm_outline(cub);
}
