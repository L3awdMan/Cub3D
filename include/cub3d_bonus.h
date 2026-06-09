/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/21 16:28:27 by baelgadi          #+#    #+#             */
/*   Updated: 2026/04/12 19:10:21 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_BONUS_H
# define CUB3D_BONUS_H

# include "libft.h"
# include "get_next_line.h"
# include "keys_bonus.h"
# include <math.h>
# include <fcntl.h>
# include <stdio.h>
# include <sys/time.h>

# define WIN_W		1280
# define WIN_H		720

# define TEX_NO		0
# define TEX_SO		1
# define TEX_WE		2
# define TEX_EA		3
# define C_FLOOR	4
# define C_CEIL		5
# define ELEM_FLOOR	6
# define TEX_COUNT	4
# define WALL_CHARS	"1456"
# define WALL_ALT_COUNT	3

# define FLAG_ALL	63

# define MOVE_SPD	0.07
# define ROT_SPD	0.1
# define COLLISION	0.2

# define CASTING	1

/* SECTION 3 (mandatory polish, include/cub3d.h) — depth shading + head bob.
 * All whitelist-safe: only <math.h> needed at runtime.
 *   FOG_K       distance (cells) at which a wall reaches full brightness;
 *               closer walls are bright, far walls fade toward black.
 *   SIDE_SHADE  multiplier applied when ray->side == 1 (horizontal hits)
 *               so N/S walls read darker than E/W — classic Wolfenstein trick.
 *   BG_FADE     min brightness of floor/ceiling at the horizon (1 - top dim).
 *   BOB_AMP     pixels of vertical horizon sway while walking (head bob).
 *   BOB_STEP    bob phase advance per frame when a WASD key is held. */
# define FOG_K		6.0
# define SIDE_SHADE	0.55
# define BG_FADE	0.45
# define BOB_AMP	5
# define BOB_STEP	0.18

/* SECTION 4 (bonus, include/cub3d_bonus.h) — tunables.
 *   MOUSE_SENS  radians of view rotation per pixel of horizontal mouse
 *               travel. The motion hook tracks the raw pointer delta
 *               (current_x - last_x) without recentering, so this scales
 *               real cursor movement — picking a sensible per-pixel rate
 *               that matches FPS feel without DPI-amplified over-rotation. */
# define MOUSE_SENS	0.0025

/* SECTION 4 (bonus) — minimap. A player-centered top-down patch drawn over
 * the 3D view after the raycast pass.
 *   MM_CELL    pixels per map cell.
 *   MM_RADIUS  cells visible in every direction from the player.
 *   MM_SIZE    full minimap side length in pixels (2 * MM_RADIUS * MM_CELL).
 *   MM_X/MM_Y  top-left screen anchor of the minimap box.
 *   MM_*       cell / marker colors (0xRRGGBB). */
# define MM_CELL	9
# define MM_RADIUS	6
# define MM_SIZE	108
# define MM_X		16
# define MM_Y		80
# define MM_WALL	0x5A5A6E
# define MM_FLOOR	0xC8C8D2
# define MM_VOID	0x141418
# define MM_DOOR	0xC8821E
# define MM_PLAYER	0xFF3030
# define MM_ENEMY	0x24D67E
# define MM_BOSS	0xFFCC33
# define MM_BOSS_RING	0x8C1020
/* SECTION 4 (bonus) — circular minimap mask. MM_R is the disc radius in
 * pixels (half the bounding box). MM_RING / MM_RING_THICK draw a white
 * outline so the disc edge reads cleanly against the 3D scene. */
# define MM_R			54
# define MM_RING		0xFFFFFF
# define MM_RING_THICK	3

/* SECTION 4 (bonus) — doors. Tile 'D' is a door cell. A door is a pure
 * boolean: open (raycaster walks straight through) or closed (rendered as
 * a full wall slice with cub->door_tex). Press KEY_E within DOOR_REACH
 * cells to toggle. A close attempt while the player stands on the door
 * cell is rejected (no crush).
 *   DOOR_REACH           cells ahead KEY_E looks for a door to toggle.
 *   DOOR_FACE_PATH       XPM sampled as the closed door panel texture.
 *   DOOR_OPEN_FACE_PATH  XPM sampled while the panel is sliding. */
# define DOOR_REACH		1.5
# define DOOR_FACE_PATH	"./textures/blake_stone_xpm/walls/wall_r15_c01.xpm"
# define DOOR_OPEN_FACE_PATH \
	"./textures/blake_stone_xpm/walls/wall_r15_c01.xpm"
/* SECTION 4 (bonus) — door slide animation. progress runs 0.0 (fully
 * closed) → 1.0 (fully retracted). DOOR_SPEED is the per-ms rate, so
 * 0.0018 ≈ 555 ms for a full open/close. DOOR_PASSABLE is the progress
 * threshold above which collision/movement treats the cell as walkable. */
# define DOOR_SPEED		0.0018
# define DOOR_PASSABLE	0.90

