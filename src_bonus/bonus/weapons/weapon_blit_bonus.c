/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   weapon_blit_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zotaj-di <zotaj-di@learner.42.tech>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 00:09:00 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/06/10 00:09:00 by zotaj-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* SECTION 4 (src_bonus/bonus/weapon_blit_bonus.c): low-level helpers for the
 * weapon HUD pass (weapon_draw_bonus.c). frame_bbox() trims a frame to its
 * opaque content so every weapon scales to the same on-screen width regardless
 * of padding; blit_scaled() nearest-neighbour blits that sub-rect, skipping
 * the frame's transparent key. */

#include "cub3d_bonus.h"

/**
 * @brief Nearest-neighbor blits src bounds {sx, sy, sw, sh} into the screen
 * rect {x, y, w, h}, skipping pixels equal to transp.
 */
void	blit_scaled(t_cub *cub, t_img *src, unsigned int transp, int r[8])
{
	int				p[2];
	int				s[2];
	unsigned int	col;

	p[1] = -1;
	while (++p[1] < r[3])
	{
		p[0] = -1;
		while (++p[0] < r[2])
		{
			s[0] = r[4] + p[0] * r[6] / r[2];
			s[1] = r[5] + p[1] * r[7] / r[3];
			col = get_tex_pixel(src, s[0], s[1]);
			if (col != transp)
				put_px(&cub->img, r[0] + p[0], r[1] + p[1], col);
		}
	}
}

/**
 * @brief Blits a whole image scaled to the full WIN_W x WIN_H screen, skipping
 * pixels equal to transp (pass OPAQUE_KEY for a fully opaque overlay).
 */
void	blit_full(t_cub *cub, t_img *f, unsigned int transp)
{
	int	r[8];

	r[0] = 0;
	r[1] = 0;
	r[2] = WIN_W;
	r[3] = WIN_H;
	r[4] = 0;
	r[5] = 0;
	r[6] = f->width;
	r[7] = f->height;
	blit_scaled(cub, f, transp, r);
}

/**
 * @brief Finds the opaque bounding box of one weapon frame into r[4..7] as
 * {min_x, min_y, max_x, max_y}.
 */
void	frame_bbox(t_img *t, unsigned int transp, int r[8])
{
	int				p[2];
	unsigned int	col;

	r[4] = t->width;
	r[5] = t->height;
	r[6] = -1;
	r[7] = -1;
	p[1] = -1;
	while (++p[1] < t->height)
	{
		p[0] = -1;
		while (++p[0] < t->width)
		{
			col = get_tex_pixel(t, p[0], p[1]);
			if (col != transp)
			{
				r[4] = fmin(r[4], p[0]);
				r[5] = fmin(r[5], p[1]);
				r[6] = fmax(r[6], p[0]);
				r[7] = fmax(r[7], p[1]);
			}
		}
	}
}
