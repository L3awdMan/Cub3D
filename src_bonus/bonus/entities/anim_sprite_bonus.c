/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   anim_sprite_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zotaj-di <zotaj-di@learner.42.tech>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 00:07:09 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/06/10 00:07:09 by zotaj-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* SECTION 4 (src_bonus/bonus/anim_sprite_bonus.c): loads every animated sprite
 * type into cub->sprite_anims and frees them on shutdown. The enemy/boss art
 * is per-floor: floor_set(cub->floor) names the walk, firing and death XPM
 * sequences for each of the ENEMY_ROLES roles, which fill slots role,
 * firing_type(role) and death_type(role). The four weapon pickups load as
 * single-frame types at SP_PICKUP+i. now_ms() exposes a wall-clock timestamp
 * shared by the sprite-frame picker and the door slide animation. */

#include "cub3d_bonus.h"
#include "mlx.h"

/* SECTION 4: weapon pickups. g_ammo[i] is the floor sprite for PICKUP_CHARS[i]
 * (weapon i + 1), loaded as a single-frame type at SP_PICKUP + i. */
static const char *const	g_ammo[] = {
	"./textures/blake_stone_xpm/sprites/guns/gun_2.xpm",
	"./textures/blake_stone_xpm/sprites/guns/gun_3.xpm",
	"./textures/blake_stone_xpm/sprites/guns/gun_4.xpm",
	"./textures/blake_stone_xpm/sprites/guns/gun_5.xpm",
	NULL,
};

/* SECTION 4: world reward pickups. g_rewards[0] loads at SP_MEAT, g_rewards[1]
 * at SP_MONEY (single-frame grounded sprites dropped by enemy/boss death). */
static const char *const	g_rewards[] = {
	"./textures/blake_stone_xpm/sprites/health_money/health_meat.xpm",
	"./textures/blake_stone_xpm/sprites/health_money/money_bag.xpm",
	NULL,
};

/**
 * @brief Returns the current wall-clock time in milliseconds (gettimeofday).
 * Used as the animation clock for sprites + doors.
 */
long	now_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000L + tv.tv_usec / 1000L);
}

/**
 * @brief Loads one sprite frame XPM into the given t_img and records its
 * transparent-pixel color by sampling the frame's (0, 0) — which the XPM
 * convention places in the see-through border around every sprite atlas.
 */
static void	load_one_frame(t_cub *cub, t_anim *a, int i, const char *path)
{
	t_img	*t;

	t = &a->frames[i];
	t->id = mlx_xpm_file_to_image(cub->mlx, (char *)path,
			&t->width, &t->height);
	if (!t->id)
		exit_error(cub, "Failed to load sprite frame");
	t->data = mlx_get_data_addr(t->id, &t->bpp, &t->line_len, &t->endian);
	a->transp[i] = *(unsigned int *)t->data;
}

/**
 * @brief Loads one animated sprite type from a NULL-terminated path table.
 */
static void	load_one_type(t_cub *cub, t_anim *a, const char *const *paths)
{
	int	i;

	i = 0;
	while (i < MAX_FRAMES && paths[i])
	{
		load_one_frame(cub, a, i, paths[i]);
		i++;
	}
	a->count = i;
}

/**
 * @brief Loads every animated sprite type. Per role: walk loop, death set and
 * firing set from the floor's art table; then the weapon pickups (single
 * frame each) at SP_PICKUP and the reward pickups at SP_MEAT/SP_MONEY.
 */
void	load_anim_sprites(t_cub *cub)
{
	const t_floorset	*fs;
	int					r;
	int					i;

	fs = floor_set(cub->floor);
	r = -1;
	while (++r < ENEMY_ROLES)
	{
		load_one_type(cub, &cub->sprite_anims[r], fs->walk[r]);
		load_one_type(cub, &cub->sprite_anims[death_type(r)], fs->death[r]);
		load_one_type(cub, &cub->sprite_anims[firing_type(r)], fs->fire[r]);
		load_one_type(cub, &cub->sprite_anims[proj_type(r)], fs->proj[r]);
	}
	i = -1;
	while (g_ammo[++i])
	{
		load_one_frame(cub, &cub->sprite_anims[SP_PICKUP + i], 0, g_ammo[i]);
		cub->sprite_anims[SP_PICKUP + i].count = 1;
	}
	i = -1;
	while (g_rewards[++i])
	{
		load_one_frame(cub, &cub->sprite_anims[SP_MEAT + i], 0, g_rewards[i]);
		cub->sprite_anims[SP_MEAT + i].count = 1;
	}
}

/**
 * @brief Destroys every loaded animated sprite frame.
 */
void	free_anim_sprites(t_cub *cub)
{
	int	t;
	int	i;

	t = 0;
	while (t < SP_TYPES)
	{
		i = 0;
		while (i < cub->sprite_anims[t].count)
		{
			if (cub->sprite_anims[t].frames[i].id && cub->mlx)
				mlx_destroy_image(cub->mlx,
					cub->sprite_anims[t].frames[i].id);
			cub->sprite_anims[t].frames[i].id = NULL;
			i++;
		}
		cub->sprite_anims[t].count = 0;
		t++;
	}
}
