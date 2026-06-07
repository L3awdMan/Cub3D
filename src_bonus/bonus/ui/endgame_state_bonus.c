/* SECTION 4 (src_bonus/bonus/endgame_state_bonus.c): end-game state machine +
 * overlay draw. update_endgame() flips PLAYING->DEAD when hp hits 0 and reads
 * the confirm key to respawn (dead) or advance_floor (win). draw_endgame()
 * lazily loads the UI then blits the current dead frame (blur->clear) or the
 * win frame full-screen with OPAQUE_KEY so nothing shows through. */

#include "cub3d_bonus.h"

/**
 * @brief Restores the floor to its captured spawn state: player camera, the
 * initial sprite set, full hp, reseeded lives; clears projectiles, flashes and
 * held keys; returns to GS_PLAYING.
 */
void	respawn(t_cub *cub)
{
	int	i;

	cub->player = cub->spawn_player;
	if (cub->spawn_sprites && cub->spawn_count > 0)
	{
		ft_memcpy(cub->sprites, cub->spawn_sprites,
			cub->spawn_count * sizeof(t_sprite));
		cub->sprite_count = cub->spawn_count;
	}
	i = -1;
	while (++i < MAX_PROJ)
		cub->projectiles[i].active = 0;
	cub->player.hp = PLAYER_HP_MAX;
	cub->lives = count_enemies(cub);
	cub->flash_ms = 0;
	cub->player.hurt_ms = 0;
	ft_bzero(cub->keys, sizeof(cub->keys));
	cub->game_state = GS_PLAYING;
}

/**
 * @brief Enters end-game state gs and stamps state_ms (shared with the win
 * trigger in reward_effect).
 */
void	enter_state(t_cub *cub, int gs)
{
	cub->game_state = gs;
	cub->state_ms = now_ms();
}

/**
 * @brief 1 when Enter/Space is held AND the dead blur anim has finished, so a
 * space held from firing cannot skip the screen before it clears.
 */
static int	confirm_pressed(t_cub *cub)
{
	if (now_ms() - cub->state_ms < (long)DEAD_FRAMES * DEAD_ANIM_MS)
		return (0);
	return (cub->keys[KEY_ENTER] || cub->keys[KEY_SPACE]);
}

/**
 * @brief Per-frame end-game logic: detect death, then poll the confirm key to
 * respawn (dead) or advance to the next floor (win).
 */
void	update_endgame(t_cub *cub)
{
	if (cub->game_state == GS_PLAYING)
	{
		if (cub->player.hp <= 0)
			enter_state(cub, GS_DEAD);
	}
	else if (cub->game_state == GS_DEAD)
	{
		if (confirm_pressed(cub))
			respawn(cub);
	}
	else if (cub->game_state == GS_WIN)
	{
		if (cub->cutscene.final_done)
			return ;
		if (confirm_pressed(cub))
			advance_floor(cub);
	}
}

/**
 * @brief Draws the full-screen end-game overlay: the dead frame indexed by
 * elapsed time (clamped clear) or the win frame. Lazily loads the UI on the
 * first end-game frame. No-op while playing.
 */
void	draw_endgame(t_cub *cub)
{
	t_img	*f;
	int		idx;

	if (cub->game_state == GS_PLAYING)
		return ;
	if (!cub->dead_ui[0].id)
		load_endgame_ui(cub);
	idx = (int)((now_ms() - cub->state_ms) / DEAD_ANIM_MS);
	if (cub->game_state == GS_DEAD)
	{
		if (idx > DEAD_FRAMES - 1)
			idx = DEAD_FRAMES - 1;
		f = &cub->dead_ui[idx];
	}
	else if (cub->cutscene.final_done)
		f = &cub->gameover_ui[idx % GAMEOVER_FRAMES];
	else
		f = &cub->win_ui;
	if (f->id)
		blit_full(cub, f, OPAQUE_KEY);
}
