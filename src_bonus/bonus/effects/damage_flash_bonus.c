/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   damage_flash_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zotaj-di <zotaj-di@learner.42.tech>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 00:06:57 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/06/10 00:06:57 by zotaj-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* SECTION 4 (src_bonus/bonus/damage_flash_bonus.c): Call-of-Duty style red
 * hit-flash. After the frame is fully drawn, draw_damage_flash() tints the
 * image buffer red, strongest at the screen edges (a vignette) and clear in the
 * center, with an alpha that fades from HURT_MAX_A to 0 over HURT_FADE_MS after
 * the last hit (player.hurt_ms). It is a pure post-process on cub->img and is
 * skipped entirely when no flash is active. */

#include "cub3d_bonus.h"

/**
 * @brief Blends one ARGB pixel toward pure red by alpha a (0..1): red rises, the
 * green and blue channels are scaled down so the pixel reddens as a grows.
 */
static unsigned int	blend_red(unsigned int c, double a)
{
	int	r;
	int	g;
	int	b;

	r = (c >> 16) & 0xFF;
	g = (c >> 8) & 0xFF;
	b = c & 0xFF;
	r = r + (int)((255 - r) * a);
	g = g - (int)(g * a);
	b = b - (int)(b * a);
	return ((r << 16) | (g << 8) | b);
}

/**
 * @brief Vignette weight at pixel (x, y): squared normalized distance from the
 * screen center, clamped to 1. 0 at the center, 1 at the corners.
 */
static double	edge_factor(int x, int y)
{
	double	nx;
	double	ny;
	double	d;

	nx = (x - WIN_W / 2.0) / (WIN_W / 2.0);
	ny = (y - WIN_H / 2.0) / (WIN_H / 2.0);
	d = nx * nx + ny * ny;
	if (d > 1.0)
		d = 1.0;
	return (d);
}

/**
 * @brief Tints one image row: each pixel reddened by (edge weight * a) so the
 * effect concentrates at the screen border.
 */
static void	tint_row(t_img *img, int y, double a)
{
	int				x;
	unsigned int	*p;
	double			f;

	x = -1;
	while (++x < WIN_W)
	{
		f = edge_factor(x, y) * a;
		if (f <= 0.0)
			continue ;
		p = (unsigned int *)(img->data
				+ y * img->line_len + x * (img->bpp / 8));
		*p = blend_red(*p, f);
	}
}

/**
 * @brief Current flash alpha: 0 when never hit or the fade has elapsed, else a
 * linear ramp from HURT_MAX_A down to 0 across HURT_FADE_MS since the last hit.
 */
static double	hurt_alpha(t_cub *cub)
{
	long	elapsed;

	if (cub->player.hurt_ms == 0)
		return (0.0);
	elapsed = now_ms() - cub->player.hurt_ms;
	if (elapsed < 0 || elapsed >= HURT_FADE_MS)
		return (0.0);
	return (HURT_MAX_A * (1.0 - (double)elapsed / HURT_FADE_MS));
}

/**
 * @brief Draws the red damage vignette over the finished frame. No-op while no
 * flash is active, so the full-screen scan only runs during the brief fade.
 */
void	draw_damage_flash(t_cub *cub)
{
	double	a;
	int		y;

	if (cub->flash_ms > cub->player.hurt_ms)
		return ;
	a = hurt_alpha(cub);
	if (a <= 0.0)
		return ;
	y = -1;
	while (++y < WIN_H)
		tint_row(&cub->img, y, a);
}
