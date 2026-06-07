/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_bg.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 15:49:23 by baelgadi          #+#    #+#             */
/*   Updated: 2026/04/12 11:35:47 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * @brief Draws ceiling pixels for column x from y = 0 to end.
 *
 * SECTION 3 (src/render/draw_bg.c): vertical gradient — ceil_col stays
 * full-bright at the top of the screen and lerps down to BG_FADE near the
 * horizon. Cheap atmospheric perspective: ceiling recedes into distance the
 * same way fogged walls do. Denominator is WIN_H/2 (fixed, not cub->horizon)
 * so head-bob doesn't yank the gradient — only the visible band shifts.
 */
void	draw_ceiling(t_cub *cub, int x, int end)
{
	int		y;
	double	k;

	y = 0;
	while (y < end)
	{
		k = 1.0 - (1.0 - BG_FADE) * ((double)y / (WIN_H / 2));
		if (k < BG_FADE)
			k = BG_FADE;
		put_px(&cub->img, x, y, shade_rgb((unsigned int)cub->map.ceil_col, k));
		y++;
	}
}

/**
 * @brief Draws floor pixels for column x from start to WIN_H.
 *
 * SECTION 3 (src/render/draw_bg.c): floor mirrors the ceiling gradient —
 * dim near the horizon, full-bright at the player's feet. Anchored at
 * WIN_H/2 (not cub->horizon) so the gradient slope is stable while the
 * head bobs; only the visible band shifts.
 */
void	draw_floor(t_cub *cub, int x, int start)
{
	int		y;
	double	k;

	y = start;
	while (y < WIN_H)
	{
		k = BG_FADE + (1.0 - BG_FADE)
			* ((double)(y - WIN_H / 2) / (WIN_H / 2));
		if (k < BG_FADE)
			k = BG_FADE;
		if (k > 1.0)
			k = 1.0;
		put_px(&cub->img, x, y, shade_rgb((unsigned int)cub->map.floor_col, k));
		y++;
	}
}
