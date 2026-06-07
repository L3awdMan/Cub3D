#include "cub3d_bonus.h"

/**
 * @brief Initializes ray direction and delta distances for col x
 * @details camera_x maps screen colums to [-1, +1].
 * Ray direction is: player dir + camera plane, scaled by camera_x.
 * Delta distances are | 1 / ray_dir | (1e30 when ray_dir component is 0, to
 * avoid using infinity)
 */
void	init_ray(t_ray *ray, t_player *p, int x)
{
	double	camera_x;

	camera_x = 2.0 * x / WIN_W - 1.0;
	ray->dir_x = p->dir_x + p->plane_x * camera_x;
	ray->dir_y = p->dir_y + p->plane_y * camera_x;
	ray->map_x = (int)p->pos_x;
	ray->map_y = (int)p->pos_y;
	if (ray->dir_x == 0)
		ray->delta_x = 1e30;
	else
		ray->delta_x = fabs(1.0 / ray->dir_x);
	if (ray->dir_y == 0)
		ray->delta_y = 1e30;
	else
		ray->delta_y = fabs(1.0 / ray->dir_y);
}

/**
 * @brief Computes the step direction and initial side distance
 * @note Step is +1 or -1 per axis
 * @details side_dist is the distance from the player to the 1st grid line in
 * that direction.
 * This determines which cell face the DDA hits first
 */
void	init_step_side(t_ray *ray, t_player *p)
{
	if (ray->dir_x < 0)
	{
		ray->step_x = -1;
		ray->side_dist_x = (p->pos_x - ray->map_x) * ray->delta_x;
	}
	else
	{
		ray->step_x = 1;
		ray->side_dist_x = (ray->map_x + 1.0 - p->pos_x) * ray->delta_x;
	}
	if (ray->dir_y < 0)
	{
		ray->step_y = -1;
		ray->side_dist_y = (p->pos_y - ray->map_y) * ray->delta_y;
	}
	else
	{
		ray->step_y = 1;
		ray->side_dist_y = (ray->map_y + 1.0 - p->pos_y) * ray->delta_y;
	}
}

/**
 * @brief Calculates wall stripes drawing parameters from a ray hit
 * @details perp_dist removes fisheye by using a perpendicular distance
 * (side_dist - delta_dist for the hit axis).
 *
 * SECTION 3 (src/execution/ray_utils.c) changes:
 *  - horizon (new param) replaces hard-coded WIN_H/2 so the stripe follows
 *    the head-bob computed by update_horizon() each frame.
 *  - dw->shade filled here: fog factor capped at 1.0 (close walls stay
 *    bright) plus SIDE_SHADE multiplier when ray->side == 1 — Wolfenstein
 *    "N/S walls darker than E/W" cue.
 */
void	calc_draw_params(t_draw *dw, t_ray *ray, int horizon)
{
	if (ray->side == 0)
		ray->perp_dist = ray->side_dist_x - ray->delta_x;
	else
		ray->perp_dist = ray->side_dist_y - ray->delta_y;
	if (ray->perp_dist < 0.001)
		ray->perp_dist = 0.001;
	dw->height = (int)(WIN_H / ray->perp_dist);
	dw->start = horizon - dw->height / 2;
	if (dw->start < 0)
		dw->start = 0;
	dw->end = horizon + dw->height / 2;
	if (dw->end >= WIN_H)
		dw->end = WIN_H - 1;
	dw->shade = fmin(1.0, FOG_K / ray->perp_dist);
	if (ray->side == 1)
		dw->shade *= SIDE_SHADE;
}

/**
 * @brief Selects the texture to use based on the wall face that was hit
 * @note side = 0 means vertical wall (WE or EA based on step_x), while
 * side = 1 means a horizontal wall (NO or SO based on step_y)
 */
int	select_texture(t_ray *ray)
{
	if (ray->side == 0)
	{
		if (ray->step_x < 0)
			return (TEX_WE);
		return (TEX_EA);
	}
	if (ray->step_y < 0)
		return (TEX_NO);
	return (TEX_SO);
}

/**
 * @brief Computes the fractional position [0..1] where the ray crosses
 * the face of the cell currently entered. Mirrors calc_wall_x but uses
 * the entry-side distance directly so the door pass-through test in
 * dda_step can run BEFORE perp_dist is finalized.
 *
 * SECTION 4 (src_bonus/execution/ray_utils.c): door slide animation —
 * if the ray's hit fraction falls inside the door's retracted central
 * band the DDA keeps stepping (reveals geometry behind).
 */
double	ray_hit_fraction(t_ray *ray, t_player *p)
{
	double	dist;
	double	hit;

	if (ray->side == 0)
		dist = ray->side_dist_x - ray->delta_x;
	else
		dist = ray->side_dist_y - ray->delta_y;
	if (ray->side == 0)
		hit = p->pos_y + dist * ray->dir_y;
	else
		hit = p->pos_x + dist * ray->dir_x;
	return (hit - floor(hit));
}
