/* SECTION 4 (src_bonus/bonus/menu_input_bonus.c): front-end input + difficulty.
 * menu_input() is called on each edge-detected key press while a menu state is
 * active: Space/Enter advance, Up/Down (or W/S) move a cursor, Backspace steps
 * back. player_dmg() scales enemy damage by the chosen difficulty so harder
 * levels drain hp faster. ESC (always-quit) stays handled in key_press. */

#include "cub3d_bonus.h"

static const double	g_diff_mult[3] = {1.0, 1.5, 2.0};

/**
 * @brief Enemy damage scaled by the selected difficulty (Easy 1x, Skilled 1.5x,
 * Gigachad 2x) so harder levels lose hp faster. Used at both player-damage
 * sites (enemy melee + projectile impact).
 */
int	player_dmg(t_cub *cub)
{
	return ((int)(ATTACK_DMG * g_diff_mult[cub->difficulty]));
}

/**
 * @brief Moves a wrapping cursor in [0, n) by the pressed key: Up/W back,
 * Down/S forward.
 */
static void	menu_nav(int key, int *sel, int n)
{
	if (key == KEY_UP || key == KEY_W)
		*sel = (*sel + n - 1) % n;
	else if (key == KEY_DOWN || key == KEY_S)
		*sel = (*sel + 1) % n;
}

/**
 * @brief Acts on the highlighted main-menu row: Start Mission begins the fade,
 * Game Settings / Difficulty open their screens.
 */
static void	menu_select(t_cub *cub)
{
	if (cub->menu_sel == 0)
		enter_state(cub, GS_FADEOUT);
	else if (cub->menu_sel == 1)
		cub->game_state = GS_SETTINGS;
	else
		cub->game_state = GS_DIFFICULTY;
}

/**
 * @brief Dispatches one menu key press by the current state. GS_FADEOUT ignores
 * input (it auto-advances).
 */
void	menu_input(t_cub *cub, int key)
{
	int	confirm;

	confirm = (key == KEY_ENTER || key == KEY_SPACE);
	if (cub->game_state == GS_INTRO && confirm)
		cub->game_state = GS_MENU;
	else if (cub->game_state == GS_MENU)
	{
		menu_nav(key, &cub->menu_sel, MENU_OPTS);
		if (confirm)
			menu_select(cub);
		else if (key == KEY_BACKSPACE)
			cub->game_state = GS_INTRO;
	}
	else if (cub->game_state == GS_DIFFICULTY)
	{
		menu_nav(key, &cub->difficulty, 3);
		if (confirm || key == KEY_BACKSPACE)
			cub->game_state = GS_MENU;
	}
	else if (cub->game_state == GS_SETTINGS && key == KEY_BACKSPACE)
		cub->game_state = GS_MENU;
}
