/* SECTION 4 (src_bonus/bonus/ui/difficulty_bonus.c): difficulty damage scaling.
 * Split out of menu_input_bonus.c to keep that file Norm-compliant. player_dmg()
 * scales enemy damage by the chosen difficulty so harder levels drain hp faster.
 * Used at both player-damage sites (enemy melee + projectile impact). */

#include "cub3d_bonus.h"

static const double	g_diff_mult[3] = {1.0, 1.5, 2.0};

/**
 * @brief Enemy damage scaled by the selected difficulty (Easy 1x, Skilled 1.5x,
 * Gigachad 2x) so harder levels lose hp faster.
 */
int	player_dmg(t_cub *cub)
{
	return ((int)(ATTACK_DMG * g_diff_mult[cub->difficulty]));
}
