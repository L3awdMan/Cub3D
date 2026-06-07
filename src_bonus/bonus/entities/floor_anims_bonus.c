/* SECTION 4 (src_bonus/bonus/floor_anims_bonus.c): per-floor enemy art tables.
 * Map tile chars name floor-agnostic roles (SP_POD/FLUID/SCIENTIST/SPECIAL/
 * POD_ALIEN); the active floor decides which Blake Stone sprite set fills each
 * slot. floor_set() returns the table for cub->floor (default = floor 1) and
 * load_anim_sprites() walks it. The SP_SPECIAL fluid alien is shared across
 * floors and has no firing set, so its walk loop doubles as its attack pose. */

#include "cub3d_bonus.h"

#define ENEMY_DIR "./textures/blake_stone_xpm/sprites/enemies_and_bosses/"

static const char *const	g_f1_pod_walk[] = {
	ENEMY_DIR "enemy_floor1/walking/enemy_floor1_walking_f00.xpm",
	ENEMY_DIR "enemy_floor1/walking/enemy_floor1_walking_f01.xpm",
	ENEMY_DIR "enemy_floor1/walking/enemy_floor1_walking_f02.xpm",
	ENEMY_DIR "enemy_floor1/walking/enemy_floor1_walking_f03.xpm",
	NULL,
};

static const char *const	g_f1_pod_fire[] = {
	ENEMY_DIR "enemy_floor1/firing/enemy_floor1_firing_f00.xpm",
	ENEMY_DIR "enemy_floor1/firing/enemy_floor1_firing_f01.xpm",
	ENEMY_DIR "enemy_floor1/firing/enemy_floor1_firing_f02.xpm",
	NULL,
};

static const char *const	g_f1_pod_death[] = {
	ENEMY_DIR "enemy_floor1/death/enemy_floor1_death_f00.xpm",
	ENEMY_DIR "enemy_floor1/death/enemy_floor1_death_f01.xpm",
	ENEMY_DIR "enemy_floor1/death/enemy_floor1_death_f02.xpm",
	ENEMY_DIR "enemy_floor1/death/enemy_floor1_death_f03.xpm",
	ENEMY_DIR "enemy_floor1/death/enemy_floor1_death_f04.xpm",
	ENEMY_DIR "enemy_floor1/death/enemy_floor1_death_f05.xpm",
	NULL,
};

static const char *const	g_f1_boss_walk[] = {
	ENEMY_DIR "boss_floor1/walking/boss_floor1_walking_f00.xpm",
	ENEMY_DIR "boss_floor1/walking/boss_floor1_walking_f01.xpm",
	ENEMY_DIR "boss_floor1/walking/boss_floor1_walking_f02.xpm",
	ENEMY_DIR "boss_floor1/walking/boss_floor1_walking_f03.xpm",
	NULL,
};

static const char *const	g_f1_boss_fire[] = {
	ENEMY_DIR "boss_floor1/firing/boss_floor1_firing_f00.xpm",
	ENEMY_DIR "boss_floor1/firing/boss_floor1_firing_f01.xpm",
	ENEMY_DIR "boss_floor1/firing/boss_floor1_firing_f02.xpm",
	NULL,
};

static const char *const	g_f1_boss_death[] = {
	ENEMY_DIR "boss_floor1/death/boss_floor1_death_f00.xpm",
	ENEMY_DIR "boss_floor1/death/boss_floor1_death_f01.xpm",
	ENEMY_DIR "boss_floor1/death/boss_floor1_death_f02.xpm",
	ENEMY_DIR "boss_floor1/death/boss_floor1_death_f03.xpm",
	ENEMY_DIR "boss_floor1/death/boss_floor1_death_f04.xpm",
	ENEMY_DIR "boss_floor1/death/boss_floor1_death_f05.xpm",
	NULL,
};

static const char *const	g_pod_walk[] = {
	ENEMY_DIR "pod_alien_floor1/walking/pod_alien_floor1_walking_f00.xpm",
	ENEMY_DIR "pod_alien_floor1/walking/pod_alien_floor1_walking_f01.xpm",
	ENEMY_DIR "pod_alien_floor1/walking/pod_alien_floor1_walking_f02.xpm",
	NULL,
};

static const char *const	g_pod_fire[] = {
	ENEMY_DIR "pod_alien_floor1/throwing/pod_alien_floor1_throwing_f00.xpm",
	ENEMY_DIR "pod_alien_floor1/throwing/pod_alien_floor1_throwing_f01.xpm",
	NULL,
};

