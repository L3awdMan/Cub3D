/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_colors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zotaj-di <zotaj-di@learner.42.tech>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 00:09:50 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/06/10 00:09:50 by zotaj-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

/**
 * @brief Packs RGB into a single int (0xRRGGBB)
 */
int	rgb_to_int(int r, int g, int b)
{
	return ((r << 16) | (g << 8) | b);
}

/**
 * @brief Validates a trimmed string that contains only digits (1-3 chars)
 * @return 1 if valid and 0 if not
 */
static int	is_valid_digits(char *trimmed)
{
	int	i;

	i = 0;
	while (trimmed[i])
	{
		if (!ft_isdigit(trimmed[i]))
			return (0);
		i++;
	}
	if (i == 0 || i > 3)
		return (0);
	return (1);
}

/**
 * @brief Validates one trimmed color component string
 * @return The int value or -1 on invalid input
 */
static int	parse_component(char *str)
{
	int		value;
	char	*trimmed;

	trimmed = ft_strtrim(str, " \t\n\r");
	if (!trimmed)
		return (-1);
	if (!is_valid_digits(trimmed))
	{
		free(trimmed);
		return (-1);
	}
	value = ft_atoi(trimmed);
	free(trimmed);
	if (value < 0 || value > 255)
		return (-1);
	return (value);
}

/**
 * @brief Splits and validates color string, extracting RGB values
 * @return 1 on success and 0 if failed
 */
static int	extract_rgb(char *line, int *rgb)
{
	char	**parts;

	parts = ft_split(line, ',');
	if (!parts || !parts[0] || !parts[1] || !parts[2] || parts[3])
	{
		ft_free_strarray(parts);
		return (0);
	}
	rgb[0] = parse_component(parts[0]);
	rgb[1] = parse_component(parts[1]);
	rgb[2] = parse_component(parts[2]);
	ft_free_strarray(parts);
	if (rgb[0] < 0 || rgb[1] < 0 || rgb[2] < 0)
		return (0);
	return (1);
}

/**
 * @brief Parses a color line (F or C) into an RGB int
 * @note Validates for [0, 255]
 */
void	parse_color(t_cub *cub, char *line, int is_ceiling)
{
	int	rgb[3];
	int	bit;

  ft_bzero(rgb, 0);
	bit = 4 + is_ceiling;
	if (cub->map.parsed_flags & (1 << bit))
		exit_error(cub, "Duplicate color identifier");
	while (*line && ft_isspace(*line))
		line++;
	line += 2;
	if (!extract_rgb(line, rgb))
		exit_error(cub, "Invalid color format");
	if (is_ceiling)
		cub->map.ceil_col = rgb_to_int(rgb[0], rgb[1], rgb[2]);
	else
		cub->map.floor_col = rgb_to_int(rgb[0], rgb[1], rgb[2]);
	cub->map.parsed_flags |= (1 << bit);
}
