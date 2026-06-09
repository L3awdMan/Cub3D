/* SECTION 4 (src_bonus/bonus/ui/endgame_input_bonus.c): game-over screen input.
 * Split out of endgame_state_bonus.c to keep both files Norm-compliant. The
 * final game-over screen offers a Retry/Quit selection: W/S/Up/Down toggle it,
 * Enter confirms (restart from floor 1 or quit). */

#include "cub3d_bonus.h"

/**
 * @brief W/S or Up/Down toggle the game-over Retry/Quit selection.
 */
static void	toggle_gameover(t_cub *cub, int key)
{
	if (key == KEY_UP || key == KEY_DOWN || key == KEY_W || key == KEY_S)
		cub->gameover_sel = 1 - cub->gameover_sel;
}

/**
 * @brief Routes a key press on the final game-over screen: toggles the
 * selection, and on Enter either restarts from floor 1 (Retry) or quits (Quit).
 * No-op unless the final game-over screen is showing.
 */
void	endgame_input(t_cub *cub, int key)
{
	if (cub->game_state != GS_WIN || !cub->cutscene.final_done)
		return ;
	toggle_gameover(cub, key);
	if (key == KEY_ENTER)
	{
		if (cub->gameover_sel == GAMEOVER_RETRY)
			restart_game(cub);
		else
			close_hook(cub);
	}
}
