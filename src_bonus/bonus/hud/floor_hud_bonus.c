/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   floor_hud_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zotaj-di <zotaj-di@learner.42.tech>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 00:07:32 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/06/10 00:07:32 by zotaj-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* SECTION 4 (src_bonus/bonus/floor_hud_bonus.c): per-floor HUD overlay. Each
 * FLOOR directive picks one top-screen cropped HUD XPM. It draws before the
 * weapon and health bars, so the new floor panel becomes the dashboard base
 * while existing gameplay HUD elements stay readable on top. */

#include "cub3d_bonus.h"
#include "mlx.h"

#define FLOOR_HUD_DIR "./textures/blake_stone_xpm/sprites/floor_hud/"

static char	*floor_hud_path(int floor)
{
	if (floor == 2)
		return (FLOOR_HUD_DIR "floor2_hud.xpm");
	if (floor == 3)
		return (FLOOR_HUD_DIR "floor3_hud.xpm");
	return (FLOOR_HUD_DIR "floor1_hud.xpm");
}

void	load_floor_hud(t_cub *cub)
{
	t_img	*h;

	h = &cub->floor_hud;
	h->id = mlx_xpm_file_to_image(cub->mlx, floor_hud_path(cub->floor),
			&h->width, &h->height);
	if (!h->id)
		exit_error(cub, "Failed to load floor HUD texture");
	h->data = mlx_get_data_addr(h->id, &h->bpp, &h->line_len, &h->endian);
	cub->floor_hud_transp = *(unsigned int *)h->data;
}

void	draw_floor_hud(t_cub *cub)
{
	int	r[8];

	if (!cub->floor_hud.id)
		return ;
	r[0] = 0;
	r[2] = WIN_W;
	r[3] = WIN_W * cub->floor_hud.height / cub->floor_hud.width;
	r[1] = 0;
	r[4] = 0;
	r[5] = 0;
	r[6] = cub->floor_hud.width;
	r[7] = cub->floor_hud.height;
	blit_scaled(cub, &cub->floor_hud, cub->floor_hud_transp, r);
}