static const char *const	g_pod_death[] = {
	ENEMY_DIR "pod_alien_floor1/death/pod_alien_floor1_death_f00.xpm",
	ENEMY_DIR "pod_alien_floor1/death/pod_alien_floor1_death_f01.xpm",
	ENEMY_DIR "pod_alien_floor1/death/pod_alien_floor1_death_f02.xpm",
	NULL,
};

static const char *const	g_special_walk[] = {
	ENEMY_DIR "fluid_alien_special/moving/fluid_alien_special_moving_f00.xpm",
	ENEMY_DIR "fluid_alien_special/moving/fluid_alien_special_moving_f01.xpm",
	NULL,
};

static const char *const	g_special_death[] = {
	ENEMY_DIR "fluid_alien_special/death/fluid_alien_special_death_f00.xpm",
	ENEMY_DIR "fluid_alien_special/death/fluid_alien_special_death_f01.xpm",
	ENEMY_DIR "fluid_alien_special/death/fluid_alien_special_death_f02.xpm",
	ENEMY_DIR "fluid_alien_special/death/fluid_alien_special_death_f03.xpm",
	NULL,
};

static const char *const	g_f2_pod_walk[] = {
	ENEMY_DIR "enemy_floor2/walking/enemy_floor2_walking_f00.xpm",
	ENEMY_DIR "enemy_floor2/walking/enemy_floor2_walking_f01.xpm",
	ENEMY_DIR "enemy_floor2/walking/enemy_floor2_walking_f02.xpm",
	ENEMY_DIR "enemy_floor2/walking/enemy_floor2_walking_f03.xpm",
	NULL,
};

static const char *const	g_f2_pod_fire[] = {
	ENEMY_DIR "enemy_floor2/firing/enemy_floor2_firing_f00.xpm",
	ENEMY_DIR "enemy_floor2/firing/enemy_floor2_firing_f01.xpm",
	ENEMY_DIR "enemy_floor2/firing/enemy_floor2_firing_f02.xpm",
	NULL,
};

static const char *const	g_f2_pod_death[] = {
	ENEMY_DIR "enemy_floor2/death/enemy_floor2_death_f00.xpm",
	ENEMY_DIR "enemy_floor2/death/enemy_floor2_death_f01.xpm",
	ENEMY_DIR "enemy_floor2/death/enemy_floor2_death_f02.xpm",
	ENEMY_DIR "enemy_floor2/death/enemy_floor2_death_f03.xpm",
	ENEMY_DIR "enemy_floor2/death/enemy_floor2_death_f04.xpm",
	ENEMY_DIR "enemy_floor2/death/enemy_floor2_death_f05.xpm",
	NULL,
};

static const char *const	g_f2_boss_walk[] = {
	ENEMY_DIR "boss_floor2/walking/boss_floor2_walking_f00.xpm",
	ENEMY_DIR "boss_floor2/walking/boss_floor2_walking_f01.xpm",
	ENEMY_DIR "boss_floor2/walking/boss_floor2_walking_f02.xpm",
	ENEMY_DIR "boss_floor2/walking/boss_floor2_walking_f03.xpm",
	NULL,
};

static const char *const	g_f2_boss_fire[] = {
	ENEMY_DIR "boss_floor2/firing/boss_floor2_firing_f00.xpm",
	ENEMY_DIR "boss_floor2/firing/boss_floor2_firing_f01.xpm",
	ENEMY_DIR "boss_floor2/firing/boss_floor2_firing_f02.xpm",
	NULL,
};

static const char *const	g_f2_boss_death[] = {
	ENEMY_DIR "boss_floor2/death/boss_floor2_death_f00.xpm",
	ENEMY_DIR "boss_floor2/death/boss_floor2_death_f01.xpm",
	ENEMY_DIR "boss_floor2/death/boss_floor2_death_f02.xpm",
	ENEMY_DIR "boss_floor2/death/boss_floor2_death_f03.xpm",
	ENEMY_DIR "boss_floor2/death/boss_floor2_death_f04.xpm",
	ENEMY_DIR "boss_floor2/death/boss_floor2_death_f05.xpm",
	NULL,
};

static const char *const	g_f3_blue_walk[] = {
	ENEMY_DIR "enemy_floor3_blue_guard/walking/"
	"enemy_floor3_blue_guard_walking_f00.xpm",
	ENEMY_DIR "enemy_floor3_blue_guard/walking/"
	"enemy_floor3_blue_guard_walking_f01.xpm",
	ENEMY_DIR "enemy_floor3_blue_guard/walking/"
	"enemy_floor3_blue_guard_walking_f02.xpm",
	ENEMY_DIR "enemy_floor3_blue_guard/walking/"
	"enemy_floor3_blue_guard_walking_f03.xpm",
	NULL,
};

