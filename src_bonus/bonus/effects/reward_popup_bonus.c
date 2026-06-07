/* SECTION 4 (src_bonus/bonus/reward_popup_bonus.c): world reward pickups. A
 * killed enemy drops a meat (SP_MEAT) and a vanished boss drops a money bag
 * (SP_MONEY) as grounded billboard sprites appended to cub->sprites (guarded by
 * sprite_cap). collect_rewards() removes one on walk-over: meat restores hp and
 * fires a green pickup vignette, money is cosmetic and fires a gold one. */

#include "cub3d_bonus.h"

/**
 * @brief Appends a reward pickup sprite at (x, y). No-op when the pre-allocated
 * sprite array is full, so the append can never overflow.
 */
void	spawn_reward(t_cub *cub, double x, double y, int type)
{
	t_sprite	*s;

	if (!cub->sprites || cub->sprite_count >= cub->sprite_cap)
		return ;
	s = &cub->sprites[cub->sprite_count];
	s->x = x;
	s->y = y;
	s->type = type;
	s->state = SP_ALIVE;
	s->hp = 1;
	s->death_ms = 0;
	s->next_attack_ms = 0;
	cub->sprite_count++;
}

/**
 * @brief Applies a collected pickup: meat heals (clamped at PLAYER_HP_MAX) and
 * flashes green; money is cosmetic and flashes gold.
 */
static void	reward_effect(t_cub *cub, int type)
{
	if (type == SP_MEAT)
	{
		cub->player.hp += MEAT_HEAL;
		if (cub->player.hp > PLAYER_HP_MAX)
			cub->player.hp = PLAYER_HP_MAX;
		cub->flash_rgb = FLASH_GREEN;
	}
	else
	{
		cub->flash_rgb = FLASH_GOLD;
		if (cub->floor == 3)
			start_cutscene(cub, CUT_END_F3, CUT_ACT_FINAL);
		else
			enter_state(cub, GS_WIN);
	}
	cub->flash_ms = now_ms();
}

/**
 * @brief 1 when sprite i is a reward pickup within REWARD_PICK_DIST cells of
 * the player (squared distance, no sqrt).
 */
static int	reward_in_reach(t_cub *cub, int i)
{
	double	dx;
	double	dy;

	if (cub->sprites[i].type != SP_MEAT && cub->sprites[i].type != SP_MONEY)
		return (0);
	dx = cub->sprites[i].x - cub->player.pos_x;
	dy = cub->sprites[i].y - cub->player.pos_y;
	return (dx * dx + dy * dy <= REWARD_PICK_DIST * REWARD_PICK_DIST);
}

/**
 * @brief Walk-over collection: applies and removes every reward pickup the
 * player is standing on (swap-shrink, same O(1) pattern as drop_pickup_sprite).
 */
void	collect_rewards(t_cub *cub)
{
	int	i;

	if (!cub->sprites || cub->sprite_count <= 0)
		return ;
	i = -1;
	while (++i < cub->sprite_count)
	{
		if (reward_in_reach(cub, i))
		{
			reward_effect(cub, cub->sprites[i].type);
			cub->sprites[i] = cub->sprites[cub->sprite_count - 1];
			cub->sprite_count--;
			i--;
		}
	}
}
