/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 10:07:34 by baelgadi          #+#    #+#             */
/*   Updated: 2026/04/12 11:13:08 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "mlx.h"

/**
 * @brief Writes a pixel at (x, y) into the image buffer
 * @warning Casting is needed because otherwise *px = color would only write
 * 1 byte (the lowest of color) and everything would be only in blue
 */
void	put_px(t_img *img, int x, int y, unsigned int color)
{
	char	*px;

	if (x < 0 || y < 0 || x >= WIN_W || y >= WIN_H)
		return ;
	px = img->data + (y * img->line_len + x * (img->bpp / 8));
	*(unsigned int *)px = color;
}

/**
 * @brief Renders a complete frame (cast rays then pushes image)
 * @note Called from loop_hook() at every frame
 *
 * SECTION 3 (src/render/render.c): the crosshair is drawn after the wall
 * pass so the column writes in cast_all_rays cannot paint over the cross.
 */
static void	render_frame(t_cub *cub)
{
	cast_all_rays(cub);
	draw_crosshair(cub);
	mlx_put_image_to_window(cub->mlx, cub->win, cub->img.id, 0, 0);
}

/**
 * @brief Main loop hook
 * @return 0 to keep MLX loop running
 *
 * SECTION 3 (src/render/render.c): update_horizon runs before the render
 * pass so this frame's head-bob matches this frame's movement state.
 */
int	loop_hook(void *param)
{
	t_cub	*cub;

	cub = (t_cub *)param;
	apply_movement(cub);
	apply_rotation(cub);
	update_horizon(cub);
	render_frame(cub);
	return (0);
}