static const char *const	g_f3_blue_fire[] = {
	ENEMY_DIR "enemy_floor3_blue_guard/firing/"
	"enemy_floor3_blue_guard_firing_f00.xpm",
	ENEMY_DIR "enemy_floor3_blue_guard/firing/"
	"enemy_floor3_blue_guard_firing_f01.xpm",
	ENEMY_DIR "enemy_floor3_blue_guard/firing/"
	"enemy_floor3_blue_guard_firing_f02.xpm",
	NULL,
};

static const char *const	g_f3_blue_death[] = {
	ENEMY_DIR "enemy_floor3_blue_guard/death/"
	"enemy_floor3_blue_guard_death_f00.xpm",
	ENEMY_DIR "enemy_floor3_blue_guard/death/"
	"enemy_floor3_blue_guard_death_f01.xpm",
	ENEMY_DIR "enemy_floor3_blue_guard/death/"
	"enemy_floor3_blue_guard_death_f02.xpm",
	ENEMY_DIR "enemy_floor3_blue_guard/death/"
	"enemy_floor3_blue_guard_death_f03.xpm",
	ENEMY_DIR "enemy_floor3_blue_guard/death/"
	"enemy_floor3_blue_guard_death_f04.xpm",
	ENEMY_DIR "enemy_floor3_blue_guard/death/"
	"enemy_floor3_blue_guard_death_f05.xpm",
	NULL,
};

static const char *const	g_f3_red_walk[] = {
	ENEMY_DIR "enemy_floor3_red_guard/walking/"
	"enemy_floor3_red_guard_walking_f00.xpm",
	ENEMY_DIR "enemy_floor3_red_guard/walking/"
	"enemy_floor3_red_guard_walking_f01.xpm",
	ENEMY_DIR "enemy_floor3_red_guard/walking/"
	"enemy_floor3_red_guard_walking_f02.xpm",
	ENEMY_DIR "enemy_floor3_red_guard/walking/"
	"enemy_floor3_red_guard_walking_f03.xpm",
	NULL,
};

static const char *const	g_f3_red_fire[] = {
	ENEMY_DIR "enemy_floor3_red_guard/firing/"
	"enemy_floor3_red_guard_firing_f00.xpm",
	ENEMY_DIR "enemy_floor3_red_guard/firing/"
	"enemy_floor3_red_guard_firing_f01.xpm",
	ENEMY_DIR "enemy_floor3_red_guard/firing/"
	"enemy_floor3_red_guard_firing_f02.xpm",
	NULL,
};

static const char *const	g_f3_red_death[] = {
	ENEMY_DIR "enemy_floor3_red_guard/death/"
	"enemy_floor3_red_guard_death_f00.xpm",
	ENEMY_DIR "enemy_floor3_red_guard/death/"
	"enemy_floor3_red_guard_death_f01.xpm",
	ENEMY_DIR "enemy_floor3_red_guard/death/"
	"enemy_floor3_red_guard_death_f02.xpm",
	ENEMY_DIR "enemy_floor3_red_guard/death/"
	"enemy_floor3_red_guard_death_f03.xpm",
	ENEMY_DIR "enemy_floor3_red_guard/death/"
	"enemy_floor3_red_guard_death_f04.xpm",
	ENEMY_DIR "enemy_floor3_red_guard/death/"
	"enemy_floor3_red_guard_death_f05.xpm",
	NULL,
};

static const char *const	g_f3_boss_walk[] = {
	ENEMY_DIR "boss_floor3/walking/boss_floor3_walking_f00.xpm",
	ENEMY_DIR "boss_floor3/walking/boss_floor3_walking_f01.xpm",
	ENEMY_DIR "boss_floor3/walking/boss_floor3_walking_f02.xpm",
	ENEMY_DIR "boss_floor3/walking/boss_floor3_walking_f03.xpm",
	NULL,
};

static const char *const	g_f3_boss_fire[] = {
	ENEMY_DIR "boss_floor3/firing/boss_floor3_firing_f00.xpm",
	ENEMY_DIR "boss_floor3/firing/boss_floor3_firing_f01.xpm",
	ENEMY_DIR "boss_floor3/firing/boss_floor3_firing_f02.xpm",
	NULL,
};

