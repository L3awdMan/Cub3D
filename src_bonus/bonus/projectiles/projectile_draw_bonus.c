/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   projectile_draw_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zotaj-di <zotaj-di@learner.42.tech>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 00:08:17 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/06/10 00:08:17 by zotaj-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* draws in-flight enemy
 * projectiles in the sprite pass. Each active shot is wrapped in a temporary
 * t_sprite (its per-role proj art type) so it reuses the camera transform
 * (sprite_transform) and the z-buffered column blitter (draw_sprite_cols) —
 * walls still occlude
 * it. Unlike floor-resting pickups the billboard is centered on the horizon at
 * PROJ_SCALE of a cell so the ball flies at eye level. */

#include "cub3d_bonus.h"

/**
 * @brief Re-centers the transformed billboard: a small (PROJ_SCALE) square
 * centered on the screen-space horizon so the projectile floats at eye level
 * instead of resting on the floor line like a pickup.
 */
static void	proj_rect(t_cub *cub, t_spr *s, int size)
{
	s->size = size;
	s->sx0 = s->screen_x - size / 2;
	s->sx1 = s->screen_x + size / 2;
	s->sy0 = cub->horizon - size / 2;
	s->sy1 = cub->horizon + size / 2;
}

/**
 * @brief Projects and draws one projectile via a temporary sprite carrying its
 * per-role proj art (looped on the shared now_ms clock), then overrides its
 * rect so
 * it is a small eye-level billboard.
 */
static void	draw_one_proj(t_cub *cub, t_proj *p)
{
	t_sprite	sp;
	t_spr		s;
	int			full;

	sp.x = p->x;
	sp.y = p->y;
	sp.type = p->type;
	sp.state = SP_ALIVE;
	sp.death_ms = 0;
	sprite_transform(cub, &sp, &s);
	if (s.depth < SP_NEAR)
		return ;
	full = (int)fabs(WIN_H / s.depth);
	proj_rect(cub, &s, (int)(full * PROJ_SCALE));
	draw_sprite_cols(cub, &s);
}

/**
 * @brief Draws every active projectile for the frame. Runs after draw_sprites
 * so the wall z-buffer is populated for correct occlusion.
 */
void	draw_projectiles(t_cub *cub)
{
	int	i;

	i = -1;
	while (++i < MAX_PROJ)
	{
		if (cub->projectiles[i].active)
			draw_one_proj(cub, &cub->projectiles[i]);
	}
}
