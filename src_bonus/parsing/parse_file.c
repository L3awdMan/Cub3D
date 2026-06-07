#include "cub3d_bonus.h"

/**
 * @brief Validates the .cub extension
 */
static void	check_extension(t_cub *cub, char *path)
{
	int	len;

	len = ft_strlen(path);
	if (len < 5 || ft_strncmp(path + len - 4, ".cub", 4) != 0)
		exit_error(cub, "File must have .cub extension");
}

/**
 * @brief Check if a line looks like the start of a map (space, 0 or 1)
 *
 * FIX 4b (src/parsing/parse_file.c): N/S/E/W are accepted as a legal first
 * non-whitespace character on the first map row (spawn at column 0).
 * dispatch_line() runs identify_element() first, so two-letter "NO"/"SO"
 * texture identifiers are still routed to parse_texture, not here.
 */
static int	is_map_line(char *line)
{
	while (*line && ft_isspace(*line) && *line != '\n')
		line++;
	if (*line == '1' || *line == '0'
		|| *line == 'N' || *line == 'S'
		|| *line == 'E' || *line == 'W')
		return (1);
	return (0);
}

/**
 * @brief Dispatches a configuration line to the corresponding parser
 */
static void	dispatch_line(t_cub *cub, char *line, int *fd)
{
	int	id;

	id = identify_element(line);
	if (id >= TEX_NO && id <= TEX_EA)
		parse_texture(cub, line, id);
	else if (id == C_FLOOR)
		parse_color(cub, line, 0);
	else if (id == C_CEIL)
		parse_color(cub, line, 1);
	else if (id == ELEM_FLOOR)
		parse_floor(cub, line);
	else if (is_map_line(line))
	{
		if (cub->map.parsed_flags != FLAG_ALL)
			exit_error(cub, "Map found before all elements");
		store_map_lines(cub, line, *fd);
	}
	else
		exit_error(cub, "Unknown identifier in config");
}

/**
 * @brief FIX 4a (src/parsing/parse_file.c): once the map is stored, any
 * non-blank line is a hard error. The old loop broke as soon as map.grid
 * was set and drain_gnl() silently swallowed the rest, letting trailing
 * junk, duplicate maps or stray identifiers pass. This also covers Fix 3c:
 * a blank line midway in the map ends store_map_lines(), so the next
 * non-blank line lands here as "Content after map" instead of being lost.
 */
static void	post_map_check(t_cub *cub, char *line, int fd)
{
	int	i;

	i = 0;
	while (line[i] && ft_isspace(line[i]))
		i++;
	if (line[i] == '\0')
		return ;
	free(line);
	close(fd);
	exit_error(cub, "Content after map");
}

/**
 * @brief Main parser entry: open file, read lines, validate map
 */
void	parse_file(t_cub *cub, char *path)
{
	int		fd;
	char	*line;

	check_extension(cub, path);
	fd = open(path, O_RDONLY);
	if (fd < 0)
		exit_error(cub, "Cannot open file");
	line = get_next_line(fd);
	while (line)
	{
		cub->pending_line = line;
		if (cub->map.grid)
			post_map_check(cub, line, fd);
		else if (line[0] != '\n')
			dispatch_line(cub, line, &fd);
		cub->pending_line = NULL;
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	if (!cub->map.grid)
		exit_error(cub, "No map found in file");
	validate_map(cub);
}