static const char *const	g_f3_boss_death[] = {
	ENEMY_DIR "boss_floor3/escaping/boss_floor3_escaping_f00.xpm",
	ENEMY_DIR "boss_floor3/escaping/boss_floor3_escaping_f01.xpm",
	ENEMY_DIR "boss_floor3/escaping/boss_floor3_escaping_f02.xpm",
	ENEMY_DIR "boss_floor3/escaping/boss_floor3_escaping_f03.xpm",
	ENEMY_DIR "boss_floor3/escaping/boss_floor3_escaping_f04.xpm",
	ENEMY_DIR "boss_floor3/escaping/boss_floor3_escaping_f05.xpm",
	NULL,
};

static const char *const	g_sci_walk[] = {
	ENEMY_DIR "scientist_enemy_floor2/walking/"
	"scientist_enemy_floor2_walking_f00.xpm",
	ENEMY_DIR "scientist_enemy_floor2/walking/"
	"scientist_enemy_floor2_walking_f01.xpm",
	ENEMY_DIR "scientist_enemy_floor2/walking/"
	"scientist_enemy_floor2_walking_f02.xpm",
	ENEMY_DIR "scientist_enemy_floor2/walking/"
	"scientist_enemy_floor2_walking_f03.xpm",
	NULL,
};

static const char *const	g_sci_fire[] = {
	ENEMY_DIR "scientist_enemy_floor2/firing/"
	"scientist_enemy_floor2_firing_f00.xpm",
	ENEMY_DIR "scientist_enemy_floor2/firing/"
	"scientist_enemy_floor2_firing_f01.xpm",
	ENEMY_DIR "scientist_enemy_floor2/firing/"
	"scientist_enemy_floor2_firing_f02.xpm",
	NULL,
};

static const char *const	g_sci_death[] = {
	ENEMY_DIR "scientist_enemy_floor2/death/"
	"scientist_enemy_floor2_death_f00.xpm",
	ENEMY_DIR "scientist_enemy_floor2/death/"
	"scientist_enemy_floor2_death_f01.xpm",
	ENEMY_DIR "scientist_enemy_floor2/death/"
	"scientist_enemy_floor2_death_f02.xpm",
	ENEMY_DIR "scientist_enemy_floor2/death/"
	"scientist_enemy_floor2_death_f03.xpm",
	ENEMY_DIR "scientist_enemy_floor2/death/"
	"scientist_enemy_floor2_death_f04.xpm",
	ENEMY_DIR "scientist_enemy_floor2/death/"
	"scientist_enemy_floor2_death_f05.xpm",
	NULL,
};

#define PROJ_DIR "./textures/blake_stone_xpm/sprites/projectiles/"

static const char *const	g_no_proj[] = {NULL};

static const char *const	g_electro_proj[] = {
	PROJ_DIR "electro_ball/electro_ball_f00.xpm",
	PROJ_DIR "electro_ball/electro_ball_f01.xpm",
	PROJ_DIR "electro_ball/electro_ball_f02.xpm",
	PROJ_DIR "electro_ball/electro_ball_f03.xpm",
	PROJ_DIR "electro_ball/electro_ball_f04.xpm",
	PROJ_DIR "electro_ball/electro_ball_f05.xpm",
	NULL,
};

static const char *const	g_spider_proj[] = {
	PROJ_DIR "spider_mutant_goo_blast/spider_mutant_goo_blast_f00.xpm",
	PROJ_DIR "spider_mutant_goo_blast/spider_mutant_goo_blast_f01.xpm",
	PROJ_DIR "spider_mutant_goo_blast/spider_mutant_goo_blast_f02.xpm",
	PROJ_DIR "spider_mutant_goo_blast/spider_mutant_goo_blast_f03.xpm",
	PROJ_DIR "spider_mutant_goo_blast/spider_mutant_goo_blast_f04.xpm",
	PROJ_DIR "spider_mutant_goo_blast/spider_mutant_goo_blast_f05.xpm",
	NULL,
};

static const char *const	g_acid_proj[] = {
	PROJ_DIR "acid_dragon_gunk_shot/acid_dragon_gunk_shot_f00.xpm",
	PROJ_DIR "acid_dragon_gunk_shot/acid_dragon_gunk_shot_f01.xpm",
	PROJ_DIR "acid_dragon_gunk_shot/acid_dragon_gunk_shot_f02.xpm",
	PROJ_DIR "acid_dragon_gunk_shot/acid_dragon_gunk_shot_f03.xpm",
	PROJ_DIR "acid_dragon_gunk_shot/acid_dragon_gunk_shot_f04.xpm",
	PROJ_DIR "acid_dragon_gunk_shot/acid_dragon_gunk_shot_f05.xpm",
	NULL,
};

