/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cutscene_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zotaj-di <zotaj-di@learner.42.tech>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 00:06:32 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/06/10 00:06:32 by zotaj-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"
#include "mlx.h"

/* SECTION 4 (src_bonus/bonus/cutscene_bonus.c): full-screen cutscene playback.
 * cutscene_meta() sets the frame count and per-frame delay for a cutscene id,
 * load_cutscene() lazily loads its CUT_DIR XPM sequence, start_cutscene() enters
 * GS_CUTSCENE and free_cutscene() frees the frames. Path building lives in
 * cutscene_path_bonus.c. */

static void	cutscene_meta(int id, int *count, int *frame_ms)
{
	if (id == CUT_BEGIN_F1)
	{
		*count = 35;
		*frame_ms = CUT_BEGIN_MS;
		return ;
	}
	if (id == CUT_END_F3)
		*count = 87;
	else
		*count = 122;
	*frame_ms = CUT_FRAME_MS;
}

void	free_cutscene(t_cub *cub)
{
	int	i;

	i = -1;
	while (++i < CUT_MAX_FRAMES)
	{
		if (cub->cutscene.frames[i].id && cub->mlx)
			mlx_destroy_image(cub->mlx, cub->cutscene.frames[i].id);
		cub->cutscene.frames[i].id = NULL;
	}
	cub->cutscene.id = 0;
	cub->cutscene.count = 0;
	cub->cutscene.frame = 0;
}

static void	load_cutscene_frame(t_cub *cub, int id, int i)
{
	t_img	*f;
	char	*path;

	path = cutscene_path(id, i);
	if (!path)
		exit_error(cub, "Memory allocation failed");
	f = &cub->cutscene.frames[i];
	f->id = mlx_xpm_file_to_image(cub->mlx, path, &f->width, &f->height);
	free(path);
	if (!f->id)
		exit_error(cub, "Failed to load cutscene texture");
	f->data = mlx_get_data_addr(f->id, &f->bpp, &f->line_len, &f->endian);
}

void	load_cutscene(t_cub *cub, int id)
{
	int	i;

	free_cutscene(cub);
	cub->cutscene.id = id;
	cutscene_meta(id, &cub->cutscene.count, &cub->cutscene.frame_ms);
	i = -1;
	while (++i < cub->cutscene.count)
		load_cutscene_frame(cub, id, i);
}

void	start_cutscene(t_cub *cub, int id, int next_action)
{
	load_cutscene(cub, id);
	cub->cutscene.next_action = next_action;
	cub->cutscene.start_ms = now_ms();
	cub->cutscene.frame = 0;
	cub->game_state = GS_CUTSCENE;
	ft_bzero(cub->keys, sizeof(cub->keys));
}
