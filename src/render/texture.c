/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 10:08:18 by baelgadi          #+#    #+#             */
/*   Updated: 2026/04/12 11:19:57 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "mlx.h"

/**
 * @brief Reads a pixel from a texture image buffer
 * @warning Casting needed for the same reasons as put_px()
 *
 * FIX 2 (src/render/texture.c): x/y are clamped before indexing.
 * wall_x * tex->width in calc_tex_x can round to exactly tex->width on
 * corner hits (FP edge), which would cause a 1-pixel OOB heap read every
 * frame. Mirrors the tex_y clamp already in draw_wall.c:draw_tex_col.
 */
unsigned int	get_tex_pixel(t_img *tex, int x, int y)
{
	char	*px;

	if (x < 0)
		x = 0;
	if (x >= tex->width)
		x = tex->width - 1;
	if (y < 0)
		y = 0;
	if (y >= tex->height)
		y = tex->height - 1;
	px = tex->data + (y * tex->line_len + x * (tex->bpp / 8));
	return (*(unsigned int *)px);
}

/**
 * @brief Loads one XPM texture into tex[idx]
 */
static void	load_single_tex(t_cub *cub, int idx)
{
	t_img	*t;

	t = &cub->tex[idx];
	t->id = mlx_xpm_file_to_image(cub->mlx, cub->map.tex_path[idx],
			&t->width, &t->height);
	if (!t->id)
		exit_error(cub, "Failed to load texture");
	t->data = mlx_get_data_addr(t->id, &t->bpp, &t->line_len, &t->endian);
}

/**
 * @brief Loads all 4 wall textures (NO, SO, WE and EA)
 */
void	load_textures(t_cub *cub)
{
	int	i;

	i = 0;
	while (i < TEX_COUNT)
	{
		load_single_tex(cub, i);
		i++;
	}
}

/**
 * @brief Destroys all loaded texture imgs
 */
void	free_textures(t_cub *cub)
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
}
