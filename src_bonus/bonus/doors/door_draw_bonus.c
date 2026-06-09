/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door_draw_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zotaj-di <zotaj-di@learner.42.tech>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 00:06:47 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/06/10 00:06:47 by zotaj-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* SECTION 4 (src_bonus/bonus/door_draw_bonus.c): door column renderer.
 * Closed doors render as a normal wall stripe with cub->door_tex. While
 * a door is animating, the ray only reaches this function on columns
 * where it hit the still-unretracted panel (the DDA pass-through test
 * in dda_step lets center-band rays continue past the cell, so they
 * naturally render the geometry behind the door instead). For those
 * panel columns warp_door_wx() compresses the door texture into the
 * remaining panel width so the visible door looks like two halves
 * retracting symmetrically from the cell's center. */

#include "cub3d_bonus.h"

/**
 * @brief Warps the raw door-face hit fraction (ray->door_hit, 0..1) into
 * a texture-sample coordinate that compresses the full door panel onto
 * the unopened side as the door retracts.
 *
 *   panel = 0.5 - progress / 2   (remaining width of one side panel)
 *
 * Left half (wx < 0.5) lives in [0, panel] → mapped to [0, 0.5].
 * Right half (wx ≥ 0.5) lives in [0.5 + progress/2, 1] → mapped to [0.5, 1].
 */
static double	warp_door_wx(double wx, double progress)
{
	double	half;
	double	panel;

	if (progress <= 0.0 || progress >= 1.0)
		return (wx);
	half = progress * 0.5;
	panel = 0.5 - half;
	if (panel <= 1e-6)
		return (wx);
	if (wx < 0.5)
		return ((wx / panel) * 0.5);
	return (0.5 + ((wx - (0.5 + half)) / panel) * 0.5);
}

/**
 * @brief Fills one screen column with door-texture texels between
 * dw->start and dw->end. Mirrors the wall column drawer with door_tex as
 * the sampled texture and dw->shade as the per-column shade multiplier.
 */
static void	draw_door_col(t_cub *cub, int x, t_draw *dw, t_img *tex)
{
	int				y;
	int				tex_x;
	int				tex_y;
	unsigned int	color;

	tex_x = dw->tex_x;
	if (tex_x < 0)
		tex_x = 0;
	if (tex_x >= tex->width)
		tex_x = tex->width - 1;
	y = dw->start;
	while (y <= dw->end)
	{
		tex_y = (int)dw->pos;
		color = get_tex_pixel(tex, tex_x, tex_y);
		put_px(&cub->img, x, y, shade_rgb(color, dw->shade));
		dw->pos += dw->step;
		y++;
	}
}

/**
 * @brief Draws a full door column: ceiling above, the (possibly warped)
 * door panel, floor below. Records the column's depth in cub->zbuf so
 * sprites are occluded by closed doors just like solid walls. Reads the
 * cell's current progress to compress the panel texture into whichever
 * side of the door has not yet retracted.
 */
void	draw_door_stripe(t_cub *cub, int x, t_ray *ray)
{
	t_draw	dw;
	t_img	*tex;
	double	wx;
	double	progress;

	calc_draw_params(&dw, ray, cub->horizon);
	cub->zbuf[x] = ray->perp_dist;
	progress = door_progress_at(cub, ray->map_y, ray->map_x);
	tex = &cub->door_tex;
	if (progress > 0.0)
		tex = &cub->door_open_tex;
	wx = warp_door_wx(ray->door_hit, progress);
	dw.tex_x = (int)(wx * tex->width);
	dw.step = (double)tex->height / dw.height;
	dw.pos = (dw.start - cub->horizon + dw.height / 2) * dw.step;
	draw_ceiling(cub, x, dw.start);
	draw_door_col(cub, x, &dw, tex);
	draw_floor(cub, x, dw.end + 1);
}
