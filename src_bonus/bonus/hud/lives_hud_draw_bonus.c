/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lives_hud_draw_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zotaj-di <zotaj-di@learner.42.tech>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 00:07:42 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/06/10 00:07:42 by zotaj-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* "enemies remaining"
 * lives counter — draw side. Picks the per-floor black slot in the floor-HUD
 * panel, fits the current lives frame inside it preserving aspect, and blits
 * it (skipping the transparent key) over the HUD. */

#include "cub3d_bonus.h"

/**
 * @brief Fills slot {x, y, w, h} with the black-slot bounds of the active
 * floor's HUD panel (measured 1:1 against the 1280-wide HUD). Defaults to
 * floor 1.
 */
static void	lives_slot(t_cub *cub, int slot[4])
{
	if (cub->floor == 2)
	{
		slot[0] = 1051;
		slot[1] = 14;
		slot[2] = 187;
		slot[3] = 41;
		return ;
	}
	slot[0] = 1068 + (cub->floor == 3) * -8;
	slot[1] = 14;
	slot[2] = 175 + (cub->floor == 3) * -7;
	slot[3] = 50 + (cub->floor == 3) * -1;
}

/**
 * @brief Centers frame f inside slot preserving its aspect, writing the
 * blit_scaled rect into r[0..3] (dest) and r[4..7] (full source).
 */
static void	fit_rect(t_img *f, int slot[4], int r[8])
{
	int	w;
	int	h;

	w = slot[2];
	h = w * f->height / f->width;
	if (h > slot[3])
	{
		h = slot[3];
		w = h * f->width / f->height;
	}
	r[0] = slot[0] + (slot[2] - w) / 2 - 1;
	r[1] = slot[1] - 4;
	r[2] = w;
	r[3] = h;
	r[4] = 0;
	r[5] = 0;
	r[6] = f->width;
	r[7] = f->height;
}

/**
 * @brief Draws the lives panel for the current count into the floor-HUD slot.
 * No-op until the frames are lazily loaded.
 */
void	draw_lives_hud(t_cub *cub)
{
	int	slot[4];
	int	r[8];
	int	idx;

	if (!cub->lives_hud[0].id)
		return ;
	idx = cub->lives;
	if (idx < 0)
		idx = 0;
	if (idx > LIVES_MAX)
		idx = LIVES_MAX;
	lives_slot(cub, slot);
	fit_rect(&cub->lives_hud[idx], slot, r);
	blit_scaled(cub, &cub->lives_hud[idx], cub->lives_transp, r);
}
