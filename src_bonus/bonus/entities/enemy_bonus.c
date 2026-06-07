/* SECTION 4 (src_bonus/bonus/enemy_bonus.c): shooting an enemy. shoot_hitscan()
 * is fired once per shot by update_weapon(): it projects every alive enemy into
 * camera space (same inverse-camera matrix as the sprite pass), keeps the ones
 * whose centre column falls inside the SHOT_AIM band, are within SHOT_RANGE,
 * and are not occluded by a wall (depth < cub->zbuf[col], the previous frame's
 * wall depth), then flips the nearest one to SP_DYING. The sprite pass then
 * plays its death set once via death_type(). */

#include "cub3d_bonus.h"

/**
 * @brief Maps an alive enemy type to its death animation type.
 */
int	death_type(int type)
{
	return (SP_POD_DEATH + type);
}

/**
 * @brief Maps an alive enemy type to its firing (attack) animation type.
 */
int	firing_type(int type)
{
	return (SP_POD_FIRING + type);
}

/**
 * @brief Projects a sprite into camera space. Returns its depth (0 when behind
 * the camera or the camera plane is degenerate) and writes its screen column.
 */
static double	enemy_project(t_cub *cub, t_sprite *sp, int *col)
{
	t_player	*p;
	double		rx;
	double		ry;
	double		inv;
	double		depth;

	p = &cub->player;
	rx = sp->x - p->pos_x;
	ry = sp->y - p->pos_y;
	inv = p->plane_x * p->dir_y - p->dir_x * p->plane_y;
	if (inv > -1e-9 && inv < 1e-9)
		return (0.0);
	inv = 1.0 / inv;
	depth = inv * (-p->plane_y * rx + p->plane_x * ry);
	if (depth < SP_NEAR)
		return (0.0);
	*col = (int)((WIN_W / 2.0) * (1.0
				+ inv * (p->dir_y * rx - p->dir_x * ry) / depth));
	return (depth);
}

/**
 * @brief Tests sprite i as a shot target. Alive enemies only, inside the aim
 * band, nearer than the best so far, and not behind a wall. Updates *bestd
 * and returns 1 on a hit so the caller can record the index.
 */
static int	enemy_candidate(t_cub *cub, int i, double *bestd)
{
	int		col;
	double	depth;

	if (cub->sprites[i].type >= SP_PICKUP
		|| cub->sprites[i].state == SP_DYING)
		return (0);
	depth = enemy_project(cub, &cub->sprites[i], &col);
	if (depth <= 0.0 || depth >= *bestd)
		return (0);
	if (col < WIN_W / 2 - SHOT_AIM || col > WIN_W / 2 + SHOT_AIM)
		return (0);
	if (depth > cub->zbuf[col])
		return (0);
	*bestd = depth;
	return (1);
}

/**
 * @brief Resolves one shot: kills the nearest alive alien under the crosshair.
 * No-op when nothing lines up, so empty shots are silent.
 */
void	shoot_hitscan(t_cub *cub)
{
	int		i;
	int		best;
	double	bestd;

	if (!cub->sprites || cub->sprite_count <= 0)
		return ;
	best = -1;
	bestd = SHOT_RANGE;
	i = -1;
	while (++i < cub->sprite_count)
		if (enemy_candidate(cub, i, &bestd))
			best = i;
	if (best < 0)
		return ;
	if (--cub->sprites[best].hp <= 0)
	{
		cub->sprites[best].state = SP_DYING;
		cub->sprites[best].death_ms = now_ms();
		if (cub->lives > 0)
			cub->lives--;
		if (cub->sprites[best].type != SP_FLUID)
			spawn_reward(cub, cub->sprites[best].x,
				cub->sprites[best].y, SP_MEAT);
	}
}
