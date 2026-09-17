/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_wall.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zotaj-di <zotaj-di@learner.42.tech>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 00:10:08 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/06/10 00:10:08 by zotaj-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

/**
 * @brief Computes the fractional part of the wall hit position (wall_x)
 * @details This is used to determine which column of the texture to sample.
 * If `side = 0` then the hit was vertical so wall_x uses the Y component,
 * otherwise it was horizontal so wall_x uses X
 *
 * de-static'd so the door drawer
 * (door_draw_bonus.c) samples the exact same hit fraction.
 */
double	calc_wall_x(t_ray *ray, t_player *p)
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
 * each texel is multiplied by dw->shade
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

static t_img	*wall_texture(t_cub *cub, t_ray *ray, int tex_idx)
{
	char	c;

	c = cub->map.grid[ray->map_y][ray->map_x];
	if (c == '4')
		return (&cub->wall_alt[0]);
	if (c == '5')
		return (&cub->wall_alt[1]);
	if (c == '6')
		return (&cub->wall_alt[2]);
	return (&cub->tex[tex_idx]);
}

/**
 * @brief Draw 1 wall column: celing, textured wall stripe and floor
 * @details It selects the correct texture from the ray infos (hit side) and
 * computes coordinates then draws ceiling/wall/floor
 *
 * door cells hand off to draw_door_stripe; the wall
 * depth is recorded in cub->zbuf for the sprite pass. cub->horizon
 * feeds calc_draw_params and the dw.pos anchor to track the head-bob.
 */
void	draw_wall_stripe(t_cub *cub, int x, t_ray *ray)
{
	t_draw	dw;
	int		tex_idx;
	t_img	*tex;

	if (ray->door)
	{
		draw_door_stripe(cub, x, ray);
		return ;
	}
	calc_draw_params(&dw, ray, cub->horizon);
	cub->zbuf[x] = ray->perp_dist;
	tex_idx = select_texture(ray);
	tex = wall_texture(cub, ray, tex_idx);
	dw.tex_x = calc_tex_x(ray, &cub->player, tex);
	dw.step = (double)tex->height / dw.height;
	dw.pos = (dw.start - cub->horizon + dw.height / 2) * dw.step;
	draw_ceiling(cub, x, dw.start);
	draw_tex_col(cub, x, &dw, tex);
	draw_floor(cub, x, dw.end + 1);
}
