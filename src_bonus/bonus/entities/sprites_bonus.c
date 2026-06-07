/* SECTION 4 (src_bonus/bonus/sprites_bonus.c): map-driven sprite spawn.
 * Map tiles '2' and '3' mark the floor-1 enemy and boss respectively. Each
 * tile becomes a t_sprite entry (cell-centered position + type index) and is
 * rewritten to '0' so the raycaster and movement code only ever see plain
 * floor. Per-frame animation is wall-clock driven from now_ms() and lives in
 * sprite_draw_bonus.c. */

#include "cub3d_bonus.h"

/**
 * @brief Maps a map character to a sprite type index, or -1 if the char is
 * not a sprite tile. '2'/'3'/'A' are robot/boss/scientist, 'F' is the
 * every-floor special fluid alien, and 'B' is the floor-1 pod alien.
 * PICKUP_CHARS map to SP_PICKUP + index so each pickup gets one frame.
 */
static int	tile_type(char c)
{
	static const char	picks[] = PICKUP_CHARS;
	const char			*p;

	if (!c)
		return (-1);
	if (c == '2')
		return (SP_POD);
	if (c == '3')
		return (SP_FLUID);
	if (c == 'A')
		return (SP_SCIENTIST);
	if (c == 'F')
		return (SP_SPECIAL);
	if (c == 'B')
		return (SP_POD_ALIEN);
	p = ft_strchr(picks, c);
	if (p)
		return (SP_PICKUP + (int)(p - picks));
	return (-1);
}

/**
 * @brief Counts every sprite tile ('2' / '3') across the grid.
 */
static int	count_sprites(t_cub *cub)
{
	int	y;
	int	x;
	int	n;

	n = 0;
	y = -1;
	while (++y < cub->map.height)
	{
		x = -1;
		while (cub->map.grid[y][++x])
			if (tile_type(cub->map.grid[y][x]) >= 0)
				n++;
	}
	return (n);
}

/**
 * @brief Records every sprite tile as a cell-centered t_sprite with its type
 * set from the tile character. Enemy tiles are rewritten to '0' so the engine
 * never sees them; pickup tiles are kept in the grid so
 * weapon_pickup() can read the weapon id on walk-over (it clears them itself).
 */
static void	fill_sprites(t_cub *cub)
{
	int	y;
	int	x;
	int	k;
	int	t;

	k = 0;
	y = -1;
	while (++y < cub->map.height)
	{
		x = -1;
		while (cub->map.grid[y][++x])
		{
			t = tile_type(cub->map.grid[y][x]);
			if (t < 0)
				continue ;
			cub->sprites[k].x = (double)x + 0.5;
			cub->sprites[k].y = (double)y + 0.5;
			cub->sprites[k].type = t;
			cub->sprites[k].hp = 1 + (t == SP_FLUID) * (BOSS_HP - 1);
			if (t < SP_PICKUP)
				cub->map.grid[y][x] = '0';
			k++;
		}
	}
}

/**
 * @brief Allocates and populates the sprite array from the parsed grid. The
 * array is over-allocated to 2 * count (sprite_cap) so spawn_reward can append
 * one meat/money pickup per killable enemy at runtime. No-op when the map has
 * no sprite tiles.
 */
void	init_sprites(t_cub *cub)
{
	cub->sprite_count = count_sprites(cub);
	if (cub->sprite_count <= 0)
		return ;
	cub->sprite_cap = cub->sprite_count * 2;
	cub->sprites = ft_calloc(cub->sprite_cap, sizeof(t_sprite));
	if (!cub->sprites)
		exit_error(cub, "Memory allocation failed");
	fill_sprites(cub);
}

/**
 * @brief Removes the collected pickup's floor sprite at cell (mx, my) from the
 * render list by swapping in the last entry and shrinking the count (O(1),
 * order-agnostic since the sprite pass re-sorts every frame).
 */
void	drop_pickup_sprite(t_cub *cub, int mx, int my)
{
	int	i;

	i = -1;
	while (++i < cub->sprite_count)
	{
		if (cub->sprites[i].type >= SP_PICKUP
			&& (int)cub->sprites[i].x == mx
			&& (int)cub->sprites[i].y == my)
		{
			cub->sprites[i] = cub->sprites[cub->sprite_count - 1];
			cub->sprite_count--;
			return ;
		}
	}
}
