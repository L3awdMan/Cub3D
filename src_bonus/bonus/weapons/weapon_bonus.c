/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   weapon_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zotaj-di <zotaj-di@learner.42.tech>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 00:09:04 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/06/10 00:09:04 by zotaj-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* SECTION 4 (src_bonus/bonus/weapon_bonus.c): weapon asset I/O and gameplay
 * hooks. Loads the five-frame player-view weapon XPM animations at startup
 * (transparency sampled from each (0, 0) corner, same convention as sprites).
 * weapon_pickup() swaps the active weapon when the player steps on a
 * PICKUP_CHARS tile; weapon_fire() is the left-click button hook that flags
 * a shot. */

#include "cub3d_bonus.h"
#include "mlx.h"

#define WPN_HUD_DIR	"./textures/blake_stone_xpm/sprites/weapon_hud/"

static const char *const	g_weapons[WPN_COUNT][WPN_FRAMES] = {
{
	WPN_HUD_DIR "auto_charge_pistol_1.xpm",
	WPN_HUD_DIR "auto_charge_pistol_2.xpm",
	WPN_HUD_DIR "auto_charge_pistol_3.xpm",
	WPN_HUD_DIR "auto_charge_pistol_4.xpm",
	WPN_HUD_DIR "auto_charge_pistol_5.xpm",
},
{
	WPN_HUD_DIR "slow_fire_protector_1.xpm",
	WPN_HUD_DIR "slow_fire_protector_2.xpm",
	WPN_HUD_DIR "slow_fire_protector_3.xpm",
	WPN_HUD_DIR "slow_fire_protector_4.xpm",
	WPN_HUD_DIR "slow_fire_protector_5.xpm",
},
{
	WPN_HUD_DIR "rapid_assault_weapon_1.xpm",
	WPN_HUD_DIR "rapid_assault_weapon_2.xpm",
	WPN_HUD_DIR "rapid_assault_weapon_3.xpm",
	WPN_HUD_DIR "rapid_assault_weapon_4.xpm",
	WPN_HUD_DIR "rapid_assault_weapon_5.xpm",
},
{
	WPN_HUD_DIR "dual_neutron_disruptor_1.xpm",
	WPN_HUD_DIR "dual_neutron_disruptor_2.xpm",
	WPN_HUD_DIR "dual_neutron_disruptor_3.xpm",
	WPN_HUD_DIR "dual_neutron_disruptor_4.xpm",
	WPN_HUD_DIR "dual_neutron_disruptor_5.xpm",
},
{
	WPN_HUD_DIR "plasma_discharge_unit_1.xpm",
	WPN_HUD_DIR "plasma_discharge_unit_2.xpm",
	WPN_HUD_DIR "plasma_discharge_unit_3.xpm",
	WPN_HUD_DIR "plasma_discharge_unit_4.xpm",
	WPN_HUD_DIR "plasma_discharge_unit_5.xpm",
},
};

/**
 * @brief Loads one weapon XPM into the given t_img (fixed paths, so
 * mirrors load_named_tex in texture.c rather than the .cub-driven loader).
 */
static void	wpn_load_one(t_cub *cub, t_img *t, const char *path)
{
	t->id = mlx_xpm_file_to_image(cub->mlx, (char *)path, &t->width,
			&t->height);
	if (!t->id)
		exit_error(cub, "Failed to load weapon texture");
	t->data = mlx_get_data_addr(t->id, &t->bpp, &t->line_len, &t->endian);
}

/**
 * @brief Loads every weapon image, records each None-pixel key, and arms the
 * player with weapon 0 in the idle pose.
 */
void	load_weapons(t_cub *cub)
{
	t_weapon	*w;
	int			i;
	int			j;

	w = &cub->weapon;
	i = -1;
	while (++i < WPN_COUNT)
	{
		j = -1;
		while (++j < WPN_FRAMES)
		{
			wpn_load_one(cub, &w->img[i][j], g_weapons[i][j]);
			w->transp[i][j] = *(unsigned int *)w->img[i][j].data;
		}
	}
	w->current = 0;
	w->shoot = 0;
	w->firing = 0;
	w->fire_ms = now_ms();
}

/**
 * @brief Destroys every loaded weapon image.
 */
void	free_weapons(t_cub *cub)
{
	int	i;
	int	j;

	i = -1;
	while (++i < WPN_COUNT)
	{
		j = -1;
		while (++j < WPN_FRAMES)
		{
			if (cub->weapon.img[i][j].id && cub->mlx)
				mlx_destroy_image(cub->mlx,
					cub->weapon.img[i][j].id);
			cub->weapon.img[i][j].id = NULL;
		}
	}
}

/**
 * @brief Swaps the active weapon when the player stands on a PICKUP_CHARS
 * tile ('7'..'P' = weapons 1..4), removes that tile's floor sprite, then
 * clears the tile to floor so the pickup is consumed once.
 */
void	weapon_pickup(t_cub *cub)
{
	int			mx;
	int			my;
	char		c;
	const char	*p;

	mx = (int)cub->player.pos_x;
	my = (int)cub->player.pos_y;
	c = cub->map.grid[my][mx];
	if (!c)
		return ;
	p = ft_strchr(PICKUP_CHARS, c);
	if (!p || !*p)
		return ;
	cub->weapon.current = (int)(p - PICKUP_CHARS) + 1;
	drop_pickup_sprite(cub, mx, my);
	cub->map.grid[my][mx] = '0';
}

/**
 * @brief ButtonPress hook (X event 4): a left-click (button 1) flags one
 * shot, consumed next frame by update_weapon(). Other buttons are ignored.
 * @return 0 per the MLX hook convention.
 */
int	weapon_fire(int button, int x, int y, void *param)
{
	t_cub	*cub;

	(void)x;
	(void)y;
	cub = (t_cub *)param;
	if (button == 1)
		cub->weapon.shoot = 1;
	return (0);
}
