#include "cub3d_bonus.h"

/**
 * @brief Sets direction for N (north) or S (south) spawn
 */
static void	set_dir_ns(t_player *p, char c)
{
	if (c == 'N')
	{
		p->dir_y = -1;
		p->plane_x = 0.66;
	}
	else
	{
		p->dir_y = 1;
		p->plane_x = -0.66;
	}
}

/**
 * @brief Sets direction for W (west) or E (east) spawn
 */
static void	set_dir_we(t_player *p, char c)
{
	if (c == 'W')
	{
		p->dir_x = -1;
		p->plane_y = -0.66;
	}
	else
	{
		p->dir_x = 1;
		p->plane_y = 0.66;
	}
}

/**
 * @brief Sets player position, direction and camera plane from spawn
 * @details Centers the player in the cell (+ 0.5) and sets dir/plane based on
 * the spawn character (N/S/E/W)
 */
void	init_player(t_cub *cub, int y, int x, char c)
{
	cub->player.pos_x = (double)x + 0.5;
	cub->player.pos_y = (double)y + 0.5;
	cub->player.dir_x = 0;
	cub->player.dir_y = 0;
	cub->player.plane_x = 0;
	cub->player.plane_y = 0;
	if (c == 'N' || c == 'S')
		set_dir_ns(&cub->player, c);
	else
		set_dir_we(&cub->player, c);
}
