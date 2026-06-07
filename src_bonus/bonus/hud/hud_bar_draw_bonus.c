#include "cub3d_bonus.h"

static void	set_box(int box[4], int x, int y, int size[2])
{
	box[0] = x;
	box[1] = y;
	box[2] = size[0];
	box[3] = size[1];
}

static void	fill_rect(t_cub *cub, int box[4], unsigned int color)
{
	int	x;
	int	y;

	y = -1;
	while (++y < box[3])
	{
		x = -1;
		while (++x < box[2])
			put_px(&cub->img, box[0] + x, box[1] + y, color);
	}
}

static unsigned int	bar_color(double ratio, int boss)
{
	if (boss)
	{
		if (ratio <= 0.25)
			return (0xFF6A2A);
		if (ratio <= 0.50)
			return (0xE69A24);
		return (0xC8333C);
	}
	if (ratio <= 0.25)
		return (0xD83232);
	if (ratio <= 0.50)
		return (0xD8B22A);
	return (0x35C85A);
}

void	draw_hud_bar(t_cub *cub, int box[4], double ratio, int boss)
{
	int	size[2];
	int	fill[4];

	if (ratio < 0)
		ratio = 0;
	if (ratio > 1)
		ratio = 1;
	fill_rect(cub, box, 0x171820);
	size[0] = (int)(box[2] * ratio);
	size[1] = box[3];
	set_box(fill, box[0], box[1], size);
	fill_rect(cub, fill, bar_color(ratio, boss));
}
