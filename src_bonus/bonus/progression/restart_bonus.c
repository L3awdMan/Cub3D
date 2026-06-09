/* SECTION 4 (src_bonus/bonus/progression/restart_bonus.c): game retry flow.
 * Retry returns to floor 1, restores the floor start state, then replays the
 * floor-1 opening cutscene before the mission briefing is scheduled. */

#include "cub3d_bonus.h"

void	reset_map_state(t_cub *cub);
void	reinit_floor(t_cub *cub);

void	restart_game(t_cub *cub)
{
	free_textures(cub);
	free_anim_sprites(cub);
	reset_map_state(cub);
	cub->floor = 1;
	cub->cutscene.final_done = 0;
	cub->cutscene.next_action = 0;
	cub->gameover_sel = GAMEOVER_RETRY;
	parse_file(cub, "maps/blake_stone_floor1.cub");
	reinit_floor(cub);
	start_cutscene(cub, CUT_BEGIN_F1, CUT_ACT_PLAY);
}
