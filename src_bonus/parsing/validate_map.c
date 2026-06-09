/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_map.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zotaj-di <zotaj-di@learner.42.tech>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 00:10:01 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/06/10 00:10:01 by zotaj-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

/**
 * @brief Check if a character is a walkable cell (0 or spawn)
 *
 * SECTION 4 (src_bonus/parsing/validate_map.c): sprite spawns count
 * as walkable floor for the enclosure check — init_sprites() rewrites them
 * to '0' after validation, so the rest of the engine never sees it.
 */
static int	is_walkable(char c)
{
	return (c == '0' || c == '2' || c == '3' || c == 'A'
		|| c == 'B' || c == 'F' || c == 'Z' || c == 'D'
		|| c == 'N' || c == 'S' || c == 'E' || c == 'W'
		|| (c && ft_strchr(PICKUP_CHARS, c)));
}

/**
 * @brief Checks 1 neighbor cell at (ny, nx)
 * @note Out of bounds or space neighbors = map is not enclosed
 */
static void	check_neighbor(t_cub *cub, int ny, int nx)
{
	int	row_len;

	if (ny < 0 || ny >= cub->map.height)
		exit_error(cub, "Map is not enclosed by walls");
	row_len = ft_strlen(cub->map.grid[ny]);
	if (nx < 0 || nx >= row_len)
		exit_error(cub, "Map is not enclosed by walls");
	if (cub->map.grid[ny][nx] == ' ')
		exit_error(cub, "Map is not enclosed by walls");
}

/**
 * @brief Checks all 8 neighbors of a walkable cell at (y, x)
 */
static void	check_cell(t_cub *cub, int y, int x)
{
	int	delta_y;
	int	delta_x;

	delta_y = -1;
	while (delta_y <= 1)
	{
		delta_x = -1;
		while (delta_x <= 1)
		{
			if (delta_y != 0 || delta_x != 0)
				check_neighbor(cub, y + delta_y, x + delta_x);
			delta_x++;
		}
		delta_y++;
	}
}

/**
 * @brief Processes one cell
 */
static void	process_cell(t_cub *cub, int y, int x, int *spawn)
{
	char	c;

	c = cub->map.grid[y][x];
	if (is_walkable(c))
		check_cell(cub, y, x);
	if (c == 'N' || c == 'S' || c == 'E' || c == 'W')
	{
		(*spawn)++;
		init_player(cub, y, x, c);
	}
}

/**
 * @brief Validates the map: checks enclosure and spawns (no more than 1)
 */
void	validate_map(t_cub *cub)
{
	int	y;
	int	x;
	int	spawn;

	spawn = 0;
	y = 0;
	while (y < cub->map.height)
	{
		x = 0;
		while (cub->map.grid[y][x])
		{
			process_cell(cub, y, x, &spawn);
			x++;
		}
		y++;
	}
	if (spawn != 1)
		exit_error(cub, "Map must have exactly one player spawn");
}
