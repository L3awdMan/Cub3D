/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shade.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zotaj-di <zotaj-di@learner.42.tech>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 00:10:35 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/06/10 00:10:35 by zotaj-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* SECTION 3 (src/render/shade.c): mandatory polish helpers split out of
 * render.c to keep both files Norm-compliant (≤5 functions per file).
 * Contains: per-channel color shader, HUD crosshair, head-bob horizon. */

#include "cub3d_bonus.h"

/**
 * @brief SECTION 3 (src/render/shade.c): per-channel RGB multiply with
 * saturation clamp. Used by wall shading (fog + side darkening) and the
 * floor/ceiling gradient. Channels clamp to [0,255] so a k > 1 (e.g. flash
 * effect) never overflows into the next byte.
 */
unsigned int	shade_rgb(unsigned int color, double k)
{
	int	r;
	int	g;
	int	b;

	r = (int)(((color >> 16) & 0xFF) * k);
	g = (int)(((color >> 8) & 0xFF) * k);
	b = (int)((color & 0xFF) * k);
	if (r < 0)
		r = 0;
	if (g < 0)
		g = 0;
	if (b < 0)
		b = 0;
	if (r > 255)
		r = 255;
	if (g > 255)
		g = 255;
	if (b > 255)
		b = 255;
	return ((unsigned int)((r << 16) | (g << 8) | b));
}

/**
 * @brief SECTION 3 (src/render/shade.c): 9-pixel white crosshair drawn at
 * the geometric center of the buffer. Called from render_frame AFTER the
 * raycast pass so wall stripes never overwrite the cross. Pure put_px,
 * no MLX text calls (whitelist-safe).
 */
void	draw_crosshair(t_cub *cub)
{
	int	cx;
	int	cy;
	int	i;

	cx = WIN_W / 2;
	cy = WIN_H / 2;
	i = -4;
	while (i <= 4)
	{
		put_px(&cub->img, cx + i, cy, 0xFFFFFF);
		put_px(&cub->img, cx, cy + i, 0xFFFFFF);
		i++;
	}
}

/**
 * @brief SECTION 3 (src/render/shade.c): head-bob horizon update.
 * bob_t advances only while a WASD key is held so the view sits still when
 * the player stops. Result lands in cub->horizon; consumed by
 * calc_draw_params (wall stripe re-centering) and draw_ceiling/draw_floor
 * (gradient bounds).
 */
void	update_horizon(t_cub *cub)
{
	int	moving;

	moving = cub->keys[KEY_W] || cub->keys[KEY_S]
		|| cub->keys[KEY_A] || cub->keys[KEY_D];
	if (moving)
		cub->bob_t += BOB_STEP;
	cub->horizon = WIN_H / 2 + (int)(sin(cub->bob_t) * BOB_AMP);
}
