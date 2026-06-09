/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_free_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zotaj-di <zotaj-di@learner.42.tech>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 00:08:23 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/06/10 00:08:23 by zotaj-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* SECTION 4 (src_bonus/bonus/texture_free_bonus.c): texture teardown. Split
 * out of texture.c so the loader file stays within the function-per-file
 * limit. free_world_tex() drops the four wall textures and the bonus wall
 * alternates; free_textures() also drops the door, floor and ceiling images. */

#include "cub3d_bonus.h"
#include "mlx.h"

/**
 * @brief Destroys the four wall textures and the bonus wall-alt textures.
 */
static void	free_world_tex(t_cub *cub)
{
	int	i;

	i = 0;
	while (i < TEX_COUNT)
	{
		if (cub->tex[i].id && cub->mlx)
			mlx_destroy_image(cub->mlx, cub->tex[i].id);
		cub->tex[i].id = NULL;
		i++;
	}
	i = 0;
	while (i < WALL_ALT_COUNT)
	{
		if (cub->wall_alt[i].id && cub->mlx)
			mlx_destroy_image(cub->mlx, cub->wall_alt[i].id);
		cub->wall_alt[i].id = NULL;
		i++;
	}
}

/**
 * @brief Destroys the 24 lazily-loaded lives-counter frames.
 */
static void	free_lives_hud(t_cub *cub)
{
	int	i;

	i = -1;
	while (++i < 24)
	{
		if (cub->lives_hud[i].id && cub->mlx)
			mlx_destroy_image(cub->mlx, cub->lives_hud[i].id);
		cub->lives_hud[i].id = NULL;
	}
}

/**
 * @brief Destroys the lazily-loaded end-game UI (6 dead frames + win frame).
 */
static void	free_endgame(t_cub *cub)
{
	int	i;

	i = -1;
	while (++i < DEAD_FRAMES)
	{
		if (cub->dead_ui[i].id && cub->mlx)
			mlx_destroy_image(cub->mlx, cub->dead_ui[i].id);
		cub->dead_ui[i].id = NULL;
	}
	if (cub->win_ui.id && cub->mlx)
		mlx_destroy_image(cub->mlx, cub->win_ui.id);
	cub->win_ui.id = NULL;
	if (cub->gameover_retry.id && cub->mlx)
		mlx_destroy_image(cub->mlx, cub->gameover_retry.id);
	cub->gameover_retry.id = NULL;
	if (cub->gameover_quit.id && cub->mlx)
		mlx_destroy_image(cub->mlx, cub->gameover_quit.id);
	cub->gameover_quit.id = NULL;
}

/**
 * @brief Destroys all loaded texture imgs (walls + bonus door / floor / ceil).
 */
void	free_textures(t_cub *cub)
{
	free_world_tex(cub);
	if (cub->door_tex.id && cub->mlx)
		mlx_destroy_image(cub->mlx, cub->door_tex.id);
	cub->door_tex.id = NULL;
	if (cub->door_open_tex.id && cub->mlx)
		mlx_destroy_image(cub->mlx, cub->door_open_tex.id);
	cub->door_open_tex.id = NULL;
	if (cub->floor_tex.id && cub->mlx)
		mlx_destroy_image(cub->mlx, cub->floor_tex.id);
	cub->floor_tex.id = NULL;
	if (cub->ceil_tex.id && cub->mlx)
		mlx_destroy_image(cub->mlx, cub->ceil_tex.id);
	cub->ceil_tex.id = NULL;
	if (cub->boss_ceil_tex.id && cub->mlx)
		mlx_destroy_image(cub->mlx, cub->boss_ceil_tex.id);
	cub->boss_ceil_tex.id = NULL;
	if (cub->floor_hud.id && cub->mlx)
		mlx_destroy_image(cub->mlx, cub->floor_hud.id);
	cub->floor_hud.id = NULL;
	free_lives_hud(cub);
	free_endgame(cub);
	free_cutscene(cub);
}
