/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heal_flash_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zotaj-di <zotaj-di@learner.42.tech>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 00:07:00 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/06/10 00:07:00 by zotaj-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* SECTION 4 (src_bonus/bonus/heal_flash_bonus.c): pickup flash, the positive
 * twin of the red damage flash. After the frame is drawn, draw_pickup_flash()
 * softly adds cub->flash_rgb (green for meat, gold for money) only inside a
 * fixed screen-edge band. It fades from FLASH_MAX_A to 0 over FLASH_FADE_MS
 * since cub->flash_ms, and is skipped when damage is the newer flash event. */

#include "cub3d_bonus.h"

/**
 * @brief Soft colored overlay: target channels are added lightly, while other
 * channels are preserved so pickup flashes do not crush the scene color.
 */
static unsigned int	blend_soft(unsigned int c, unsigned int target, double a)
{
	int	r;
	int	g;
	int	b;

	r = (c >> 16) & 0xFF;
	g = (c >> 8) & 0xFF;
	b = c & 0xFF;
	r = r + (int)(((target >> 16) & 0xFF) * a);
	g = g + (int)(((target >> 8) & 0xFF) * a);
	b = b + (int)((target & 0xFF) * a);
	if (r > 255)
		r = 255;
	if (g > 255)
		g = 255;
	if (b > 255)
		b = 255;
	return ((r << 16) | (g << 8) | b);
}

/**
 * @brief Edge-only weight at (x, y): fully clear after FLASH_EDGE_PX pixels
 * from every border, then stronger toward the actual screen edge.
 */
static double	edge_factor(int x, int y)
{
	int		edge;
	double	f;

	edge = x;
	if (WIN_W - 1 - x < edge)
		edge = WIN_W - 1 - x;
	if (y < edge)
		edge = y;
	if (WIN_H - 1 - y < edge)
		edge = WIN_H - 1 - y;
	if (edge >= FLASH_EDGE_PX)
		return (0.0);
	f = 1.0 - (double)edge / FLASH_EDGE_PX;
	return (f * f);
}

/**
 * @brief Tints one image row toward target: each pixel blended by (edge weight
 * * a) so the effect concentrates at the screen border.
 */
static void	tint_row(t_img *img, int y, double a, unsigned int target)
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
		*p = blend_soft(*p, target, f);
	}
}

/**
 * @brief Current flash alpha: 0 when never fired or the fade has elapsed, else
 * a linear ramp from FLASH_MAX_A down to 0 across FLASH_FADE_MS since flash_ms.
 */
static double	flash_alpha(t_cub *cub)
{
	long	elapsed;

	if (cub->flash_ms == 0)
		return (0.0);
	elapsed = now_ms() - cub->flash_ms;
	if (elapsed < 0 || elapsed >= FLASH_FADE_MS)
		return (0.0);
	return (FLASH_MAX_A * (1.0 - (double)elapsed / FLASH_FADE_MS));
}

/**
 * @brief Draws the pickup vignette over the finished frame. No-op while no
 * flash is active, so the full-screen scan only runs during the brief fade.
 */
void	draw_pickup_flash(t_cub *cub)
{
	double	a;
	int		y;

	a = flash_alpha(cub);
	if (a <= 0.0)
		return ;
	if (cub->player.hurt_ms >= cub->flash_ms)
		return ;
	y = -1;
	while (++y < WIN_H)
		tint_row(&cub->img, y, a, cub->flash_rgb);
}