/* SECTION 4 (bonus) — animated billboard sprites. Map tiles '2' and '3' spawn
 * the floor-1 enemy and boss respectively. Each type has its own XPM frame
 * sequence cycling every ANIM_MS milliseconds (wall clock, gettimeofday-based).
 * Tiles are rewritten to '0' after parsing so the raycaster/movement never see
 * them.
 *   ANIM_MS     ms between sprite frames.
 *   SP_NEAR     min camera-space depth to bother drawing (avoids /0).
 *   SP_TYPES    number of distinct sprite types (5 enemies + 4 pickups +
 *               5 death sequences).
 *   MAX_FRAMES  upper bound on frames per type.
 *   SP_POD/SP_FLUID/SP_SCIENTIST/SP_SPECIAL/SP_POD_ALIEN alive type indices
 *               used by t_sprite::type.
 *   SP_PICKUP   first pickup type index; SP_PICKUP+i is the i-th PICKUP_CHARS
 *               weapon pickup (single-frame ammo XPM), rendered grounded.
 *   SP_*_DEATH  play-once death frame sets; death_type() maps an alive enemy
 *               type to its corpse animation.
 *   PICKUP_SCALE  pickup billboard height as a fraction of a full cell, so
 *               ammo rests small on the floor instead of filling the column.
 *   SP_ALIVE/SP_DYING  t_sprite::state values. ALIVE loops the walk anim;
 *               DYING plays the death set once (clamped on the last frame =
 *               persistent corpse). Set by shoot_hitscan() on a shot.
 *   DEATH_MS    ms per death frame (snappier than ANIM_MS, Blake Stone feel).
 *   SHOT_RANGE  max camera-space depth a shot can kill at.
 *   SHOT_AIM    half-width (px) of the centered aim band a target sits in. */
# define ANIM_MS		150
# define SP_NEAR		0.2
# define SP_TYPES		26
# define MAX_FRAMES		10
# define ENEMY_ROLES	5
# define SP_POD			0
# define SP_FLUID		1
# define SP_SCIENTIST	2
# define SP_SPECIAL		3
# define SP_POD_ALIEN	4
# define SP_PICKUP		5
# define SP_POD_DEATH	9
# define SP_FLUID_DEATH	10
# define SP_SCIENTIST_DEATH	11
# define SP_SPECIAL_DEATH	12
# define SP_POD_ALIEN_DEATH	13
# define SP_POD_FIRING	14
# define SP_FLUID_FIRING	15
# define SP_SCIENTIST_FIRING	16
# define SP_SPECIAL_FIRING	17
# define SP_POD_ALIEN_FIRING	18
# define SP_POD_PROJ	19
# define SP_FLUID_PROJ	20
# define SP_SCIENTIST_PROJ	21
# define SP_SPECIAL_PROJ	22
# define SP_POD_ALIEN_PROJ	23
# define SP_MEAT		24
# define SP_MONEY		25
# define SP_ALIVE		0
# define SP_DYING		1
# define SP_ATTACKING	2
# define DEATH_MS		90
# define BOSS_VANISH_AFTER_DEATH	3000
# define SHOT_RANGE		15.0
# define SHOT_AIM		60
# define PICKUP_SCALE	0.40
/* SECTION 4 (bonus) — world reward pickups. A killed enemy drops a meat
 * (SP_MEAT) and a dead boss drops a money bag (SP_MONEY) as grounded billboard
 * sprites. Walking within REWARD_PICK_DIST cells collects it: meat restores
 * MEAT_HEAL hp + a green pickup vignette, money is cosmetic + a gold vignette.
 *   FLASH_FADE_MS / FLASH_MAX_A  pickup edge flash timing/peak alpha.
 *   FLASH_EDGE_PX                border width affected by pickup flashes.
 *   FLASH_GREEN / FLASH_GOLD     target tint colors (0xRRGGBB). */
# define MEAT_HEAL		25
# define REWARD_PICK_DIST	0.6
# define FLASH_FADE_MS	600
# define FLASH_MAX_A	0.35
# define FLASH_EDGE_PX	110
# define FLASH_GREEN	0x40C878
# define FLASH_GOLD		0xE8C34A

/* SECTION 4 (bonus) — enemy combat + per-floor art.
 *   ENEMY_ROLES   number of alive enemy roles (0..4). death_type(t)=9+t and
 *                 firing_type(t)=14+t map a role to its corpse / attack set.
 *   BOSS_HP       shots the SP_FLUID boss soaks; every other role dies in one.
 *   PLAYER_HP_MAX starting player health (HUD feel only — no game-over yet).
 *   ATTACK_RANGE  cells within which an enemy with clear LoS can hit you.
 *   ATTACK_DMG    hp removed per enemy attack tick (player hp clamps at 0).
 *   ATTACK_CD_MS  cooldown (ms) between an enemy's attacks.
 *   FIRE_MS       how long the firing (SP_ATTACKING) set plays before the
 *                 enemy snaps back to SP_ALIVE.
 *   ENEMY_SPD     cells/frame an enemy walks toward the player (< MOVE_SPD).
 *   CHASE_RANGE   cells within which a seen enemy starts walking in.
 *   ENEMY_STOP_DIST  stop closing once this near (keeps attack distance).
 *   ENEMY_PAD     collision half-box for the enemy body.
 *   HURT_FADE_MS  ms for the red hit-flash to fade back to clear.
 *   HURT_MAX_A    peak red alpha at the screen edge right after a hit. */
