/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite_draw_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zotaj-di <zotaj-di@learner.42.tech>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 00:07:25 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/06/10 00:07:25 by zotaj-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* sprite pass.
 * Each frame: pick the current animation frame for every sprite from the
 * wall-clock (now_ms), sort sprites far-to-near, project each into camera
 * space, and hand the visible ones to the column blitter. Runs after the
 * wall pass so cub->zbuf is populated for correct wall/sprite occlusion. */

#include "cub3d_bonus.h"

/**
 * @brief Picks the current animation frame for a sprite. An SP_ALIVE sprite
 * loops its walk set on the shared (now_ms / ANIM_MS) clock. An SP_DYING
 * sprite plays its death set once at DEATH_MS per frame and clamps on the
 * last frame unless the sprite type is removed by expire_dead_boss().
 */
static t_img	*current_frame(t_cub *cub, t_sprite *sp, unsigned int *transp)
{
	t_anim	*a;
	int		idx;

	if (sp->state == SP_DYING)
		a = &cub->sprite_anims[death_type(sp->type)];
	else if (sp->state == SP_ATTACKING)
		a = &cub->sprite_anims[firing_type(sp->type)];
	else
		a = &cub->sprite_anims[sp->type];
	if (a->count <= 0)
		return (NULL);
	if (sp->state == SP_DYING)
	{
		idx = (int)((now_ms() - sp->death_ms) / DEATH_MS);
		if (idx > a->count - 1)
			idx = a->count - 1;
	}
	else if (sp->state == SP_ATTACKING)
		idx = (int)((now_ms() - sp->death_ms) / DEATH_MS) % a->count;
	else
		idx = (int)((now_ms() / ANIM_MS) % a->count);
	*transp = a->transp[idx];
	return (&a->frames[idx]);
}

/**
 * @brief Removes the boss 3 seconds after death and drops a money-bag reward
 * pickup at its spot. The swap-shrink frees a slot that spawn_reward reclaims,
 * so the sprite count is unchanged and stays within sprite_cap.
 */
static int	expire_dead_boss(t_cub *cub, int i)
{
	long	elapsed;
	double	bx;
	double	by;

	if (!BOSS_VANISH_AFTER_DEATH || cub->sprites[i].type != SP_FLUID
		|| cub->sprites[i].state != SP_DYING)
		return (0);
	elapsed = now_ms() - cub->sprites[i].death_ms;
	if (elapsed < BOSS_VANISH_AFTER_DEATH)
		return (0);
	bx = cub->sprites[i].x;
	by = cub->sprites[i].y;
	cub->sprites[i] = cub->sprites[cub->sprite_count - 1];
	cub->sprite_count--;
	spawn_reward(cub, bx, by, SP_MONEY);
	return (1);
}

/**
 * @brief Projects a world-space sprite into screen-space draw parameters
 * using the inverse camera matrix. Leaves s->size 0 when the camera matrix
 * is degenerate (sprite behind the camera or singular plane).
 */
void	sprite_transform(t_cub *cub, t_sprite *sp, t_spr *s)
{
	t_player	*p;
	double		rx;
	double		ry;
	double		inv;
	double		tx;

	p = &cub->player;
	rx = sp->x - p->pos_x;
	ry = sp->y - p->pos_y;
	inv = p->plane_x * p->dir_y - p->dir_x * p->plane_y;
	if (inv > -1e-9 && inv < 1e-9)
	{
		s->depth = 0.0;
		return ;
	}
	inv = 1.0 / inv;
	tx = inv * (p->dir_y * rx - p->dir_x * ry);
	s->depth = inv * (-p->plane_y * rx + p->plane_x * ry);
	if (s->depth < SP_NEAR)
		return ;
	s->screen_x = (int)((WIN_W / 2.0) * (1.0 + tx / s->depth));
	s->shade = FOG_K / (FOG_K + s->depth);
	s->tex = current_frame(cub, sp, &s->transp);
	sprite_rect(cub, sp, s, (int)fabs(WIN_H / s->depth));
}

/**
 * @brief Draws every sprite for the frame: sort, then transform + blit each
 * one that sits in front of the camera.
 */
void	draw_sprites(t_cub *cub)
{
	int		i;
	t_spr	s;

	if (!cub->sprites || cub->sprite_count <= 0)
		return ;
	sort_sprites(cub);
	i = 0;
	while (i < cub->sprite_count)
	{
		if (expire_dead_boss(cub, i))
			continue ;
		sprite_transform(cub, &cub->sprites[i], &s);
		if (s.depth >= SP_NEAR)
			draw_sprite_cols(cub, &s);
		i++;
	}
}
