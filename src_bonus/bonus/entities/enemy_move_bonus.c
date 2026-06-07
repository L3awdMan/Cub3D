/* SECTION 4 (src_bonus/bonus/enemy_move_bonus.c): slow chase movement.
 * move_enemy() is called once per frame from enemy_tick(): an alive non-static
 * enemy that sees the player (los_clear) and sits within CHASE_RANGE but
 * beyond ENEMY_STOP_DIST walks toward the player at ENEMY_SPD. Collision
 * mirrors the player's 4-corner box (movement.c) so the enemy slides along
 * walls instead of clipping through; enemies never open doors. */

#include "cub3d_bonus.h"

/**
 * @brief True iff the cell containing point (x, y) is off-grid, a wall, or a
 * door (enemies cannot pass through doors). Point test for one box corner.
 */
static int	enemy_cell(t_cub *cub, double x, double y)
{
	t_map	*map;
	int		mx;
	int		my;
	char	c;

	map = &cub->map;
	mx = (int)x;
	my = (int)y;
	if (my < 0 || my >= map->height || mx < 0 || mx >= map->row_len[my])
		return (1);
	c = map->grid[my][mx];
	if (ft_strchr(WALL_CHARS, c) || c == 'D')
		return (1);
	return (0);
}

/**
 * @brief Bounding-box collision test: the enemy body is an ENEMY_PAD square
 * centered on (x, y). Any corner in a wall, a door or off-grid blocks the move.
 */
static int	enemy_blocked(t_cub *cub, double x, double y)
{
	if (enemy_cell(cub, x - ENEMY_PAD, y - ENEMY_PAD))
		return (1);
	if (enemy_cell(cub, x + ENEMY_PAD, y - ENEMY_PAD))
		return (1);
	if (enemy_cell(cub, x - ENEMY_PAD, y + ENEMY_PAD))
		return (1);
	if (enemy_cell(cub, x + ENEMY_PAD, y + ENEMY_PAD))
		return (1);
	return (0);
}

/**
 * @brief Advances the enemy along (vx, vy), testing each axis independently so
 * a blocked axis still lets the other through (wall sliding).
 */
static void	slide_toward(t_cub *cub, t_sprite *s, double vx, double vy)
{
	double	nx;
	double	ny;

	nx = s->x + vx;
	ny = s->y + vy;
	if (!enemy_blocked(cub, nx, s->y))
		s->x = nx;
	if (!enemy_blocked(cub, s->x, ny))
		s->y = ny;
}

/**
 * @brief Walks one alive enemy toward the player when it has clear sight and is
 * within CHASE_RANGE but farther than ENEMY_STOP_DIST. The fluid boss stays
 * fixed in place. Direction is normalized and scaled by ENEMY_SPD.
 */
void	move_enemy(t_cub *cub, t_sprite *s)
{
	double	dx;
	double	dy;
	double	dist;

	if (s->type >= SP_PICKUP || s->type == SP_FLUID
		|| s->state != SP_ALIVE)
		return ;
	dx = cub->player.pos_x - s->x;
	dy = cub->player.pos_y - s->y;
	dist = sqrt(dx * dx + dy * dy);
	if (dist > CHASE_RANGE || dist < ENEMY_STOP_DIST)
		return ;
	if (!los_clear(cub, s))
		return ;
	slide_toward(cub, s, dx / dist * ENEMY_SPD, dy / dist * ENEMY_SPD);
}