# define BOSS_HP		8
# define PLAYER_HP_MAX	100
# define ATTACK_RANGE	6.0
# define ATTACK_DMG		6
# define ATTACK_CD_MS	900
# define FIRE_MS		270
# define ENEMY_SPD		0.02
# define CHASE_RANGE		8.0
# define ENEMY_STOP_DIST	1.0
# define ENEMY_PAD		0.3
# define HURT_FADE_MS	600
# define HURT_MAX_A		0.6

/* SECTION 4 (bonus) — "enemies remaining" lives counter. 24 panel frames
 * (lives_0..lives_23) render inside the black slot of the floor HUD. lives
 * starts at the floor's killable enemy count (capped LIVES_MAX) and ticks
 * down by one on each kill. */
# define LIVES_MAX		23
# define LIVES_DIR		"./textures/blake_stone_xpm/sprites/lives_counter/"

/* SECTION 4 (bonus) — end-game state machine. game_state is GS_PLAYING until
 * the player dies (GS_DEAD), beats the boss and grabs its money bag (GS_WIN),
 * or a full-screen mission cutscene is playing (GS_CUTSCENE). The dead UI
 * plays DEAD_FRAMES blur->clear frames at DEAD_ANIM_MS each, then Enter/Space
 * respawns (dead) or advances a floor (win). OPAQUE_KEY is a sentinel
 * transparent key (top byte set) so blit_scaled never skips a pixel. */
# define GS_PLAYING		0
# define GS_DEAD		1
# define GS_WIN			2
# define GS_CUTSCENE	3
/* SECTION 4 (bonus) — front-end UI flow. The game opens on GS_INTRO; Space or
 * Enter steps to GS_MENU (3 options), which branches to GS_SETTINGS /
 * GS_DIFFICULTY or starts the run via GS_FADEOUT (intro clear->blur->black, then
 * the floor-1 cutscene). MENU_OPTS menu rows; FADE_FRAMES blur frames at
 * FADE_ANIM_MS each. difficulty (DIFF_*) scales player hp loss (player_dmg). */
# define GS_INTRO		4
# define GS_MENU		5
# define GS_SETTINGS	6
# define GS_DIFFICULTY	7
# define GS_FADEOUT		8
# define GS_PAUSE		9
# define GS_MISSION		10
# define MENU_OPTS		3
# define PAUSE_OPTS		3
# define MISSION_COUNT	3
# define MISSION_DELAY_MS	1000
# define FADE_FRAMES	6
# define FADE_ANIM_MS	110
# define DIFF_EASY		0
# define DIFF_SKILLED	1
# define DIFF_GIGACHAD	2
# define UI_DIR			"./textures/blake_stone_xpm/ui_screens/"
# define MISSION_DIR	"./textures/blake_stone_xpm/mission_ui/"
# define DEAD_FRAMES	6
# define DEAD_ANIM_MS	120
# define GAMEOVER_RETRY	0
# define GAMEOVER_QUIT	1
# define OPAQUE_KEY		0x01000000u
# define ENDGAME_DIR	"./textures/blake_stone_xpm/sprites/gameover_win_ui/"
/* SECTION 4 (bonus) — full-screen cutscenes. PNG originals are converted to
 * gapless XPM sequences under CUT_DIR and played in numeric order. */
# define CUT_DIR		"./textures/blake_stone_xpm/cutscenes/"
# define CUT_BEGIN_F1	1
# define CUT_END_F2		2
# define CUT_END_F3		3
# define CUT_MAX_FRAMES	122
# define CUT_FRAME_MS	80
# define CUT_BEGIN_MS	120
# define CUT_ACT_PLAY	1
# define CUT_ACT_NEXT	2
# define CUT_ACT_FINAL	3

/* SECTION 4 (bonus) — enemy projectiles. When an enemy fires it spawns an
 * animated shot (per-role proj art) that travels toward the player and deals
 * ATTACK_DMG on impact (no more instant hitscan from the AI).
 * The shot art is per (floor, role): proj_type(role) = SP_POD_PROJ + role names
 * the sprite_anims slot, filled per floor by the t_floorset proj table. A role
 * with an empty proj list (the fluid alien) attacks invisibly (instant damage).
 *   MAX_PROJ       size of the fixed projectile pool (active flag per slot).
 *   PROJ_SPD       cells advanced per frame (snappy; player MOVE_SPD is 0.05).
 *   PROJ_HIT_DIST  cell radius around the player counted as a hit.
 *   PROJ_SCALE     projectile billboard size as a fraction of the full cell. */
# define MAX_PROJ		32
# define PROJ_SPD		0.18
# define PROJ_HIT_DIST	0.4
# define PROJ_SCALE		0.5

/* SECTION 4 (bonus) — textured floor / ceiling (Blake Stone tiles).
 * The mandatory F/C lines in the .cub file still parse to RGB (kept as a
 * fallback color), but the bonus renderer samples these XPMs instead via
 * per-pixel floor casting. 64x64 power-of-two textures so the wrap mask
 * `& (tw - 1)` stays valid. */
# define FLOOR_PATH \
	"./textures/blake_stone_xpm/floors_ceilings/floor_r00_c00.xpm"
# define CEIL_PATH \
	"./textures/blake_stone_xpm/floors_ceilings/floor_r02_c00.xpm"
# define BOSS_CEIL_PATH \
	"./textures/blake_stone_xpm/floors_ceilings/floor_r07_c03.xpm"
