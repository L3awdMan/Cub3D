/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   theme_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zotaj-di <zotaj-di@learner.42.tech>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 00:08:55 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/06/10 00:08:55 by zotaj-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* per-floor texture theme. Only the
 * NO/SO/WE/EA wall faces are read from the .cub; the door, floor, ceiling,
 * boss-ceiling and accent-wall (map chars 4/5/6) textures are otherwise fixed
 * global paths. floor_theme() returns the set for the current floor so a floor
 * can fully re-skin those surfaces. Floors 1 and 2 resolve to default_theme()
 * (the original macro paths) so they look exactly as before; floor 3 gets its
 * own free Blake-Stone tiles. load_textures() consumes the returned t_theme. */

#include "cub3d_bonus.h"

#define F3_WALLS	"./textures/blake_stone_xpm/walls/"
#define F3_FC		"./textures/blake_stone_xpm/floors_ceilings/"

/**
 * @brief The original (floors 1 & 2) theme: the hardcoded macro paths.
 */
static t_theme	default_theme(void)
{
	t_theme	t;

	t.door = DOOR_FACE_PATH;
	t.door_open = DOOR_OPEN_FACE_PATH;
	t.floor = FLOOR_PATH;
	t.ceil = CEIL_PATH;
	t.boss_ceil = BOSS_CEIL_PATH;
	t.alt[0] = WALL_ALT0_PATH;
	t.alt[1] = WALL_ALT1_PATH;
	t.alt[2] = WALL_ALT2_PATH;
	return (t);
}

/**
 * @brief Floor 3 theme: free Blake-Stone tiles unused by floors 1 & 2, so the
 * doors, ceiling, floor and accent walls all look distinct on the final floor.
 */
static t_theme	floor3_theme(void)
{
	t_theme	t;

	t.door = F3_WALLS "wall_r15_c07.xpm";
	t.door_open = F3_WALLS "wall_r15_c07.xpm";
	t.floor = F3_FC "floor_r04_c00.xpm";
	t.ceil = F3_FC "floor_r03_c00.xpm";
	t.boss_ceil = F3_FC "floor_r05_c00.xpm";
	t.alt[0] = F3_WALLS "wall_r01_c04.xpm";
	t.alt[1] = F3_WALLS "wall_r01_c05.xpm";
	t.alt[2] = F3_WALLS "wall_r01_c06.xpm";
	return (t);
}

/**
 * @brief Returns the texture theme for the given floor. Floor 3 re-skins its
 * surfaces; every other value falls back to the original default theme.
 */
t_theme	floor_theme(int floor)
{
	if (floor == 3)
		return (floor3_theme());
	return (default_theme());
}
