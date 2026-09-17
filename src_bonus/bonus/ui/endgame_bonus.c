/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   endgame_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zotaj-di <zotaj-di@learner.42.tech>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 00:08:33 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/06/10 00:08:33 by zotaj-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* end-game UI — load side.
 * capture_spawn() snapshots the floor's start (player + initial sprites) so
 * respawn() can restore it. load_endgame_ui() lazily loads the dead frames,
 * the win frame and the terminal retry/quit screens. State + draw live in
 * endgame_state_bonus.c. */

#include "cub3d_bonus.h"
#include "mlx.h"

/**
 * @brief Builds the malloc'd path ENDGAME_DIR "you_are_dead_frame_0<i>.xpm"
 * (i is 0..5, always single-digit). Caller frees.
 */
static char	*dead_path(int i)
{
	char	*num;
	char	*mid;
	char	*path;

	num = ft_itoa(i);
	if (!num)
		return (NULL);
	mid = ft_strjoin(ENDGAME_DIR "you_are_dead_frame_0", num);
	free(num);
	if (!mid)
		return (NULL);
	path = ft_strjoin(mid, ".xpm");
	free(mid);
	return (path);
}

/**
 * @brief Snapshots the floor's spawn state: the player camera and a fresh copy
 * of the initial sprite array. Re-callable per floor (frees the old snapshot).
 */
void	capture_spawn(t_cub *cub)
{
	cub->spawn_player = cub->player;
	if (cub->spawn_sprites)
		free(cub->spawn_sprites);
	cub->spawn_sprites = NULL;
	cub->spawn_count = 0;
	if (cub->sprite_count <= 0)
		return ;
	cub->spawn_sprites = ft_calloc(cub->sprite_count, sizeof(t_sprite));
	if (!cub->spawn_sprites)
		exit_error(cub, "Memory allocation failed");
	ft_memcpy(cub->spawn_sprites, cub->sprites,
		cub->sprite_count * sizeof(t_sprite));
	cub->spawn_count = cub->sprite_count;
}

/**
 * @brief Loads one full-screen UI frame into f from path, exit_error on fail.
 */
static void	load_ui(t_cub *cub, t_img *f, char *path)
{
	if (!path)
		exit_error(cub, "Memory allocation failed");
	f->id = mlx_xpm_file_to_image(cub->mlx, path, &f->width, &f->height);
	free(path);
	if (!f->id)
		exit_error(cub, "Failed to load end-game UI texture");
	f->data = mlx_get_data_addr(f->id, &f->bpp, &f->line_len, &f->endian);
}

/**
 * @brief Lazily loads the dead frames, win frame, and terminal game-over
 * retry/quit screens.
 */
void	load_endgame_ui(t_cub *cub)
{
	int	i;

	i = -1;
	while (++i < DEAD_FRAMES)
		load_ui(cub, &cub->dead_ui[i], dead_path(i));
	load_ui(cub, &cub->win_ui, ft_strjoin(ENDGAME_DIR "you_win_ui", ".xpm"));
	load_ui(cub, &cub->gameover_retry,
		ft_strjoin(ENDGAME_DIR "gameover_retry", ".xpm"));
	load_ui(cub, &cub->gameover_quit,
		ft_strjoin(ENDGAME_DIR "gameover_quit", ".xpm"));
}
