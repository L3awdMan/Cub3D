/* SECTION 4 (src_bonus/bonus/endgame_bonus.c): end-game UI — load side.
 * capture_spawn() snapshots the floor's start (player + initial sprites) so
 * respawn() can restore it. load_endgame_ui() lazily loads the 6 blur->clear
 * "YOU ARE DEAD" frames and the "YOU WIN" frame on the first death/win, so
 * the big full-screen XPMs never stall startup. State + draw live in
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
 * @brief Builds the malloc'd path ENDGAME_DIR "game_over_<i>.xpm" (i is 1..6,
 * always single-digit). Caller frees.
 */
static char	*gameover_path(int i)
{
	char	*num;
	char	*mid;
	char	*path;

	num = ft_itoa(i);
	if (!num)
		return (NULL);
	mid = ft_strjoin(ENDGAME_DIR "game_over_", num);
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
 * @brief Lazily loads the 6 dead frames + the win frame (one-time, on first
 * death or win).
 */
void	load_endgame_ui(t_cub *cub)
{
	int	i;

	i = -1;
	while (++i < DEAD_FRAMES)
		load_ui(cub, &cub->dead_ui[i], dead_path(i));
	load_ui(cub, &cub->win_ui, ft_strjoin(ENDGAME_DIR "you_win_ui", ".xpm"));
	i = -1;
	while (++i < GAMEOVER_FRAMES)
		load_ui(cub, &cub->gameover_ui[i], gameover_path(i + 1));
}
