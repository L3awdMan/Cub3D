/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite_col_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zotaj-di <zotaj-di@learner.42.tech>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 00:07:23 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/06/10 00:07:23 by zotaj-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* sprite column blitter.
 * Draws one transformed sprite column-by-column from its current animation
 * frame (s->tex). Skips texels equal to the frame's recorded transparent
 * color (s->transp, sampled from the XPM's (0, 0) corner — the None fill).
 * Columns whose wall (cub->zbuf[x]) is nearer than the sprite are dropped so
 * sprites are correctly occluded by walls and doors. */

#include "cub3d_bonus.h"

/**
 * @brief Fills the on-screen rectangle for a sprite. `full` is the full-cell
 * billboard size at this depth. Aliens fill it; pickups are shrunk to
 * PICKUP_SCALE and aligned to the same floor line (sy1) so they rest on the
 * ground rather than float at eye level.
 */
void	sprite_rect(t_cub *cub, t_sprite *sp, t_spr *s, int full)
{
	if (sp->type >= SP_PICKUP)
		s->size = (int)(full * PICKUP_SCALE);
	else
		s->size = full;
	s->sx0 = s->screen_x - s->size / 2;
	s->sx1 = s->screen_x + s->size / 2;
	s->sy1 = cub->horizon + full / 2;
	s->sy0 = s->sy1 - s->size;
}

/**
 * @brief Draws one vertical strip of a sprite at screen column x, mapping
 * screen rows to texture texels and skipping transparent ones. Texels are
 * fog-shaded by s->shade so far sprites recede the same way walls do.
 */
static void	draw_sprite_strip(t_cub *cub, t_spr *s, int x)
{
	int				y;
	int				tx;
	int				ty;
	unsigned int	color;

	tx = (x - s->sx0) * s->tex->width / s->size;
	y = s->sy0;
	while (y < s->sy1)
	{
		ty = (y - s->sy0) * s->tex->height / s->size;
		color = get_tex_pixel(s->tex, tx, ty);
		if (color != s->transp && y >= 0 && y < WIN_H)
			put_px(&cub->img, x, y, shade_rgb(color, s->shade));
		y++;
	}
}

/**
 * @brief Draws every visible column of a transformed sprite. A column is
 * drawn only when on-screen and nearer than the wall depth recorded in the
 * z-buffer for that column.
 */
void	draw_sprite_cols(t_cub *cub, t_spr *s)
{
	int	x;

	if (s->size <= 0 || !s->tex)
		return ;
	x = s->sx0;
	while (x < s->sx1)
	{
		if (x >= 0 && x < WIN_W && s->depth < cub->zbuf[x])
			draw_sprite_strip(cub, s, x);
		x++;
	}
}

/**
 * @brief Squared distance from the player to a sprite (used for sort order).
 */
static double	spr_dist2(t_cub *cub, t_sprite *s)
{
	double	dx;
	double	dy;

	dx = s->x - cub->player.pos_x;
	dy = s->y - cub->player.pos_y;
	return (dx * dx + dy * dy);
}

/**
 * @brief Bubble-sorts the sprite array far-to-near so nearer sprites are
 * drawn last (on top). The array is tiny, so a simple sort is plenty.
 */
void	sort_sprites(t_cub *cub)
{
	int			i;
	int			j;
	t_sprite	tmp;

	i = -1;
	while (++i < cub->sprite_count - 1)
	{
		j = -1;
		while (++j < cub->sprite_count - 1 - i)
		{
			if (spr_dist2(cub, &cub->sprites[j])
				< spr_dist2(cub, &cub->sprites[j + 1]))
			{
				tmp = cub->sprites[j];
				cub->sprites[j] = cub->sprites[j + 1];
				cub->sprites[j + 1] = tmp;
			}
		}
	}
}
