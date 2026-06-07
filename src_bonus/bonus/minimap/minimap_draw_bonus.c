/* SECTION 4 (src_bonus/bonus/minimap_draw_bonus.c): circular-mask drawing
 * primitives for the minimap. Every put_px in the minimap path routes
 * through mm_put_circ, which Euclidean-distance-tests the target pixel
 * against the disc center before writing. */

#include "cub3d_bonus.h"

#define MM_BACK		0x050812
#define MM_GRID		0x1B2030
#define MM_EDGE		0x080A10
#define MM_HILITE	0xDDE8F2

/**
 * @brief Disc-gated pixel write. Exposed because mm_overlay paints the
 * player marker directly through it from minimap_bonus.c.
 */
void	mm_put_circ(t_cub *cub, int x, int y, unsigned int c)
{
	int	lx;
	int	ly;

	lx = x - (MM_X + MM_R);
	ly = y - (MM_Y + MM_R);
	if (lx * lx + ly * ly <= MM_R * MM_R)
		put_px(&cub->img, x, y, c);
}

/**
 * @brief Paints the dark circular base behind the map cells. The cell drawer
 * leaves 1px gutters, so this gives the minimap a cleaner instrument look.
 */
void	mm_backdrop(t_cub *cub)
{
	int	x;
	int	y;

	y = MM_Y - 1;
	while (++y < MM_Y + MM_SIZE)
	{
		x = MM_X - 1;
		while (++x < MM_X + MM_SIZE)
			mm_put_circ(cub, x, y, MM_BACK);
	}
}

/**
 * @brief Fills one MM_CELL-sized square at (px, py), gated by the disc
 * mask so corners outside the circle silently drop.
 */
void	mm_square(t_cub *cub, int px, int py, unsigned int color)
{
	int	i;
	int	j;
	int	gutter;

	i = -1;
	while (++i < MM_CELL)
	{
		j = -1;
		while (++j < MM_CELL)
		{
			gutter = (i == 0 || j == 0);
			if (gutter)
				mm_put_circ(cub, px + j, py + i, MM_GRID);
			else
				mm_put_circ(cub, px + j, py + i, color);
		}
	}
}

void	mm_dot(t_cub *cub, int dot[3], unsigned int color)
{
	int	i;
	int	j;

	i = -dot[2] - 1;
	while (++i <= dot[2])
	{
		j = -dot[2] - 1;
		while (++j <= dot[2])
			if (i * i + j * j <= dot[2] * dot[2])
				mm_put_circ(cub, dot[0] + j, dot[1] + i, color);
	}
}

/**
 * @brief Paints a dark outer rim plus light inner rim. Drawn after the cell
 * grid + overlay so the circle edge stays crisp.
 */
void	mm_outline(t_cub *cub)
{
	int	x;
	int	y;
	int	lx;
	int	ly;
	int	d2;

	y = MM_Y - 1;
	while (++y < MM_Y + MM_SIZE)
	{
		x = MM_X - 1;
		while (++x < MM_X + MM_SIZE)
		{
			lx = x - (MM_X + MM_R);
			ly = y - (MM_Y + MM_R);
			d2 = lx * lx + ly * ly;
			if (d2 <= MM_R * MM_R
				&& d2 > (MM_R - MM_RING_THICK) * (MM_R - MM_RING_THICK))
				put_px(&cub->img, x, y, MM_EDGE);
			else if (d2 <= (MM_R - MM_RING_THICK) * (MM_R - MM_RING_THICK)
				&& d2 > (MM_R - MM_RING_THICK - 2)
				* (MM_R - MM_RING_THICK - 2))
				put_px(&cub->img, x, y, MM_HILITE);
		}
	}
}
