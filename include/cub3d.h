/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/21 16:28:27 by baelgadi          #+#    #+#             */
/*   Updated: 2026/06/20 18:23:31 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include "libft.h"
# include "get_next_line.h"
# include "keys.h"
# include <math.h>
# include <fcntl.h>
# include <stdio.h>

# define WIN_W		1280
# define WIN_H		720

# define TEX_NO		0
# define TEX_SO		1
# define TEX_WE		2
# define TEX_EA		3
# define C_FLOOR	4
# define C_CEIL		5
# define TEX_COUNT	4

# define FLAG_ALL	63

# define MOVE_SPD	0.05
# define ROT_SPD	0.03
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
}	t_player;

/**
 * @brief Map data and parsed configuration
 * 
 * @note parsed_flags uses a bitmask: 0-3=textures, 4=floor and 5=ceiling
 */
typedef struct s_map
{
	char	**grid;
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
}	t_ray;

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

/**
 * @brief Main cub struct
 * 
 * @note parse_fd is -1 if no fd is owned and >= 0 otherwise
 */
typedef struct s_cub
{
	void		*mlx;
	void		*win;
	t_img		img;
	t_img		tex[4];
	t_player	player;
	t_map		map;
	int			keys[65536];
	int			parse_fd;
	/* SECTION 3 (include/cub3d.h):
	 *   horizon = current vertical center of projection (WIN_H/2 ± bob).
	 *   bob_t   = head-bob phase, only advanced while moving. */
	int			horizon;
	double		bob_t;
}	t_cub;

/* ────────────── cleanup.c ──────────────── */

void			exit_error(t_cub *cub, char *msg);
void			cub_destroy(t_cub *cub);

/* ────────────── hooks.c ──────────────── */

int				key_press(int key, void *param);
int				key_release(int key, void *param);
int				close_hook(void *param);
void			register_hooks(t_cub *cub);

/* ────────────── movement.c ──────────────── */

void			apply_movement(t_cub *cub);

/* ────────────── rotation.c ──────────────── */

void			apply_rotation(t_cub *cub);

/* ────────────── ray_utils.c ──────────────── */

void			init_ray(t_ray *ray, t_player *p, int x);
void			init_step_side(t_ray *ray, t_player *p);
/* SECTION 3 (include/cub3d.h): calc_draw_params gained a horizon param so
 * the wall stripe re-centers on the bobbing horizon, and it now fills
 * dw->shade so draw_tex_col can shade fog + side darkening in one pass. */
void			calc_draw_params(t_draw *dw, t_ray *ray, int horizon);
int				select_texture(t_ray *ray);

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
