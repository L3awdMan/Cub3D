/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_wall.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/12 15:36:03 by baelgadi          #+#    #+#             */
/*   Updated: 2026/04/12 14:21:35 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * @brief Computes the fractional part of the wall hit position (wall_x)
 * @details This is used to determine which column of the texture to sample.
 * If `side = 0` then the hit was vertical so wall_x uses the Y component,
 * otherwise it was horizontal so wall_x uses X
 */
static double	calc_wall_x(t_ray *ray, t_player *p)
{
	double	wall_x;

	if (ray->side == 0)
		wall_x = p->pos_y + ray->perp_dist * ray->dir_y;
	else
		wall_x = p->pos_x + ray->perp_dist * ray->dir_x;
	wall_x -= floor(wall_x);
	return (wall_x);
}

/**
 * @brief Computes the texture X coordinate (with mirror correction)
 * @details We flip tex_x for certain ray directions in order to prevent
 * mirrored textures on opposing wall faces
 */
static int	calc_tex_x(t_ray *ray, t_player *p, t_img *tex)
{
	double	wall_x;
	int		tex_x;

	wall_x = calc_wall_x(ray, p);
	tex_x = (int)(wall_x * tex->width);
	if (ray->side == 0 && ray->dir_x > 0)
		tex_x = (tex->width - 1) - tex_x;
	if (ray->side == 1 && ray->dir_y < 0)
		tex_x = (tex->width - 1) - tex_x;
	return (tex_x);
}

/**
 * @brief Draws the textured pixels of one wall column
 * @details It steps through the texture vertically while sampling 1 pixel per
 * screen row, between draw start and draw end
 *
 * SECTION 3 (src/render/draw_wall.c): each texel is multiplied by dw->shade
 * (fog x side darkening), filled earlier by calc_draw_params.
 */
static void	draw_tex_col(t_cub *cub, int x, t_draw *dw, t_img *tex)
{
	int				y;
	int				tex_y;
	unsigned int	color;

	y = dw->start;
	while (y <= dw->end)
	{
		tex_y = (int)dw->pos;
		if (tex_y >= tex->height)
			tex_y = tex->height - 1;
		if (tex_y < 0)
			tex_y = 0;
		color = shade_rgb(get_tex_pixel(tex, dw->tex_x, tex_y), dw->shade);
		put_px(&cub->img, x, y, color);
		dw->pos += dw->step;
		y++;
	}
}

/**
 * @brief Draw 1 wall column: celing, textured wall stripe and floor
 * @details It selects the correct texture from the ray infos (hit side) and
 * computes coordinates then draws ceiling/wall/floor
 *
 * SECTION 3 (src/render/draw_wall.c): cub->horizon feeds both
 * calc_draw_params and the dw.pos texture anchor, so the stripe stays
 * aligned with the bobbing horizon instead of sliding inside the column.
 */
void	draw_wall_stripe(t_cub *cub, int x, t_ray *ray)
{
	t_draw	dw;
	int		tex_idx;
	t_img	*tex;

	calc_draw_params(&dw, ray, cub->horizon);
	tex_idx = select_texture(ray);
	tex = &cub->tex[tex_idx];
	dw.tex_x = calc_tex_x(ray, &cub->player, tex);
	dw.step = (double)tex->height / dw.height;
	dw.pos = (dw.start - cub->horizon + dw.height / 2) * dw.step;
	draw_ceiling(cub, x, dw.start);
	draw_tex_col(cub, x, &dw, tex);
	draw_floor(cub, x, dw.end + 1);
}
