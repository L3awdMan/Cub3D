/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lives_hud_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zotaj-di <zotaj-di@learner.42.tech>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 00:07:40 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/06/10 00:07:40 by zotaj-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* SECTION 4 (src_bonus/bonus/lives_hud_bonus.c): "enemies remaining" lives
 * counter — load side. The 24 panel frames (lives_0..lives_23) are lazily
 * loaded once, then draw_lives_hud (lives_hud_draw_bonus.c) blits the frame
 * indexed by cub->lives into the floor-HUD black slot. lives starts at the
 * floor's killable enemy count (capped LIVES_MAX) and ticks down per kill. */

#include "cub3d_bonus.h"
#include "mlx.h"

/**
 * @brief Builds the malloc'd path LIVES_DIR "lives_<i>.xpm". Caller frees.
 */
static char	*lives_path(int i)
{
	char	*num;
	char	*mid;
	char	*path;

	num = ft_itoa(i);
	if (!num)
		return (NULL);
	mid = ft_strjoin(LIVES_DIR "lives_", num);
	free(num);
	if (!mid)
		return (NULL);
	path = ft_strjoin(mid, ".xpm");
	free(mid);
	return (path);
}

/**
 * @brief Counts the floor's killable entities (every sprite of type < SP_PICKUP
 * — robots, scientists and the boss), capped at LIVES_MAX so the result always
 * indexes a valid lives frame.
 */
int	count_enemies(t_cub *cub)
{
	int	i;
	int	n;

	n = 0;
	i = -1;
	while (++i < cub->sprite_count)
		if (cub->sprites[i].type < SP_PICKUP)
			n++;
	if (n > LIVES_MAX)
		n = LIVES_MAX;
	return (n);
}

/**
 * @brief Loads one lives frame into slot i, or exit_error on failure.
 */
static void	load_one_life(t_cub *cub, int i)
{
	t_img	*f;
	char	*path;

	path = lives_path(i);
	if (!path)
		exit_error(cub, "Memory allocation failed");
	f = &cub->lives_hud[i];
	f->id = mlx_xpm_file_to_image(cub->mlx, path, &f->width, &f->height);
	free(path);
	if (!f->id)
		exit_error(cub, "Failed to load lives counter texture");
	f->data = mlx_get_data_addr(f->id, &f->bpp, &f->line_len, &f->endian);
}

/**
 * @brief Lazily loads the 24 lives frames, records frame 0's transparent key
 * and seeds cub->lives with the floor's enemy count.
 */
void	load_lives_hud(t_cub *cub)
{
	int	i;

	i = -1;
	while (++i < 24)
		load_one_life(cub, i);
	cub->lives_transp = *(unsigned int *)cub->lives_hud[0].data;
	cub->lives = count_enemies(cub);
}
