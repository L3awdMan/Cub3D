/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door_use_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zotaj-di <zotaj-di@learner.42.tech>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 00:06:50 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/06/10 00:06:50 by zotaj-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* SECTION 4 (src_bonus/bonus/door_use_bonus.c): KEY_E door interaction and
 * the per-ray pass-through test consumed by the DDA. toggle_door flips the
 * faced door's target (open ↔ closed); the slide itself runs in
 * update_doors(). A close attempt while the player stands on the door
 * tile is rejected so the slide cannot crush the player.
 *
 * door_allows_passage() implements the central-slide cut: the door panel
 * retracts symmetrically from the middle, so a ray that hits the cell
 * within [0.5 - p/2, 0.5 + p/2] of the door face (where p is the current
 * progress) passes through and the DDA keeps stepping past the cell. */

#include "cub3d_bonus.h"

/**
 * @brief Toggles the door the player faces. Flips target so update_doors
 * slides it open or closed at DOOR_SPEED. If the player is currently
 * standing on the door tile and the door is at all open, refuses to start
 * closing it (no crush).
 */
void	toggle_door(t_cub *cub)
{
	int		fx;
	int		fy;
	t_door	*d;

	fx = (int)(cub->player.pos_x + cub->player.dir_x * DOOR_REACH);
	fy = (int)(cub->player.pos_y + cub->player.dir_y * DOOR_REACH);
	d = door_at(cub, fy, fx);
	if (!d)
		return ;
	if (d->target == 1 && fx == (int)cub->player.pos_x
		&& fy == (int)cub->player.pos_y)
		return ;
	if (d->target == 1)
		d->target = 0;
	else
		d->target = 1;
}

/**
 * @brief Per-ray door pass-through test. Returns 1 when the ray's hit
 * fraction along the door face lies within the retracted central band
 * [0.5 - p/2, 0.5 + p/2], where p is the door's current progress. The
 * DDA caller uses this to decide whether to keep stepping past the cell
 * (ray pass-through, reveals the geometry behind during the slide) or
 * stop and treat the cell as a wall slice.
 */
int	door_allows_passage(t_cub *cub, int y, int x, double hit)
{
	double	p;

	p = door_progress_at(cub, y, x);
	if (p <= 0.0)
		return (0);
	if (p >= 1.0)
		return (1);
	return (hit > 0.5 - p * 0.5 && hit < 0.5 + p * 0.5);
}
