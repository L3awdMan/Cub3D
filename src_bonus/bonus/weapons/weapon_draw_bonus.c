/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   weapon_draw_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zotaj-di <zotaj-di@learner.42.tech>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 00:09:07 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/06/10 00:09:07 by zotaj-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* weapon HUD pass. Runs
 * after the world layers (walls/sprites/minimap) and before the crosshair so
 * the reticle stays on top. Each weapon has five full-frame HUD XPMs. Frame 0
 * is idle; firing walks through the sequence so the muzzle-flash frame appears.
 * None-pixels are skipped. */

#include "cub3d_bonus.h"

/**
 * @brief Returns the current HUD frame: idle frame 0 or the shot frame derived
 * from elapsed shot time.
 */
static int	weapon_frame(t_cub *cub)
{
	long	elapsed;
	int		frame;

	if (!cub->weapon.firing)
		return (0);
	elapsed = now_ms() - cub->weapon.fire_ms;
	frame = (int)(elapsed / WPN_FRAME_MS);
	if (frame < 0)
		return (0);
	if (frame >= WPN_FRAMES)
		return (WPN_FRAMES - 1);
	return (frame);
}

/**
 * @brief Draws the active weapon frame centered along the bottom edge: the
 * idle frame normally, the firing sequence while shooting.
 */
static void	draw_weapon_img(t_cub *cub)
{
	t_img			*t;
	unsigned int	transp;
	int				r[8];
	int				frame;

	frame = weapon_frame(cub);
	t = &cub->weapon.img[cub->weapon.current][frame];
	transp = cub->weapon.transp[cub->weapon.current][frame];
	frame_bbox(t, transp, r);
	if (r[6] < r[4] || r[7] < r[5])
		return ;
	r[6] = r[6] - r[4] + 1;
	r[7] = r[7] - r[5] + 1;
	r[2] = (int)(WIN_W * WPN_SCALE);
	r[3] = r[2] * r[7] / r[6];
	r[0] = (WIN_W - r[2]) / 2;
	r[1] = WIN_H - r[3];
	blit_scaled(cub, t, transp, r);
}

/**
 * @brief Per-frame fire clock. KEY_SPACE held or a left-click starts a shot,
 * then the five-frame firing animation plays before snapping back to idle.
 */
void	update_weapon(t_cub *cub)
{
	t_weapon	*w;
	long		now;

	w = &cub->weapon;
	now = now_ms();
	if ((cub->keys[KEY_SPACE] || w->shoot) && !w->firing)
	{
		w->firing = 1;
		w->fire_ms = now;
		shoot_hitscan(cub);
	}
	w->shoot = 0;
	if (w->firing && now - w->fire_ms >= WPN_FIRE_MS)
		w->firing = 0;
}

/**
 * @brief Draws the weapon HUD for this frame.
 */
void	draw_weapon(t_cub *cub)
{
	draw_weapon_img(cub);
}