# define WALL_ALT0_PATH	"./textures/blake_stone_xpm/walls/wall_r12_c02.xpm"
# define WALL_ALT1_PATH	"./textures/blake_stone_xpm/walls/wall_r10_c06.xpm"
# define WALL_ALT2_PATH	"./textures/blake_stone_xpm/walls/wall_r07_c02.xpm"

/* SECTION 4 (bonus) — weapon HUD. Each weapon has five player-view XPM frames
 * (frame 0 = idle, later frames = firing/muzzle-flash sequence). Firing
 * (KEY_SPACE or left-click) plays the sequence, then returns to idle.
 *   WPN_COUNT     number of weapons (index 0 = starting pistol).
 *   WPN_FRAMES    number of HUD animation frames per weapon.
 *   WPN_SCALE     weapon width as a fraction of WIN_W (centered, bottom).
 *   WPN_FRAME_MS  how long each firing animation frame shows.
 *   WPN_FIRE_MS   total shot animation time.
 *   PICKUP_CHARS  map tiles that swap to weapons 2..5 on walk-over. */
# define WPN_COUNT		5
# define WPN_FRAMES		5
# define WPN_SCALE		0.18
# define WPN_FRAME_MS	45
# define WPN_FIRE_MS	225
# define PICKUP_CHARS	"789P"

/**
 * @brief MinilibX image wrapper (img handle, pixel buffer, dimensions)
 */
typedef struct s_img
{
	void	*id;
	char	*data;
	int		bpp;
	int		line_len;
	int		endian;
	int		width;
	int		height;
}	t_img;

/**
 * @brief Player state (position, direction and camera plane)
 * 
 * @note dir is the look direction and plane is the camera plane perpendicular
 * to it (defines the FOV)
 */
typedef struct s_player
{
	double	pos_x;
	double	pos_y;
	double	dir_x;
	double	dir_y;
	double	plane_x;
	double	plane_y;
	int		hp;
	long	hurt_ms;
}	t_player;

/**
 * @brief Map data and parsed configuration
 * 
 * @note parsed_flags uses a bitmask: 0-3=textures, 4=floor and 5=ceiling
 */
typedef struct s_map
{
	char	**grid;
	int		*row_len;
	int		width;
	int		height;
	char	*tex_path[4];
	int		floor_col;
	int		ceil_col;
	int		parsed_flags;
}	t_map;

/**
 * @brief Ray state during DDA traversal
 */
typedef struct s_ray
{
	double	dir_x;
	double	dir_y;
	int		map_x;
	int		map_y;
	double	side_dist_x;
	double	side_dist_y;
	double	delta_x;
	double	delta_y;
	double	perp_dist;
	int		step_x;
	int		step_y;
	int		side;
	/* SECTION 4: 1 if the DDA stopped on a closed/animating door cell;
	 * draw_wall_stripe routes those columns to the door drawer. */
	int		door;
	/* SECTION 4: fractional hit position [0..1] across the door face on
	 * the cell where the DDA stopped. Used by the door drawer to warp
	 * wall_x so the panel texture compresses to the unopened side as the
	 * door slides open. Undefined when ray->door is 0. */
	double	door_hit;
}	t_ray;

/**
 * @brief SECTION 4 (include/cub3d_bonus.h): per-cell door state.
 *   progress  0.0 = fully closed, 1.0 = fully retracted. Animated each
 *             frame by update_doors() toward `target` at DOOR_SPEED ms⁻¹.
 *   target    0 = closing, 1 = opening. toggle_door flips this.
 */
typedef struct s_door
{
	double	progress;
	int		target;
}	t_door;

/**
 * @brief SECTION 4 (include/cub3d_bonus.h): an in-flight enemy projectile.
 *   x, y      world position (cell coordinates), advanced each frame.
 *   dx, dy    per-frame velocity (unit direction toward the player at spawn
 *             time, scaled by PROJ_SPD).
 *   type      sprite_anims slot for this shot's art (proj_type of the firing
 *             enemy's role), so each enemy/boss flies its own projectile.
 *   active    1 while the shot is travelling; cleared on wall/player impact so
 *             the pool slot can be reused. The pool is a fixed t_cub array.
 */
typedef struct s_proj
{
	double	x;
	double	y;
	double	dx;
	double	dy;
	int		type;
	int		active;
}	t_proj;

/**
 * @brief SECTION 4 (include/cub3d_bonus.h): a billboard sprite entity.
 *   x, y      world position (cell coordinates, cell-centered).
 *   type      enemy / boss / SP_PICKUP+ — picks the animation set.
 *   state     SP_ALIVE (loop walk), SP_DYING (death set once) or
 *             SP_ATTACKING (firing set once, then back to SP_ALIVE).
 *   hp        shots left before death (boss = BOSS_HP, others = 1).
 *   death_ms  wall-clock (now_ms) when the death / firing anim started.
 *   next_attack_ms  earliest wall-clock at which this enemy may attack again.
 */
typedef struct s_sprite
{
	double	x;
	double	y;
	int		type;
	int		state;
	int		hp;
	long	death_ms;
	long	next_attack_ms;
}	t_sprite;

