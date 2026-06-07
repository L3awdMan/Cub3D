/* SECTION 4 (src_bonus/bonus/cutscene_draw_bonus.c): cutscene update/draw.
 * While GS_CUTSCENE is active the world is frozen and render_frame draws only
 * the current frame scaled to the full window. */

#include "cub3d_bonus.h"

static void	finish_cutscene(t_cub *cub)
{
	int	action;

	action = cub->cutscene.next_action;
	free_cutscene(cub);
	ft_bzero(cub->keys, sizeof(cub->keys));
	if (action == CUT_ACT_NEXT)
		advance_floor_after_cutscene(cub);
	else if (action == CUT_ACT_FINAL)
	{
		cub->cutscene.final_done = 1;
		enter_state(cub, GS_WIN);
	}
	else
		cub->game_state = GS_PLAYING;
}

void	update_cutscene(t_cub *cub)
{
	long	elapsed;
	int		frame;

	if (cub->game_state != GS_CUTSCENE)
		return ;
	elapsed = now_ms() - cub->cutscene.start_ms;
	frame = (int)(elapsed / cub->cutscene.frame_ms);
	if (frame >= cub->cutscene.count)
	{
		finish_cutscene(cub);
		return ;
	}
	cub->cutscene.frame = frame;
}

void	draw_cutscene(t_cub *cub)
{
	t_img	*f;

	if (cub->game_state != GS_CUTSCENE || cub->cutscene.count <= 0)
		return ;
	f = &cub->cutscene.frames[cub->cutscene.frame];
	if (f->id)
		blit_full(cub, f, OPAQUE_KEY);
}
