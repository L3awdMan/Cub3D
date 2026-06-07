/* SECTION 4 (src_bonus/bonus/menu_frame_bonus.c): menu screen selection + the
 * Start-Mission fade. menu_frame() maps the current GS_* state to the image to
 * blit; on GS_FADEOUT it indexes the clear->black sequence by elapsed time.
 * update_menu() drives that timer and, on the black frame, starts the floor-1
 * intro cutscene (which itself transitions to GS_PLAYING). */

#include "cub3d_bonus.h"

/**
 * @brief Returns the full-screen image for the current menu state. On
 * GS_FADEOUT the frame is picked from elapsed time (clamped on the black frame).
 */
t_img	*menu_frame(t_cub *cub)
{
	int	idx;

	if (cub->game_state == GS_MENU)
		return (&cub->ui_menu[cub->menu_sel]);
	if (cub->game_state == GS_SETTINGS)
		return (&cub->ui_settings);
	if (cub->game_state == GS_DIFFICULTY)
		return (&cub->ui_diff[cub->difficulty]);
	if (cub->game_state == GS_FADEOUT)
	{
		idx = (int)((now_ms() - cub->state_ms) / FADE_ANIM_MS);
		if (idx > FADE_FRAMES - 1)
			idx = FADE_FRAMES - 1;
		return (&cub->ui_fade[idx]);
	}
	return (&cub->ui_intro);
}

/**
 * @brief Begins the run: plays the floor-1 intro cutscene, which flips to
 * GS_PLAYING when it finishes.
 */
static void	start_mission(t_cub *cub)
{
	start_cutscene(cub, CUT_BEGIN_F1, CUT_ACT_PLAY);
}

/**
 * @brief Per-frame menu update. Only acts on GS_FADEOUT: once the clear->black
 * sequence has fully played, hands off to the floor-1 cutscene. No-op on the
 * static menu screens.
 */
void	update_menu(t_cub *cub)
{
	if (cub->game_state != GS_FADEOUT)
		return ;
	if (now_ms() - cub->state_ms >= (long)FADE_FRAMES * FADE_ANIM_MS)
		start_mission(cub);
}