/**
 * @brief SECTION 4 (include/cub3d_bonus.h): one animated sprite type. Frames
 * are XPM images loaded once at startup; transp[i] holds the per-frame None-
 * pixel color (sampled from frame's (0,0)) so the column blitter can skip it.
 */
typedef struct s_anim
{
	t_img			frames[MAX_FRAMES];
	unsigned int	transp[MAX_FRAMES];
	int				count;
}	t_anim;

/**
 * @brief SECTION 4 (include/cub3d_bonus.h): active full-screen cutscene.
 * Frames are lazily loaded as one XPM sequence when a cutscene starts, scaled
 * to WIN_W x WIN_H by draw_cutscene(), then freed after the sequence finishes.
 */
typedef struct s_cutscene
{
	t_img	frames[CUT_MAX_FRAMES];
	int		id;
	int		count;
	int		frame;
	int		frame_ms;
	int		next_action;
	long	start_ms;
	int		final_done;
}	t_cutscene;

/**
 * @brief SECTION 4 (include/cub3d_bonus.h): per-floor enemy art table. For
 * each of the ENEMY_ROLES alive roles it holds four NULL-terminated XPM path
 * lists — walk loop, firing (attack) set, play-once death set and the flying
 * projectile art (empty list = invisible attack). floor_set() picks the table
 * for cub->floor; load_anim_sprites() fills the matching sprite_anims slots
 * (role, firing_type(role), death_type(role), proj_type(role)).
 */
typedef struct s_floorset
{
	const char *const	**walk;
	const char *const	**fire;
	const char *const	**death;
	const char *const	**proj;
}	t_floorset;

/**
 * @brief SECTION 4 (include/cub3d_bonus.h): per-floor texture theme. Holds the
 * XPM paths that are otherwise global hardcoded macros (door, floor, ceiling,
 * boss-ceiling and the three accent walls 4/5/6). floor_theme() returns the set
 * for cub->floor so a floor can re-skin its walls/ceiling/doors; floors 1 and 2
 * fall back to the original macro paths.
 */
typedef struct s_theme
{
	char	*door;
	char	*door_open;
	char	*floor;
	char	*ceil;
	char	*boss_ceil;
	char	*alt[3];
}	t_theme;

/**
 * @brief SECTION 4 (include/cub3d_bonus.h): weapon HUD state.
 *   img / transp  WPN_COUNT x WPN_FRAMES HUD images and their per-frame
 *                 None-pixel key (sampled from the XPM (0,0) corner).
 *   current       active weapon index (0 = pistol; pickups set 1..4).
 *   shoot         set by the left-click button hook, consumed by update_weapon.
 *   firing        1 while the fire-pose frame is shown, 0 for the idle frame.
 *   fire_ms       wall-clock when the current shot started (fire-frame timer).
 */
typedef struct s_weapon
{
	t_img			img[WPN_COUNT][WPN_FRAMES];
	unsigned int	transp[WPN_COUNT][WPN_FRAMES];
	int				current;
	int				shoot;
	int				firing;
	long			fire_ms;
}	t_weapon;

/**
 * @brief SECTION 4 (include/cub3d_bonus.h): screen-space draw parameters for
 * one sprite, produced by sprite_transform() and consumed by the column
 * drawer. depth is camera-space distance (used for the z-buffer test).
 */
typedef struct s_spr
{
	double			depth;
	int				screen_x;
	int				size;
	int				sx0;
	int				sx1;
	int				sy0;
	int				sy1;
	double			shade;
	t_img			*tex;
	unsigned int	transp;
}	t_spr;

/**
 * @brief Drawing parameters for a single wall column (computed from ray hit)
 */
typedef struct s_draw
{
	int		height;
	int		start;
	int		end;
	int		tex_x;
	double	step;
	double	pos;
	/* SECTION 3 (include/cub3d.h): per-column brightness multiplier
	 * (fog * side darkening). Computed once in calc_draw_params, applied to
	 * each sampled texel in draw_tex_col. */
	double	shade;
}	t_draw;

