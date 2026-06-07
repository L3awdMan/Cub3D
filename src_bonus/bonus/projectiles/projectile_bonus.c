/* SECTION 4 (src_bonus/bonus/projectile_bonus.c): in-flight enemy projectiles.
 * When an enemy fires (enemy_ai_bonus.c) it calls spawn_projectile, which aims
 * a pooled shot at the player. update_projectiles() flies every active shot
 * PROJ_SPD cells per frame, expiring it on a wall/closed-door hit and removing
 * ATTACK_DMG from the player (clamped at 0, no game-over) when it reaches the
 * player within PROJ_HIT_DIST. The pool is fixed (MAX_PROJ) so nothing is
 * heap-allocated per shot. */

#include "cub3d_bonus.h"

/**
 * @brief Maps an alive enemy role to its projectile (flying shot) art type.
 */
int	proj_type(int type)
{
	return (SP_POD_PROJ + type);
}

/**
 * @brief Returns 1 if world point (x, y) sits in a wall, a closed door or off
 * the grid — i.e. the projectile should expire there.
 */
static int	proj_blocked(t_cub *cub, double x, double y)
{
	int		mx;
	int		my;
	char	c;

	mx = (int)x;
	my = (int)y;
	if (mx < 0 || my < 0 || my >= cub->map.height || !cub->map.grid[my])
		return (1);
	if (mx >= cub->map.row_len[my])
		return (1);
	c = cub->map.grid[my][mx];
	if (ft_strchr(WALL_CHARS, c))
		return (1);
	if (c == 'D' && !door_is_open(cub, my, mx))
		return (1);
	return (0);
}

/**
 * @brief Spawns a projectile from enemy `s` aimed at the player. Claims the
 * first free pool slot; its velocity is the unit enemy->player direction
 * scaled by PROJ_SPD. No-op if the pool is full or the enemy is on the player.
 */
void	spawn_projectile(t_cub *cub, t_sprite *s)
{
	int		i;
	double	d[3];

	d[0] = cub->player.pos_x - s->x;
	d[1] = cub->player.pos_y - s->y;
	d[2] = sqrt(d[0] * d[0] + d[1] * d[1]);
	if (d[2] < 1e-6)
		return ;
	i = -1;
	while (++i < MAX_PROJ)
	{
		if (cub->projectiles[i].active)
			continue ;
		cub->projectiles[i].x = s->x;
		cub->projectiles[i].y = s->y;
		cub->projectiles[i].dx = d[0] / d[2] * PROJ_SPD;
		cub->projectiles[i].dy = d[1] / d[2] * PROJ_SPD;
		cub->projectiles[i].type = proj_type(s->type);
		cub->projectiles[i].active = 1;
		return ;
	}
}

/**
 * @brief Advances one projectile by its velocity. Expires it on a wall/door
 * hit; on reaching the player within PROJ_HIT_DIST it expires and chips
 * ATTACK_DMG off the player's hp (clamped at 0).
 */
static void	step_one(t_cub *cub, t_proj *p)
{
	double	dx;
	double	dy;

	p->x += p->dx;
	p->y += p->dy;
	if (proj_blocked(cub, p->x, p->y))
	{
		p->active = 0;
		return ;
	}
	dx = cub->player.pos_x - p->x;
	dy = cub->player.pos_y - p->y;
	if (dx * dx + dy * dy > PROJ_HIT_DIST * PROJ_HIT_DIST)
		return ;
	p->active = 0;
	cub->player.hp -= player_dmg(cub);
	if (cub->player.hp < 0)
		cub->player.hp = 0;
	cub->player.hurt_ms = now_ms();
}

/**
 * @brief Advances every active projectile for this frame.
 */
void	update_projectiles(t_cub *cub)
{
	int	i;

	i = -1;
	while (++i < MAX_PROJ)
	{
		if (cub->projectiles[i].active)
			step_one(cub, &cub->projectiles[i]);
	}
}
