/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doors_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zotaj-di <zotaj-di@learner.42.tech>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 00:06:53 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/06/10 00:06:53 by zotaj-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* bonus feature #3 — animated
 * doors. Each map cell holding 'D' has a t_door slot: progress runs
 * 0.0 (closed) → 1.0 (fully retracted) and target is the value progress
 * is animating toward. update_doors() advances every door by
 * DOOR_SPEED * delta_time each frame. door_is_open() — used by movement
 * collision — returns true only past DOOR_PASSABLE so the player cannot
 * pinch through a barely-cracked door. The raycaster uses the per-ray
 * door_allows_passage() in door_use_bonus.c for the slide-through cut. */

#include "cub3d_bonus.h"

/**
 * @brief Allocates the flat door-state grid (map.width * map.height slots,
 * zero-initialized so every door starts closed with progress = 0).
 */
void	init_doors(t_cub *cub)
{
	int	count;

	count = cub->map.width * cub->map.height;
	if (count <= 0)
		return ;
	cub->doors = ft_calloc(count, sizeof(t_door));
	if (!cub->doors)
		exit_error(cub, "Memory allocation failed");
}

/**
 * @brief Returns the t_door slot for cell (y, x), or NULL if the cell is
 * out of bounds or is not a 'D' tile.
 */
t_door	*door_at(t_cub *cub, int y, int x)
{
	if (!cub->doors || y < 0 || y >= cub->map.height)
		return (NULL);
	if (x < 0 || x >= cub->map.width)
		return (NULL);
	if (x >= (int)ft_strlen(cub->map.grid[y]))
		return (NULL);
	if (cub->map.grid[y][x] != 'D')
		return (NULL);
	return (&cub->doors[y * cub->map.width + x]);
}

/**
 * @brief Open enough for collision to treat the cell as walkable. Used by
 * apply_movement so the player cannot squeeze through a sliver gap before
 * the slide animation completes.
 */
int	door_is_open(t_cub *cub, int y, int x)
{
	t_door	*d;

	d = door_at(cub, y, x);
	return (d != NULL && d->progress >= DOOR_PASSABLE);
}

/**
 * @brief Raw animation progress at cell (y, x). 0 if not a door. Used by
 * draw_door_stripe to warp wall_x and by door_allows_passage to decide
 * which pixels of the door face are still solid.
 */
double	door_progress_at(t_cub *cub, int y, int x)
{
	t_door	*d;

	d = door_at(cub, y, x);
	if (!d)
		return (0.0);
	return (d->progress);
}

/**
 * @brief Per-frame door driver. Walks every cell and nudges progress
 * toward its target by DOOR_SPEED * delta-time (gettimeofday-based).
 * Clamps to [0, 1]. The very first call seeds door_last_ms with no
 * advance so the first frame's delta is zero (avoids a startup jump).
 */
void	update_doors(t_cub *cub)
{
	long	now;
	double	step;
	int		i;
	int		n;

	now = now_ms();
	if (cub->door_last_ms == 0)
		cub->door_last_ms = now;
	step = (double)(now - cub->door_last_ms) * DOOR_SPEED;
	cub->door_last_ms = now;
	if (!cub->doors)
		return ;
	n = cub->map.width * cub->map.height;
	i = -1;
	while (++i < n)
	{
		if (cub->doors[i].target == 1 && cub->doors[i].progress < 1.0)
			cub->doors[i].progress = fmin(1.0, cub->doors[i].progress + step);
		else if (cub->doors[i].target == 0 && cub->doors[i].progress > 0.0)
			cub->doors[i].progress = fmax(0.0, cub->doors[i].progress - step);
	}
}
