/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zotaj-di <zotaj-di@learner.42.tech>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 00:09:58 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/06/10 00:09:58 by zotaj-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

/**
 * @brief Validates characters in 1 map line
 *
 * FIX 3a (src/parsing/parse_map.c): TAB and CR are accepted alongside space
 * (CRLF / tab-indented maps); normalize_row() rewrites them before storage.
 * SECTION 4 (src_bonus): 'D' (door) and sprite spawns are also valid map
 * characters in the bonus build.
 */
static void	check_map_chars(t_cub *cub, char *line)
{
	int	i;

	i = 0;
	while (line[i] && line[i] != '\n')
	{
		if (line[i] != '0' && !ft_strchr(WALL_CHARS, line[i])
			&& !ft_strchr(PICKUP_CHARS, line[i])
			&& line[i] != ' '
			&& line[i] != '\t' && line[i] != '\r' && line[i] != 'D'
			&& line[i] != '2' && line[i] != '3' && line[i] != 'A'
			&& line[i] != 'B' && line[i] != 'F' && line[i] != 'Z'
			&& line[i] != 'N' && line[i] != 'S'
			&& line[i] != 'W' && line[i] != 'E')
			exit_error(cub, "Invalid character in map");
		i++;
	}
}

/**
 * @brief FIX 3b (src/parsing/parse_map.c): normalize a stored row in place.
 * Converts TAB to space and drops CR so the grid stays uniform for
 * can_move(), raycaster DDA and validate_map() — all of which index
 * `grid[y][x]` directly and only understand 0/1/space/N/S/E/W.
 */
static void	normalize_row(char *row)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (row[i])
	{
		if (row[i] == '\t')
			row[j++] = ' ';
		else if (row[i] != '\r')
			row[j++] = row[i];
		i++;
	}
	row[j] = '\0';
}

/**
 * @brief Grows the grid array and appends 1 new row
 */
static void	append_row(t_cub *cub, char *row)
{
	char	**new_grid;
	int		i;

	new_grid = ft_calloc(cub->map.height + 1, sizeof(char *));
	if (!new_grid)
	{
		free(row);
		exit_error(cub, "Memory allocation failed");
	}
	i = 0;
	while (i < cub->map.height)
	{
		new_grid[i] = cub->map.grid[i];
		i++;
	}
	new_grid[i] = row;
	free(cub->map.grid);
	cub->map.grid = new_grid;
	cub->map.height++;
}

/**
 * @brief Processes 1 map line (validate -> trim newline -> append)
 * 
 */
static void	add_map_line(t_cub *cub, char *line)
{
	char	*row;
	int		len;

	check_map_chars(cub, line);
	row = ft_strtrim(line, "\n");
	if (!row)
		exit_error(cub, "Memory allocation failed");
	normalize_row(row);
	append_row(cub, row);
	len = ft_strlen(row);
	if (len > cub->map.width)
		cub->map.width = len;
}

/**
 * @brief Reads all map lines from the first one to EOF
 *
 * @note Reuses cub->pending_line (owned by parse_file) as the current-line
 * slot so a mid-map exit_error frees exactly one tracked pointer in
 * cub_destroy. On a blank line the loop stops leaving cub->pending_line on
 * that blank line; parse_file frees it and re-reads, routing any trailing
 * content to post_map_check().
 */
void	store_map_lines(t_cub *cub, int fd)
{
	add_map_line(cub, cub->pending_line);
	free(cub->pending_line);
	cub->pending_line = get_next_line(fd);
	while (cub->pending_line && cub->pending_line[0] != '\n')
	{
		add_map_line(cub, cub->pending_line);
		free(cub->pending_line);
		cub->pending_line = get_next_line(fd);
	}
}
