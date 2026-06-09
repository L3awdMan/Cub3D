/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   menu_input_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zotaj-di <zotaj-di@learner.42.tech>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 00:08:48 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/06/10 00:08:48 by zotaj-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* SECTION 4 (src_bonus/bonus/menu_input_bonus.c): front-end input dispatch.
 * menu_input() is called on each edge-detected key press while a menu state is
 * active: Space/Enter advance, Up/Down (or W/S) move a cursor, Backspace steps
 * back. ESC (always-quit) stays handled in key_press. Difficulty damage scaling
 * lives in difficulty_bonus.c. */

#include "cub3d_bonus.h"

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
	{
		cub->settings_return_state = GS_MENU;
		cub->game_state = GS_SETTINGS;
	}
	else
		cub->game_state = GS_DIFFICULTY;
}

static void	pause_select(t_cub *cub)
{
	if (cub->pause_sel == 0)
		cub->game_state = GS_PLAYING;
	else if (cub->pause_sel == 1)
	{
		cub->settings_return_state = GS_PAUSE;
		cub->game_state = GS_SETTINGS;
	}
	else
		close_hook(cub);
}

/**
 * @brief Handles key presses on the pause + settings screens. Returns 1 if it
 * consumed the key (caller stops), 0 otherwise.
 */
static int	pause_menu_input(t_cub *cub, int key, int confirm)
{
	if (cub->game_state == GS_PAUSE)
	{
		menu_nav(key, &cub->pause_sel, PAUSE_OPTS);
		if (confirm)
			pause_select(cub);
		return (1);
	}
	if (cub->game_state == GS_SETTINGS && key == KEY_BACKSPACE)
	{
		if (cub->settings_return_state == GS_PAUSE)
			cub->game_state = GS_PAUSE;
		else
			cub->game_state = GS_MENU;
		return (1);
	}
	return (0);
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
	else
		pause_menu_input(cub, key, confirm);
}