typedef struct s_cub
{
	void		*mlx;
	void		*win;
	t_img		img;
	t_img		tex[4];
	t_player	player;
	t_map		map;
	int			keys[65536];
	/* SECTION 3 (include/cub3d.h):
	 *   horizon = current vertical center of projection (WIN_H/2 ± bob).
	 *   bob_t   = head-bob phase, only advanced while moving. */
	int			horizon;
	double		bob_t;
	/* SECTION 4 (include/cub3d_bonus.h): show_minimap toggles the minimap
	 * overlay (KEY_M). Defaults to 1 — set in main() after cub_init(). */
	int			show_minimap;
	/* SECTION 4 (include/cub3d_bonus.h): flat door-state grid, one t_door per
	 * map cell, indexed [y * map.width + x]. NULL until init_doors(). */
	t_door		*doors;
	/* SECTION 4 (include/cub3d_bonus.h): billboard sprites.
	 *   sprites/sprite_count - the entity array parsed from '2' tiles.
	 *   zbuf - per-column wall depth from the raycast pass; sprites draw a
	 *          column only where they are nearer than the wall behind it. */
	t_sprite	*sprites;
	int			sprite_count;
	double		zbuf[WIN_W];
	/* SECTION 4 (include/cub3d_bonus.h): fixed pool of in-flight enemy
	 * projectiles. Zeroed by cub_init (active = 0); spawn_projectile claims a
	 * free slot, update_projectiles flies/expires them, draw_projectiles
	 * billboards the active ones in the sprite pass. */
	t_proj		projectiles[MAX_PROJ];
	/* SECTION 4 (include/cub3d_bonus.h): bonus door panel XPM loaded once
	 * at startup. Sampled by draw_door_stripe for closed/animating doors. */
	t_img		door_tex;
	t_img		door_open_tex;
	/* SECTION 4 (include/cub3d_bonus.h): floor + ceiling tiles. Sampled per
	 * pixel by the floor-cast pass in draw_bg.c. */
	t_img		floor_tex;
	t_img		ceil_tex;
	t_img		boss_ceil_tex;
	t_img		wall_alt[WALL_ALT_COUNT];
	t_img		floor_hud;
	int			floor_hud_transp;
	/* SECTION 4 (include/cub3d_bonus.h): "enemies remaining" lives counter.
	 * lives_hud holds the 24 panel frames (lazy-loaded); lives is the live
	 * count drawn into the floor-HUD black slot; lives_transp is frame 0's
	 * transparent key. */
	t_img		lives_hud[24];
	int			lives;
	int			lives_transp;
	/* SECTION 4 (include/cub3d_bonus.h): end-game state. game_state is GS_*;
	 * state_ms is the now_ms() when DEAD/WIN was entered (drives the dead-anim
	 * frame index + the confirm-input delay). spawn_player/spawn_sprites
	 * snapshot the floor's start so respawn() restores it. dead_ui/win_ui are
	 * the lazily-loaded full-screen overlays. */
	int			game_state;
	long		state_ms;
	t_player	spawn_player;
	t_sprite	*spawn_sprites;
	int			spawn_count;
	t_img		dead_ui[DEAD_FRAMES];
	t_img		win_ui;
	t_img		gameover_retry;
	t_img		gameover_quit;
	int			gameover_sel;
	/* SECTION 4 (include/cub3d_bonus.h): front-end menu. menu_sel is the main-
	 * menu cursor (0..2); difficulty (DIFF_*) is the chosen level. ui_* are the
	 * lazily-loaded full-screen menu screens; ui_fade is the clear->black
	 * Start-Mission blur sequence. */
	int			menu_sel;
	int			pause_sel;
	int			settings_return_state;
	int			difficulty;
	t_img		ui_intro;
	t_img		ui_menu[MENU_OPTS];
	t_img		ui_settings;
	t_img		ui_pause[PAUSE_OPTS];
	t_img		ui_diff[3];
	t_img		ui_fade[FADE_FRAMES];
	t_img		ui_mission[MISSION_COUNT];
	long		mission_due_ms;
	int			mission_pending;
	t_cutscene	cutscene;
	/* SECTION 4 (include/cub3d_bonus.h): world reward pickups. sprite_cap is
	 * the allocated length of the sprites array (> sprite_count) so spawn_reward
	 * can append meat/money at runtime. flash_ms/flash_rgb drive the green/gold
	 * pickup vignette (draw_pickup_flash), mirroring the red damage flash. */
	int			sprite_cap;
	long		flash_ms;
	int			flash_rgb;
	/* SECTION 4 (include/cub3d_bonus.h): animated sprite types (pod / fluid)
	 * cycling every ANIM_MS milliseconds. */
	t_anim		sprite_anims[SP_TYPES];
	/* SECTION 4 (include/cub3d_bonus.h): weapon HUD state. Loaded by
	 * load_weapons() after the world textures; drawn over the 3D view each
	 * frame by draw_weapon(); freed by free_weapons(). */
	t_weapon	weapon;
	/* SECTION 4 (include/cub3d_bonus.h): line returned by get_next_line
	 * currently being dispatched by parse_file. cub_destroy frees it if
	 * an exit_error fires mid-parse so it never leaks. */
	char		*pending_line;
	/* SECTION 4 (include/cub3d_bonus.h): mouse-look state. The motion hook
	 * accumulates raw pointer deltas (no cursor warp) and the per-frame
	 * loop applies the rotation, then resets mouse_dx. mouse_ready stays 0
	 * until the first MotionNotify after focus is (re)gained so the very
	 * first event seeds mouse_last_x without producing a giant rotation
	 * jump from the cursor having moved while the window was unfocused. */
	int			mouse_last_x;
	int			mouse_dx;
	int			mouse_ready;
	/* SECTION 4 (include/cub3d_bonus.h): wall-clock timestamp (ms) of the
	 * last update_doors() call. The next call uses (now - door_last_ms)
	 * as delta-time to advance every door's progress at DOOR_SPEED. */
	long		door_last_ms;
	/* SECTION 4 (include/cub3d_bonus.h): scene floor (1..) set by the bonus
	 * FLOOR directive (default 1). Selects which per-floor enemy/boss art
	 * fills each role slot in load_anim_sprites(). */
	int			floor;
}	t_cub;

/* ────────────── main.c ──────────────── */

void			init_row_len(t_cub *cub);

/* ────────────── cleanup.c ──────────────── */

void			exit_error(t_cub *cub, char *msg);
void			cub_destroy(t_cub *cub);
void			free_grid(t_map *map);
void			free_tex_paths(t_map *map);

/* ────────────── hooks.c ──────────────── */

