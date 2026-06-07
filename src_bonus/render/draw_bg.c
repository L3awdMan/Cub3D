/* SECTION 4 (src_bonus/render/draw_bg.c): textured floor + ceiling via
 * per-pixel floor casting. For each background pixel we recover the world
 * point it covers from the row's perspective distance, sample the floor or
 * ceiling XPM at that world (x, y), and shade by depth (FOG_K). The mandatory
 * F/C RGB colors stay parsed but are no longer drawn — the bonus map looks
 * like a Blake-Stone-style textured corridor. */

#include "cub3d_bonus.h"

/**
 * @brief SECTION 4 (src_bonus/render/draw_bg.c): samples the floor or ceiling
 * texture for screen pixel (x, y). Reconstructs the row's perspective
 * distance from cub->horizon, projects (x, y) into world space along this
 * column's ray, and returns the texel at the wrapped world coords with depth
 * fog applied. Textures must be power-of-two wide for the `& (tw - 1)` wrap.
 */
static int	is_boss_room(t_cub *cub, double x, double y)
{
	int	mx;
	int	my;

	mx = (int)x;
	my = (int)y;
	if (my < 0 || my >= cub->map.height)
		return (0);
	if (mx < 0 || mx >= cub->map.row_len[my])
		return (0);
	return (cub->map.grid[my][mx] == 'Z');
}

static unsigned int	sample_bg(t_cub *cub, int x, int y, int is_floor)
{
	t_img		*tex;
	double		rd;
	double		fx;
	double		fy;
	int			t[2];

	if (is_floor)
		rd = (double)(WIN_H / 2) / (y - cub->horizon);
	else
		rd = (double)(WIN_H / 2) / (cub->horizon - y);
	fx = cub->player.pos_x + rd * (cub->player.dir_x
			+ cub->player.plane_x * (2.0 * x / WIN_W - 1.0));
	fy = cub->player.pos_y + rd * (cub->player.dir_y
			+ cub->player.plane_y * (2.0 * x / WIN_W - 1.0));
	tex = &cub->floor_tex;
	if (!is_floor)
	{
		tex = &cub->ceil_tex;
		if (is_boss_room(cub, fx, fy))
			tex = &cub->boss_ceil_tex;
	}
	t[0] = ((int)(tex->width * (fx - floor(fx)))) & (tex->width - 1);
	t[1] = ((int)(tex->width * (fy - floor(fy)))) & (tex->width - 1);
	return (shade_rgb(get_tex_pixel(tex, t[0], t[1]), FOG_K / (FOG_K + rd)));
}

/**
 * @brief Draws ceiling pixels for column x from y = 0 to end. Each row above
 * cub->horizon is sampled from cub->ceil_tex via sample_bg. The y == horizon
 * row is skipped (division-by-zero guard).
 */
void	draw_ceiling(t_cub *cub, int x, int end)
{
	int	y;

	y = 0;
	while (y < end)
	{
		if (y < cub->horizon)
			put_px(&cub->img, x, y, sample_bg(cub, x, y, 0));
		y++;
	}
}

/**
 * @brief Draws floor pixels for column x from start to WIN_H. Each row below
 * cub->horizon is sampled from cub->floor_tex via sample_bg.
 */
void	draw_floor(t_cub *cub, int x, int start)
{
	int	y;

	y = start;
	while (y < WIN_H)
	{
		if (y > cub->horizon)
			put_px(&cub->img, x, y, sample_bg(cub, x, y, 1));
		y++;
	}
}
