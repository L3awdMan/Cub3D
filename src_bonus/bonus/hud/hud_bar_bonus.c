/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hud_bar_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zotaj-di <zotaj-di@learner.42.tech>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 00:07:35 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/06/10 00:07:35 by zotaj-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* SECTION 4 (src_bonus/bonus/hud_bar_bonus.c): HUD health bars. Drawn after
 * the weapon and before the crosshair. The player bar (bottom-left) always
 * shows and drains as enemies chip cub->player.hp (clamped at 0, no death).
 * The boss bar (top-center, just below the floor HUD) appears only while a live
 * SP_FLUID boss exists and drains over its BOSS_HP shots. */

#include "cub3d_bonus.h"

static int	player_in_boss_room(t_cub *cub)
{
	int		mx;
	int		my;

	mx = (int)cub->player.pos_x;
	my = (int)cub->player.pos_y;
	if (my < 0 || my >= cub->map.height || mx < 0)
		return (0);
	if (mx >= (int)ft_strlen(cub->map.grid[my]))
		return (0);
	return (cub->map.grid[my][mx] == 'Z');
}

static int	hud_bottom_y(t_cub *cub)
{
	if (!cub->floor_hud.id)
		return (22);
	return (WIN_W * cub->floor_hud.height / cub->floor_hud.width + 8);
}

static void	draw_boss_bar(t_cub *cub)
{
	int		bbox[4];
	double	ratio;
	int		i;

	if (!cub->sprites || cub->sprite_count <= 0)
		return ;
	if (!player_in_boss_room(cub))
		return ;
	i = -1;
	while (++i < cub->sprite_count)
	{
		if (cub->sprites[i].type == SP_FLUID
			&& cub->sprites[i].state != SP_DYING)
		{
			ratio = (double)cub->sprites[i].hp / BOSS_HP;
			bbox[0] = WIN_W / 2 - 240;
			bbox[1] = hud_bottom_y(cub);
			bbox[2] = 480;
			bbox[3] = 26;
			draw_hud_bar(cub, bbox, ratio, 1);
			return ;
		}
	}
}

/**
 * @brief Draws the player health bar (always) and the boss bar (when a live
 * boss is on the map) over the 3D view.
 */
void	draw_health_bars(t_cub *cub)
{
	int	pbox[4];

	pbox[0] = 20;
	pbox[1] = WIN_H - 44;
	pbox[2] = 260;
	pbox[3] = 24;
	draw_hud_bar(cub, pbox, (double)cub->player.hp / PLAYER_HP_MAX, 0);
	draw_boss_bar(cub);
}
