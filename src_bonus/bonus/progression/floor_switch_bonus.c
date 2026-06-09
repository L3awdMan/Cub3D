/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   floor_switch_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zotaj-di <zotaj-di@learner.42.tech>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 00:08:01 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/06/10 00:08:03 by zotaj-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* SECTION 4 (src_bonus/bonus/floor_switch_bonus.c): floor progression. On a
 * floor-2 win, advance_floor() starts the ending cutscene; the cutscene then
 * calls advance_floor_after_cutscene() to tear down/reload floor 3. The floor-3
 * ending is driven from reward_popup_bonus.c (money bag -> CUT_END_F3 ->
 * terminal GS_WIN game_over loop), so advance_floor never handles floor 3. */

#include "cub3d_bonus.h"

/**
 * @brief Maps the current floor to the next floor's map path, or NULL when the
 * current floor is the last (no progression).
 */
static char	*next_floor_map(int floor)
{
	if (floor == 1)
		return ("maps/blake_stone_floor2.cub");
	if (floor == 2)
		return ("maps/blake_stone_floor3.cub");
	return (NULL);
}

/**
 * @brief Frees the current floor's parsed heap (grid + row_len, texture paths,
 * doors, sprites) and resets the map flags/counts so parse_file starts clean.
 */
void	reset_map_state(t_cub *cub)
{
	free_grid(&cub->map);
	free_tex_paths(&cub->map);
	if (cub->doors)
	{
		free(cub->doors);
		cub->doors = NULL;
	}
	if (cub->sprites)
	{
		free(cub->sprites);
		cub->sprites = NULL;
	}
	cub->map.parsed_flags = 0;
	cub->map.height = 0;
	cub->map.width = 0;
	cub->sprite_count = 0;
	cub->sprite_cap = 0;
}

/**
 * @brief Re-runs the per-floor init chain after parse_file: row lengths, doors,
 * sprites, spawn snapshot, world + anim textures; then resets player hp,
 * projectiles, flash and returns to GS_PLAYING. (floor/lives HUDs reload lazily
 * via loop_hook since free_textures nulled their ids.)
 */
void	reinit_floor(t_cub *cub)
{
	int	i;

	init_row_len(cub);
	init_doors(cub);
	init_sprites(cub);
	capture_spawn(cub);
	load_textures(cub);
	load_anim_sprites(cub);
	i = -1;
	while (++i < MAX_PROJ)
		cub->projectiles[i].active = 0;
	cub->player.hp = PLAYER_HP_MAX;
	cub->flash_ms = 0;
	cub->player.hurt_ms = 0;
	cub->game_state = GS_PLAYING;
}

/**
 * @brief Loads the next floor after an ending cutscene, or directly for floors
 * without an ending cutscene. Keeps mlx/window/weapon images intact.
 */
void	advance_floor_after_cutscene(t_cub *cub)
{
	char	*path;

	path = next_floor_map(cub->floor);
	if (!path)
		return ;
	free_textures(cub);
	free_anim_sprites(cub);
	reset_map_state(cub);
	parse_file(cub, path);
	reinit_floor(cub);
	schedule_mission_ui(cub);
}

/**
 * @brief Confirms a YOU WIN screen: floor 1 loads floor 2 directly, floor 2
 * plays its ending cutscene before floor 3. (Floor 3 never reaches here.)
 */
void	advance_floor(t_cub *cub)
{
	if (cub->floor == 2)
		start_cutscene(cub, CUT_END_F2, CUT_ACT_NEXT);
	else
		advance_floor_after_cutscene(cub);
}
