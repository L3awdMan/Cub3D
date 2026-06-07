/* SECTION 4 (src_bonus/bonus/enemy_ai_bonus.c): stationary enemy attack AI.
 * update_enemies() runs once per frame from loop_hook(). Any alive enemy whose
 * clear line of sight reaches the player within ATTACK_RANGE fires on a
 * cooldown: it flips to SP_ATTACKING (its firing frames play for FIRE_MS) and
 * spawns a projectile (projectile_bonus.c) that travels to the player and
 * removes ATTACK_DMG on impact — no instant hitscan. It won't attack again
 * until ATTACK_CD_MS has passed. Enemies do not move (chase is a future
 * phase). */

#include "cub3d_bonus.h"

/**
 * @brief Returns 1 if world point (x, y) sits in a wall, closed door or off
 * the grid — i.e. blocks an enemy's line of sight to the player.
 */
static int	is_blocked(t_cub *cub, double x, double y)
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
	if (ft_strchr(WALL_CHARS, c) || c == 'D')
		return (1);
	return (0);
}

/**
 * @brief Marches a thin ray from the enemy toward the player, sampling every
 * 0.1 cell. Returns 0 as soon as a wall blocks the path, 1 if the player is
 * reachable.
 */
int	los_clear(t_cub *cub, t_sprite *s)
{
	double	dx;
	double	dy;
	double	dist;
	double	t;

	dx = cub->player.pos_x - s->x;
	dy = cub->player.pos_y - s->y;
	dist = sqrt(dx * dx + dy * dy);
	if (dist < 1e-6)
		return (1);
	t = 0.1;
	while (t < dist)
	{
		if (is_blocked(cub, s->x + dx * (t / dist), s->y + dy * (t / dist)))
			return (0);
		t += 0.1;
	}
	return (1);
}

/**
 * @brief Fires one attack. A role with projectile art spawns a flying shot
 * (damage lands on impact); a role with an empty proj list (the fluid alien)
 * stays invisible and chips the player's hp instantly, clamped at 0.
 */
static void	enemy_fire(t_cub *cub, t_sprite *s)
{
	if (cub->sprite_anims[proj_type(s->type)].count > 0)
	{
		spawn_projectile(cub, s);
		return ;
	}
	cub->player.hp -= player_dmg(cub);
	if (cub->player.hp < 0)
		cub->player.hp = 0;
	cub->player.hurt_ms = now_ms();
}

/**
 * @brief Steps one enemy: ends a finished firing pose, then (if in range, off
 * cooldown and with clear sight) opens fire and chips the player's hp.
 */
static void	enemy_tick(t_cub *cub, t_sprite *s, long now)
{
	double	dx;
	double	dy;

	if (s->type >= SP_PICKUP || s->state == SP_DYING)
		return ;
	if (s->state == SP_ATTACKING && now - s->death_ms >= FIRE_MS)
		s->state = SP_ALIVE;
	move_enemy(cub, s);
	dx = cub->player.pos_x - s->x;
	dy = cub->player.pos_y - s->y;
	if (dx * dx + dy * dy > ATTACK_RANGE * ATTACK_RANGE)
		return ;
	if (now < s->next_attack_ms || !los_clear(cub, s))
		return ;
	s->state = SP_ATTACKING;
	s->death_ms = now;
	s->next_attack_ms = now + ATTACK_CD_MS;
	enemy_fire(cub, s);
}

/**
 * @brief Advances every enemy's attack AI for this frame.
 */
void	update_enemies(t_cub *cub)
{
	int		i;
	long	now;

	if (!cub->sprites || cub->sprite_count <= 0)
		return ;
	now = now_ms();
	i = -1;
	while (++i < cub->sprite_count)
		enemy_tick(cub, &cub->sprites[i], now);
}