static const char *const	g_plasma_proj[] = {
	PROJ_DIR "plasma_discharge_grenade/plasma_discharge_grenade_f00.xpm",
	PROJ_DIR "plasma_discharge_grenade/plasma_discharge_grenade_f01.xpm",
	PROJ_DIR "plasma_discharge_grenade/plasma_discharge_grenade_f02.xpm",
	PROJ_DIR "plasma_discharge_grenade/plasma_discharge_grenade_f03.xpm",
	NULL,
};

static const char *const	g_snot_proj[] = {
	PROJ_DIR "snotball/snotball_f00.xpm",
	PROJ_DIR "snotball/snotball_f01.xpm",
	PROJ_DIR "snotball/snotball_f02.xpm",
	PROJ_DIR "snotball/snotball_f03.xpm",
	PROJ_DIR "snotball/snotball_f04.xpm",
	PROJ_DIR "snotball/snotball_f05.xpm",
	NULL,
};

static const char *const	g_beam_ring_proj[] = {
	PROJ_DIR "beam_ring/beam_ring_f00.xpm",
	PROJ_DIR "beam_ring/beam_ring_f01.xpm",
	PROJ_DIR "beam_ring/beam_ring_f02.xpm",
	PROJ_DIR "beam_ring/beam_ring_f03.xpm",
	PROJ_DIR "beam_ring/beam_ring_f04.xpm",
	PROJ_DIR "beam_ring/beam_ring_f05.xpm",
	NULL,
};

static const char *const	*g_f1_walk[ENEMY_ROLES] = {
	g_f1_pod_walk, g_f1_boss_walk, g_sci_walk, g_special_walk,
	g_pod_walk};
static const char *const	*g_f1_fire[ENEMY_ROLES] = {
	g_f1_pod_fire, g_f1_boss_fire, g_sci_fire, g_special_walk,
	g_pod_fire};
static const char *const	*g_f1_death[ENEMY_ROLES] = {
	g_f1_pod_death, g_f1_boss_death, g_sci_death, g_special_death,
	g_pod_death};
static const char *const	*g_f1_proj[ENEMY_ROLES] = {
	g_spider_proj, g_electro_proj, g_beam_ring_proj, g_no_proj, g_acid_proj};

static const char *const	*g_f2_walk[ENEMY_ROLES] = {
	g_f2_pod_walk, g_f2_boss_walk, g_sci_walk, g_special_walk, g_f2_pod_walk};
static const char *const	*g_f2_fire[ENEMY_ROLES] = {
	g_f2_pod_fire, g_f2_boss_fire, g_sci_fire, g_special_walk, g_f2_pod_fire};
static const char *const	*g_f2_death[ENEMY_ROLES] = {
	g_f2_pod_death, g_f2_boss_death, g_sci_death, g_special_death,
	g_f2_pod_death};
static const char *const	*g_f2_proj[ENEMY_ROLES] = {
	g_electro_proj, g_plasma_proj, g_snot_proj, g_no_proj, g_electro_proj};

static const char *const	*g_f3_walk[ENEMY_ROLES] = {
	g_f3_blue_walk, g_f3_boss_walk, g_f3_red_walk, g_special_walk,
	g_f3_blue_walk};
static const char *const	*g_f3_fire[ENEMY_ROLES] = {
	g_f3_blue_fire, g_f3_boss_fire, g_f3_red_fire, g_special_walk,
	g_f3_blue_fire};
static const char *const	*g_f3_death[ENEMY_ROLES] = {
	g_f3_blue_death, g_f3_boss_death, g_f3_red_death, g_special_death,
	g_f3_blue_death};
static const char *const	*g_f3_proj[ENEMY_ROLES] = {
	g_electro_proj, g_beam_ring_proj, g_plasma_proj, g_no_proj,
	g_electro_proj};

static t_floorset			g_floor1 = {
	g_f1_walk, g_f1_fire, g_f1_death, g_f1_proj};
static t_floorset			g_floor2 = {
	g_f2_walk, g_f2_fire, g_f2_death, g_f2_proj};
static t_floorset			g_floor3 = {
	g_f3_walk, g_f3_fire, g_f3_death, g_f3_proj};

/**
 * @brief Returns the per-floor enemy art table for the given floor. Floors 2
 * and 3 use their own lineups; every other value (default 1) falls back to
 * floor 1.
 */
t_floorset	*floor_set(int floor)
{
	if (floor == 2)
		return (&g_floor2);
	if (floor == 3)
		return (&g_floor3);
	return (&g_floor1);
}
