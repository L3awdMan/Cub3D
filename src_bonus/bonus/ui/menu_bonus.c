/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   menu_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zotaj-di <zotaj-di@learner.42.tech>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 00:08:42 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/06/10 00:08:42 by zotaj-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* front-end UI — load / draw / free.
 * load_menu_ui() lazily loads the intro, the 3 main-menu states, the settings
 * screen, the 3 difficulty states and the 6 clear->black Start-Mission fade
 * frames on the first menu frame. draw_menu() blits the current screen
 * full-screen via blit_full with OPAQUE_KEY. State + input live in
 * menu_frame_bonus.c / menu_input_bonus.c. */

#include "cub3d_bonus.h"
#include "mlx.h"

/**
 * @brief Builds the malloc'd path UI_DIR <name>. Caller frees.
 */
static char	*ui_path(char *name)
{
	return (ft_strjoin(UI_DIR, name));
}

/**
 * @brief Loads one full-screen UI screen named <name> into f, exit_error on
 * failure. Frees the built path.
 */
static void	load_one(t_cub *cub, t_img *f, char *name)
{
	char	*path;

	path = ui_path(name);
	if (!path)
		exit_error(cub, "Memory allocation failed");
	f->id = mlx_xpm_file_to_image(cub->mlx, path, &f->width, &f->height);
	free(path);
	if (!f->id)
		exit_error(cub, "Failed to load menu UI texture");
	f->data = mlx_get_data_addr(f->id, &f->bpp, &f->line_len, &f->endian);
}

/**
 * @brief Lazily loads every menu screen + the fade sequence (one-time, on the
 * first menu frame).
 */
void	load_menu_ui(t_cub *cub)
{
	int		i;
	char	name[17];

	ft_strlcpy(name, "intro_fade_0.xpm", sizeof(name));
	load_one(cub, &cub->ui_intro, "intro.xpm");
	load_one(cub, &cub->ui_menu[0], "menu_start.xpm");
	load_one(cub, &cub->ui_menu[1], "menu_settings.xpm");
	load_one(cub, &cub->ui_menu[2], "menu_difficulty.xpm");
	load_one(cub, &cub->ui_settings, "settings.xpm");
	load_one(cub, &cub->ui_diff[0], "diff_easy.xpm");
	load_one(cub, &cub->ui_diff[1], "diff_skilled.xpm");
	load_one(cub, &cub->ui_diff[2], "diff_gigachad.xpm");
	load_one(cub, &cub->ui_pause[0], "pause_resume.xpm");
	load_one(cub, &cub->ui_pause[1], "pause_settings.xpm");
	load_one(cub, &cub->ui_pause[2], "pause_quit.xpm");
	i = -1;
	while (++i < FADE_FRAMES)
	{
		name[11] = '0' + i;
		load_one(cub, &cub->ui_fade[i], name);
	}
}

/**
 * @brief Draws the current menu screen full-screen, lazily loading the UI on
 * the first frame.
 */
void	draw_menu(t_cub *cub)
{
	if (!cub->ui_intro.id)
		load_menu_ui(cub);
	blit_full(cub, menu_frame(cub), OPAQUE_KEY);
}

/**
 * @brief Destroys every menu + fade image (guards id && mlx, nulls handles).
 */
void	free_menu_ui(t_cub *cub)
{
	int		i;
	t_img	*all[6];

	all[0] = &cub->ui_intro;
	all[1] = &cub->ui_settings;
	i = -1;
	while (++i < MENU_OPTS)
	{
		if (cub->ui_menu[i].id && cub->mlx)
			mlx_destroy_image(cub->mlx, cub->ui_menu[i].id);
		if (cub->ui_diff[i].id && cub->mlx)
			mlx_destroy_image(cub->mlx, cub->ui_diff[i].id);
	}
	i = -1;
	while (++i < PAUSE_OPTS)
		if (cub->ui_pause[i].id && cub->mlx)
			mlx_destroy_image(cub->mlx, cub->ui_pause[i].id);
	i = -1;
	while (++i < FADE_FRAMES)
		if (cub->ui_fade[i].id && cub->mlx)
			mlx_destroy_image(cub->mlx, cub->ui_fade[i].id);
	i = -1;
	while (++i < 2)
		if (all[i]->id && cub->mlx)
			mlx_destroy_image(cub->mlx, all[i]->id);
}