int				key_press(int key, void *param);
int				key_release(int key, void *param);
int				close_hook(void *param);
void			register_hooks(t_cub *cub);

/* ────────────── movement.c ──────────────── */

void			apply_movement(t_cub *cub);

/* ────────────── rotation.c ──────────────── */

void			apply_rotation(t_cub *cub);
/* SECTION 4 (include/cub3d_bonus.h): exposed for mouse_bonus.c reuse. */
void			rotate_vectors(t_player *p, double angle);

/* ────────────── bonus/mouse_bonus.c ──────────────── */

int				mouse_move(int x, int y, void *param);
int				focus_in(void *param);
int				focus_out(void *param);
void			apply_mouse_look(t_cub *cub);
void			register_mouse(t_cub *cub);

/* ────────────── bonus/minimap_bonus.c ──────────────── */

void			draw_minimap(t_cub *cub);

/* ────────────── bonus/minimap_draw_bonus.c ──────────────── */

void			mm_square(t_cub *cub, int px, int py, unsigned int color);
void			mm_backdrop(t_cub *cub);
void			mm_outline(t_cub *cub);
void			mm_put_circ(t_cub *cub, int x, int y, unsigned int c);
void			mm_dot(t_cub *cub, int dot[3], unsigned int color);

/* ────────────── bonus/doors_bonus.c ──────────────── */

void			init_doors(t_cub *cub);
t_door			*door_at(t_cub *cub, int y, int x);
void			update_doors(t_cub *cub);
double			door_progress_at(t_cub *cub, int y, int x);
int				door_allows_passage(t_cub *cub, int y, int x, double hit);
int				door_is_open(t_cub *cub, int y, int x);

/* ────────────── bonus/door_use_bonus.c ──────────────── */

void			toggle_door(t_cub *cub);

/* ────────────── bonus/door_draw_bonus.c ──────────────── */

void			draw_door_stripe(t_cub *cub, int x, t_ray *ray);

/* ────────────── bonus/sprites_bonus.c ──────────────── */

void			init_sprites(t_cub *cub);
void			drop_pickup_sprite(t_cub *cub, int mx, int my);

/* ────────────── bonus/anim_sprite_bonus.c ──────────────── */

long			now_ms(void);
void			load_anim_sprites(t_cub *cub);
void			free_anim_sprites(t_cub *cub);

/* ────────────── bonus/floor_anims_bonus.c ──────────────── */

t_floorset		*floor_set(int floor);
t_theme			floor_theme(int floor);

/* ────────────── bonus/sprite_col_bonus.c ──────────────── */

void			draw_sprite_cols(t_cub *cub, t_spr *spr);
void			sprite_rect(t_cub *cub, t_sprite *sp, t_spr *s, int full);
void			sort_sprites(t_cub *cub);

/* ────────────── bonus/sprite_draw_bonus.c ──────────────── */

void			draw_sprites(t_cub *cub);
/* SECTION 4: exposed so projectile_draw_bonus.c reuses the camera transform
 * to billboard a projectile via a temporary t_sprite. */
void			sprite_transform(t_cub *cub, t_sprite *sp, t_spr *s);

/* ────────────── bonus/enemy_bonus.c ──────────────── */

int				death_type(int type);
int				firing_type(int type);
void			shoot_hitscan(t_cub *cub);

/* ────────────── bonus/enemy_ai_bonus.c ──────────────── */

void			update_enemies(t_cub *cub);
int				los_clear(t_cub *cub, t_sprite *s);

/* ────────────── bonus/enemy_move_bonus.c ──────────────── */

void			move_enemy(t_cub *cub, t_sprite *s);

/* ────────────── bonus/projectile_bonus.c ──────────────── */

int				proj_type(int type);
void			spawn_projectile(t_cub *cub, t_sprite *s);
void			update_projectiles(t_cub *cub);

/* ────────────── bonus/projectile_draw_bonus.c ──────────────── */

void			draw_projectiles(t_cub *cub);

/* ────────────── bonus/hud_bar_bonus.c ──────────────── */

void			draw_health_bars(t_cub *cub);
void			draw_hud_bar(t_cub *cub, int box[4], double ratio, int boss);

/* ────────────── bonus/damage_flash_bonus.c ──────────────── */

void			draw_damage_flash(t_cub *cub);

/* ────────────── bonus/weapon_bonus.c ──────────────── */

void			load_weapons(t_cub *cub);
void			free_weapons(t_cub *cub);
void			weapon_pickup(t_cub *cub);
int				weapon_fire(int button, int x, int y, void *param);

/* ────────────── bonus/weapon_draw_bonus.c ──────────────── */

void			update_weapon(t_cub *cub);
void			draw_weapon(t_cub *cub);

/* ────────────── bonus/weapon_blit_bonus.c ──────────────── */

void			blit_scaled(t_cub *cub, t_img *src, unsigned int transp,
					int r[8]);
void			blit_full(t_cub *cub, t_img *f, unsigned int transp);
void			frame_bbox(t_img *t, unsigned int transp, int r[8]);

/* ────────────── bonus/floor_hud_bonus.c ──────────────── */

void			load_floor_hud(t_cub *cub);
void			draw_floor_hud(t_cub *cub);

/* ────────────── bonus/lives_hud_bonus.c ──────────────── */

