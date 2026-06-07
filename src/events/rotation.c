/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotation.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/12 13:14:44 by baelgadi          #+#    #+#             */
/*   Updated: 2026/04/12 19:50:14 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * @brief Rotates direction and plane vectors by angle
 * @details It uses the standard 2d rotation matrix:
 * 
 * new_x = old_x * cos(a) - old_y * sin(a)
 * 
 * new_y = old_y * sin(a) + old_y * cos(a)
 * @note Both dir and plane are rotated together to keep the perpendicularity
 */
static void	rotate_vectors(t_player *p, double angle)
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
