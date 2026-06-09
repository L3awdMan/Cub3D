/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mission_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zotaj-di <zotaj-di@learner.42.tech>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 00:08:50 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/06/10 00:08:50 by zotaj-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* SECTION 4 (src_bonus/bonus/mission_bonus.c): full-screen mission briefings.
 * A floor schedules its briefing after entering gameplay. The player gets one
 * playable second, then GS_MISSION freezes the world until Enter/Space closes
 * the current floor's full-screen briefing image. */

#include "cub3d_bonus.h"
#include "mlx.h"

static char	*mission_name(int floor)
{
	if (floor == 1)
		return ("mission_floor_1.xpm");
	if (floor == 2)
		return ("mission_floor_2.xpm");
	return ("mission_floor_3.xpm");
}

static void	load_one_mission(t_cub *cub, int i)
{
	t_img	*f;
	char	*path;

	f = &cub->ui_mission[i];
	path = ft_strjoin(MISSION_DIR, mission_name(i + 1));
	if (!path)
		exit_error(cub, "Memory allocation failed");
	f->id = mlx_xpm_file_to_image(cub->mlx, path, &f->width, &f->height);
	free(path);
	if (!f->id)
		exit_error(cub, "Failed to load mission UI texture");
	f->data = mlx_get_data_addr(f->id, &f->bpp, &f->line_len, &f->endian);
}

void	load_mission_ui(t_cub *cub)
{
	int	i;

	i = -1;
	while (++i < MISSION_COUNT)
		if (!cub->ui_mission[i].id)
			load_one_mission(cub, i);
}

void	free_mission_ui(t_cub *cub)
{
	int	i;

	i = -1;
	while (++i < MISSION_COUNT)
	{
		if (cub->ui_mission[i].id && cub->mlx)
			mlx_destroy_image(cub->mlx, cub->ui_mission[i].id);
		cub->ui_mission[i].id = NULL;
	}
}