void			load_lives_hud(t_cub *cub);
void			draw_lives_hud(t_cub *cub);
int				count_enemies(t_cub *cub);

/* ────────────── bonus/endgame_bonus.c ──────────────── */

void			capture_spawn(t_cub *cub);
void			load_endgame_ui(t_cub *cub);

/* ────────────── bonus/endgame_state_bonus.c ──────────────── */

void			respawn(t_cub *cub);
void			enter_state(t_cub *cub, int gs);
void			endgame_input(t_cub *cub, int key);
void			update_endgame(t_cub *cub);
void			draw_endgame(t_cub *cub);

/* ────────────── bonus/menu_bonus.c ──────────────── */

void			load_menu_ui(t_cub *cub);
void			draw_menu(t_cub *cub);
void			free_menu_ui(t_cub *cub);

/* ────────────── bonus/menu_frame_bonus.c ──────────────── */

t_img			*menu_frame(t_cub *cub);
void			update_menu(t_cub *cub);

/* ────────────── bonus/menu_input_bonus.c ──────────────── */

int				player_dmg(t_cub *cub);
void			menu_input(t_cub *cub, int key);

/* ────────────── bonus/mission_bonus.c ──────────────── */

void			load_mission_ui(t_cub *cub);
void			free_mission_ui(t_cub *cub);
void			schedule_mission_ui(t_cub *cub);
void			update_mission_ui(t_cub *cub);
void			draw_mission_ui(t_cub *cub);
void			mission_input(t_cub *cub, int key);

/* ────────────── bonus/cutscene_bonus.c ──────────────── */

char			*cutscene_path(int id, int frame);
void			free_cutscene(t_cub *cub);
void			load_cutscene(t_cub *cub, int id);
void			start_cutscene(t_cub *cub, int id, int next_action);
void			update_cutscene(t_cub *cub);
void			draw_cutscene(t_cub *cub);

/* ────────────── bonus/floor_switch_bonus.c ──────────────── */

void			advance_floor(t_cub *cub);
void			advance_floor_after_cutscene(t_cub *cub);
void			restart_game(t_cub *cub);

/* ────────────── bonus/reward_popup_bonus.c ──────────────── */

void			spawn_reward(t_cub *cub, double x, double y, int type);
void			collect_rewards(t_cub *cub);
void			draw_pickup_flash(t_cub *cub);

/* ────────────── ray_utils.c ──────────────── */

void			init_ray(t_ray *ray, t_player *p, int x);
void			init_step_side(t_ray *ray, t_player *p);
/* SECTION 3 (include/cub3d.h): calc_draw_params gained a horizon param so
 * the wall stripe re-centers on the bobbing horizon, and it now fills
 * dw->shade so draw_tex_col can shade fog + side darkening in one pass. */
void			calc_draw_params(t_draw *dw, t_ray *ray, int horizon);
int				select_texture(t_ray *ray);
/* SECTION 4: hit fraction [0..1] across the cell face the DDA is about
 * to enter. Used by the door pass-through test in dda_step. */
double			ray_hit_fraction(t_ray *ray, t_player *p);

/* ────────────── raycaster.c ──────────────── */

void			cast_all_rays(t_cub *cub);

/* ────────────── init_player.c ──────────────── */

void			init_player(t_cub *cub, int y, int x, char c);

/* ────────────── parse_color.c ──────────────── */

int				rgb_to_int(int r, int g, int b);
void			parse_color(t_cub *cub, char *line, int is_ceiling);

/* ────────────── parse_elements.c ──────────────── */

int				identify_element(char *line);
void			parse_texture(t_cub *cub, char *line, int idx);
void			parse_floor(t_cub *cub, char *line);

/* ────────────── parse_file.c ──────────────── */

void			parse_file(t_cub *cub, char *path);

/* ────────────── parse_map.c ──────────────── */

void			store_map_lines(t_cub *cub, char *first, int fd);

/* ────────────── validate_map.c ──────────────── */

void			validate_map(t_cub *cub);

/* ────────────── draw_bg.c ──────────────── */

void			draw_ceiling(t_cub *cub, int x, int end);
void			draw_floor(t_cub *cub, int x, int start);

/* ────────────── draw_wall.c ──────────────── */

void			draw_wall_stripe(t_cub *cub, int x, t_ray *ray);
/* SECTION 4 (include/cub3d_bonus.h): exposed for the door drawer reuse. */
double			calc_wall_x(t_ray *ray, t_player *p);

/* ────────────── render.c ──────────────── */

void			put_px(t_img *img, int x, int y, unsigned int color);
int				loop_hook(void *param);
/* SECTION 3 (include/cub3d.h): shared render helpers.
 *   shade_rgb     - per-channel RGB multiply with 0..1 saturation clamp.
 *   draw_crosshair- 9-pixel HUD cross drawn after the raycast pass.
 *   update_horizon- per-frame head-bob update from held WASD keys. */
unsigned int	shade_rgb(unsigned int color, double k);
void			draw_crosshair(t_cub *cub);
void			update_horizon(t_cub *cub);

/* ────────────── texture.c ──────────────── */

unsigned int	get_tex_pixel(t_img *tex, int x, int y);
void			load_textures(t_cub *cub);
void			free_textures(t_cub *cub);

#endif
