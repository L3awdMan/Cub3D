/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_elements.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 10:19:03 by baelgadi          #+#    #+#             */
/*   Updated: 2026/04/12 19:17:01 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * @brief FIX 5 helper (src/parsing/parse_elements.c): match identifier `id`
 * (of length `len`) followed by any whitespace. Replaces the previous literal
 * "NO "/"SO "/"WE "/"EA "/"F "/"C " ft_strncmp checks which only accepted a
 * single space — `NO\t./tex.xpm` (tab-separated) used to be rejected as
 * "Unknown identifier in config". Subject allows "one or more spaces".
 */
static int	matches_id(const char *s, const char *id, int len)
{
	return (ft_strncmp(s, id, len) == 0 && ft_isspace((unsigned char)s[len]));
}

/**
 * @brief Identifies the type of a config line
 * @details 0-3 for textures, 4 for Floor, 5 for Ceiling and -1 if unknown
 */
int	identify_element(char *line)
{
	while (*line && ft_isspace(*line))
		line++;
	if (matches_id(line, "NO", 2))
		return (TEX_NO);
	if (matches_id(line, "SO", 2))
		return (TEX_SO);
	if (matches_id(line, "WE", 2))
		return (TEX_WE);
	if (matches_id(line, "EA", 2))
		return (TEX_EA);
	if (matches_id(line, "F", 1))
		return (C_FLOOR);
	if (matches_id(line, "C", 1))
		return (C_CEIL);
	return (-1);
}

/**
 * @brief Skips the identifier prefix and leading whitespaces
 * @return Pointer to the path part of the line
 */
static char	*skip_to_path(char *line, int idx)
{
	while (*line && ft_isspace(*line))
		line++;
	if (idx <= TEX_EA)
		line += 3;
	else
		line += 2;
	while (*line && ft_isspace(*line))
		line++;
	return (line);
}

/**
 * @brief Parses a texture path line (NO, SO, WE or EA)
 * 
 * Checks for duplicates then trims and stores the path
 */
void	parse_texture(t_cub *cub, char *line, int idx)
{
	char	*path;
	char	*trimmed;

	if (cub->map.parsed_flags & (1 << idx))
		exit_error(cub, "Duplicate texture identifier");
	path = skip_to_path(line, idx);
	if (!*path)
		exit_error(cub, "Empty texture path");
	trimmed = ft_strtrim(path, " \t\n\r");
	if (!trimmed)
		exit_error(cub, "Memory allocation failed");
	cub->map.tex_path[idx] = trimmed;
	cub->map.parsed_flags |= (1 << idx);
}
