/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotation.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zotaj-di <zotaj-di@learner.42.tech>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 00:09:38 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/06/10 00:09:38 by zotaj-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

/**
 * @brief Rotates direction and plane vectors by angle
 * @details It uses the standard 2d rotation matrix:
 *
 * new_x = old_x * cos(a) - old_y * sin(a)
 *
 * new_y = old_y * sin(a) + old_y * cos(a)
 * @note Both dir and plane are rotated together to keep the perpendicularity
 *
 * de-static'd so the mouse-look
 * module (src_bonus/bonus/mouse_bonus.c) can reuse the exact same rotation
 * math — no duplicated trig.
 */
void	rotate_vectors(t_player *p, double angle)
{
	double	old_dir_x;
	double	old_plane_x;
	double	cos_a;
	double	sin_a;

	cos_a = cos(angle);
	sin_a = sin(angle);
	old_dir_x = p->dir_x;
	p->dir_x = old_dir_x * cos_a - p->dir_y * sin_a;
	p->dir_y = old_dir_x * sin_a + p->dir_y * cos_a;
	old_plane_x = p->plane_x;
	p->plane_x = old_plane_x * cos_a - p->plane_y * sin_a;
	p->plane_y = old_plane_x * sin_a + p->plane_y * cos_a;
}

/**
 * @brief Applies rotation based on which arrow key is held
 * 
 * @param cub 
 */
void	apply_rotation(t_cub *cub)
{
	if (cub->keys[KEY_LEFT])
		rotate_vectors(&cub->player, -ROT_SPD);
	if (cub->keys[KEY_RIGHT])
		rotate_vectors(&cub->player, ROT_SPD);
}
